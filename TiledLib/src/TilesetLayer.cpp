//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include <Tiled/TilesetLayer.h>
#include <KAOS/Common/Utilities.h>
#include <iostream>


namespace KAOS { namespace Tiled
{

	bool TilesetLayer::Parse(const pugi::xml_node& layer)
	{
		if (!Layer::Parse(layer))
		{
			return false;
		}

		Size dimensions;
		if (!dimensions.Parse(layer))
		{
			return false;
		}


		const auto& layerData(layer.child("data"));
		if (layerData.empty())
		{
			std::cerr << "Layer does not have any data\n";
			return false;
		}

		const auto& layerDataEncodingAttr(layerData.attribute("encoding"));
		if (layerDataEncodingAttr.empty())
		{
			std::cerr << "Layer data does not have an encoding attribute\n";
			return false;
		}

		const std::string layerDataEncoding(layerDataEncodingAttr.as_string());
		if (layerDataEncoding == "csv")
		{
			if (layerData.first_child().empty())
			{
				std::cerr << "Layer data is missing child container\n";
				return false;
			}

			return ParseCSV(dimensions, layerData.first_child().value());
		}

		std::cerr << "Unknown encoding `" << layerDataEncoding << "`\n";

		return false;
	}




	bool TilesetLayer::ParseCSV(Size dimensions, const std::string& data)
	{
		auto values(Common::ConvertToInteger(Common::SplitString(data, ",")));

		if (dimensions.GetCount() != values.size())
		{
			std::cerr << "Size mismatch. Width * Height does not match size of data\n";
			return false;
		}

		m_Dimensions = dimensions;
		m_Data = move(values);

		return true;
	}




	Size TilesetLayer::GetDimensions() const
	{
		return m_Dimensions;
	}


	TilesetLayer::size_type TilesetLayer::size() const
	{
		return m_Data.size();
	}


	TilesetLayer::const_iterator TilesetLayer::begin() const
	{
		return m_Data.cbegin();
	}


	TilesetLayer::const_iterator TilesetLayer::end() const
	{
		return m_Data.cend();
	}

}}




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
