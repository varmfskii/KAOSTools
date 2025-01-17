//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include "DescriptorNodes/TiledLayer.h"
#include <KAOS/Common/Logging.h>
#include <KAOS/Common/xml.h>
#include <algorithm>
#include <deque>
#include <iostream>
#include <iomanip>


namespace DescriptorNodes
{

	bool TiledLayer::Parse(const pugi::xml_node& node)
	{
		const auto& indexAttr(node.attribute("index"));
		const auto& srcAttr(node.attribute("src"));

		if (!indexAttr.empty() && !srcAttr.empty())
		{
			KAOS::Logging::Error("TiledLayer cannot have both an `index` and `src` attributes");
			return false;
		}

		if (indexAttr.empty() && srcAttr.empty())
		{
			KAOS::Logging::Error("TiledLayer must have either an `index` or `src` attribute");
			return false;
		}

		std::optional<uint64_t> layerIndex;
		if (!indexAttr.empty())
		{
			layerIndex = indexAttr.as_ullong();
		}

		std::optional<std::string> layerName;
		if (!srcAttr.empty())
		{
			layerName = srcAttr.as_string();
		}

		decltype(m_Signature) signature;
		KAOS::Common::XML::LoadAttribute(node, "signature", signature);
		

		if (!DescriptorNode::Parse(node))
		{
			return false;
		}


		m_LayerIndex = move(layerIndex);
		m_LayerName = move(layerName);
		m_Signature = move(signature);


		return true;
	}



	bool TiledLayer::CompileDefinition(definitionbuilder_type& /*builder*/) const
	{
		return true;
	}

	bool TiledLayer::CompileInstance(databuilder_type& builder, const datasource_type& dataSource, const Configuration& configuration) const
	{
		auto& mapDataSource(dynamic_cast<const Builder::MapDataSource&>(dataSource));

		std::shared_ptr<Builder::MapDataSource::tilesetlayer_type> layer;
		if (m_LayerName.has_value())
		{
			layer = mapDataSource.QueryTilesetLayer(*m_LayerName);
			if (!layer)
			{
				KAOS::Logging::Error("Unable to find tiled layer named `" + *m_LayerName + "`");
				return false;
			}
		}
		if (m_LayerIndex.has_value())
		{
			layer = mapDataSource.QueryTilesetLayer(*m_LayerIndex);
			if (!layer)
			{
				KAOS::Logging::Error("Unable to find tiled layer at index `" + std::to_string(*m_LayerIndex) + "`");
				return false;
			}
		}


		builder.EmitBlank();
		builder.EmitSeparatorComment();
		builder.EmitComment("Map layer `" + layer->GetName() + "'");
		builder.EmitSeparatorComment();
		if (!GetSymbol().empty())
		{
			builder.EmitLabel(GetSymbol());
		}

		const signature_type signature(m_Signature.has_value() ? signature_type(m_Signature.value()) : Signature);

		builder.EmitValue(std::string(), signature, "Block Signature");
		builder.EmitValue(std::string(), static_cast<Builder::DataBuilder::property_type::quad_type>(layer->size()), "Number of cells");
		builder.EmitComment("");

		const auto emptyCellId(0U);	//	FIXME: This needs to come from somewhere (datasource?)
		const auto layerSize(layer->GetDimensions());

		//	Build into rows
		std::deque<Builder::MapDataSource::tilesetlayer_type::cell_type> data(layer->begin(), layer->end());
		std::vector<rowcontainer_type> mapDataByRow;
		while (!data.empty())
		{
			auto lineEnd(data.begin());
			advance(lineEnd, layerSize.GetWidth());


			rowcontainer_type newRow(data.begin(), lineEnd);
			for (auto& cell : newRow)
			{
				cell = (!cell ? emptyCellId : cell - 1);
			}

			mapDataByRow.emplace_back(move(newRow));

			data.erase(data.begin(), lineEnd);
		}

		//	Compress rows
		if (configuration.compressTileLayers)
		{
			for (auto& rowData : mapDataByRow)
			{
				rowData = compressRowData(rowData);
			}
		}

		//	Generate the data
		for (const auto& rowData : mapDataByRow)
		{
			for (const auto& id : rowData)
			{
				//	FIXME: Check for bounds error
				builder.EmitValue(std::string(), databuilder_type::property_type::byte_type(id));
			}

			builder.Flush();
		}

		return true;
	}


	TiledLayer::rowcontainer_type TiledLayer::compressRowData(rowcontainer_type rowData) const
	{
		const ptrdiff_t MAX_RLE_BYTES = 127U;
		rlecontainer_type rleData;


		while (!rowData.empty())
		{
			auto runBegin(rowData.begin());
			auto runEnd(std::find_if_not(runBegin + 1, rowData.end(), [runBegin](rowcontainer_type::value_type x) { return x == *runBegin; }));
			//auto distance(std::min(MAX_RLE_BYTES, distance(runBegin, runEnd)));
			auto distance(std::min(MAX_RLE_BYTES, std::distance(runBegin, runEnd)));
			runEnd = runBegin;
			advance(runEnd, distance);

			if (distance <= 0 || distance >= MAX_RLE_BYTES)
			{
				throw std::runtime_error("Distance cannot be 0");
			}

			rleData.emplace_back(rowcontainer_type::value_type(distance), *runBegin);

			rowData.erase(runBegin, runEnd);
		}


		rowcontainer_type newRowData;
		while (!rleData.empty())
		{
			auto rleGroupEnd(rleData.begin());
			auto totalDataBytes = 0U;
			auto totalChunkBytes = 0U;
			while (rleGroupEnd != rleData.end())
			{
				if (rleGroupEnd != rleData.begin())
				{
					if (totalDataBytes + rleGroupEnd->first > MAX_RLE_BYTES || totalDataBytes + rleGroupEnd->first > totalChunkBytes + 2)
					{
						break;
					}
				}

				totalChunkBytes += 2;
				totalDataBytes += rleGroupEnd->first;
				++rleGroupEnd;
			}

			const auto rleGroupCount(distance(rleData.begin(), rleGroupEnd));
			if (rleGroupCount == 0)
			{
				throw std::runtime_error("Cannot process 0 rle elements");
			}

			auto rleGroupPtr(rleData.begin());
			if(rleGroupCount == 1)
			{
				newRowData.push_back(rleGroupPtr->first);
				newRowData.push_back(rleGroupPtr->second);
			}
			else
			{
				rowcontainer_type chunkData;
				while (rleGroupPtr != rleGroupEnd)
				{
					rowcontainer_type appendableChunkData(rleGroupPtr->first, rleGroupPtr->second);
					chunkData.insert(chunkData.end(), appendableChunkData.begin(), appendableChunkData.end());
					++rleGroupPtr;
				}

				if (chunkData.size() > 127)
				{
					throw std::runtime_error("Cannot create chunk");
				}

				newRowData.push_back(chunkData.size() | 0x80);
				newRowData.insert(newRowData.end(), chunkData.begin(), chunkData.end());
			}

			rleData.erase(rleData.begin(), rleGroupEnd);
		}

		//rleData = mergeRunLenths(move(rleData));

		//rowcontainer_type newRowData;
		//for (const auto& runLength : rleData)
		//{
		//	newRowData.push_back(runLength.first);
		//	newRowData.insert(newRowData.end(), runLength.second);
		//}
		newRowData.push_back(0);

		return newRowData;
	}

	TiledLayer::rlecontainer_type TiledLayer::mergeRunLenths(rlecontainer_type rowData) const
	{
		return rowData;
	}


}




//	Copyright (c) 2018 Chet Simpson
//	
//	Permission is hereby granted, free of charge, to any person
//	obtaining a copy of this software and associated documentation
//	files (the "Software"), to deal in the Software without
//	restriction, including without limitation the rights to use,
//	copy, modify, merge, publish, distribute, sublicense, and/or sell
//	copies of the Software, and to permit persons to whom the
//	Software is furnished to do so, subject to the following
//	conditions:
//	
//	The above copyright notice and this permission notice shall be
//	included in all copies or substantial portions of the Software.
//	
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
//	OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
//	NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//	HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//	WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//	FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//	OTHER DEALINGS IN THE SOFTWARE.
