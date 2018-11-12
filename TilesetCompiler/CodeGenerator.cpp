//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include "CodeGenerator.h"
#include <KAOS/Common/Utilities.h>
#include <KAOS/Imaging/ImageUtils.h>
#include <fstream>
#include <iostream>
#include <array>
#include <string>
#include <vector>
#include <type_traits>
#include <sstream>



void CodeGenerator::GenerateTileCode(
	std::ostream& output,
	const KAOS::Imaging::Image& image,
	unsigned int id,
	bool optimize, 
	int64_t displayPitch) const
{
	struct RowInfo	//	FIXME: Needs proper ctor
	{
		BitmapRow				row;
		int64_t					offset;
		std::vector<int64_t>	offsets;
	};

	const auto labelBase("Tile" + std::to_string(id));
	const auto& bitmapRows(image.GetRows());

	std::vector<RowInfo> rows;

	//	Set up the original bitmap for optimization
	for (auto i(0u); i < bitmapRows.size(); ++i)
	{
		RowInfo info;
		info.row = bitmapRows[i];
		info.offset = i * displayPitch;
		info.offsets.emplace_back(info.offset);
		rows.emplace_back(info);
	}

	if (optimize)
	{
		for (auto i(0u); i < rows.size(); ++i)
		{
			RowInfo& info(rows[i]);
			for (auto x(i + 1); x < rows.size(); ++x)
			{
				auto& other(rows[x]);
				if (info.row == other.row)
				{
					info.offsets.insert(info.offsets.end(), other.offsets.begin(), other.offsets.end());
					other.offsets.clear();
				}
			}
		}
	}

	std::ostringstream outputTextureAnimCode;
	std::ostringstream outputTexturePreRenderCode;
	std::ostringstream outputTextureLeftHalfCode;
	std::ostringstream outputTextureRightHalfCode;
	for (auto i(0u); i < rows.size(); ++i)
	{
		GenerateRowCode(
			outputTextureAnimCode,
			rows[i].offsets,
			1,
			rows[i].row);

		GenerateRowCode(
			outputTexturePreRenderCode,
			rows[i].offsets,
			4,
			rows[i].row);

		GenerateLeftHalfColumnCode(
			outputTextureLeftHalfCode,
			rows[i].offsets,
			rows[i].row);

		GenerateRightHalfColumnCode(
			outputTextureRightHalfCode,
			rows[i].offsets,
			rows[i].row);
	}


	output
		<< "***************************************************************\n"
		<< "* Tile #" << id << "\n"
		<< "***************************************************************\n"
		<< labelBase << "\n"
		<< "\tFDB\t" << std::to_string(id) << "\n"
		<< "\tFDB\t" << labelBase << "DataEnd-" << labelBase << "DataBegin\n"
		<< labelBase << "DataBegin\n"
		<< "\tFDB\t" << labelBase << "StaticTexture-" << labelBase << "DataBegin\n"
		<< "\tFDB\t" << labelBase << "AnimTexture-" << labelBase << "DataBegin\n"
		<< "\tFDB\t" << labelBase << "LeftTexture-" << labelBase << "DataBegin\n"
		<< "\tFDB\t" << labelBase << "RightTexture-" << labelBase << "DataBegin\n"
		<< labelBase << "StaticTexture\n"
		<< outputTexturePreRenderCode.str()
		<< "\trts\n"
		<< labelBase << "AnimTexture\n"
		<< outputTextureAnimCode.str()
		<< "\trts\n"
		<< labelBase << "LeftTexture\n"
		<< outputTextureLeftHalfCode.str()
		<< "\trts\n"
		<< labelBase << "RightTexture\n"
		<< outputTextureRightHalfCode.str()
		<< labelBase << "DataEnd\n"
		<< "*\n";
}


void CodeGenerator::GenerateTileCode(
	std::ostream& output,
	unsigned int id,
	unsigned int aliasId) const
{
	const auto labelBase("Tile" + std::to_string(id));

	output
		<< "***************************************************************\n"
		<< "* Tile #" << id << "\n"
		<< "***************************************************************\n"
		<< labelBase << "\n"
		<< "\tFDB\t$" << KAOS::Common::to_hex_string(id, 4) << "\n"
		<< "\tFDB\t$" << KAOS::Common::to_hex_string(0x8000 | aliasId, 4) << "\n"
		<< "*\n";
}



void CodeGenerator::GenerateRowCode(
	std::ostream& output,
	const std::vector<int64_t>& offsets,
	int64_t offsetMultiplier,
	const BitmapRow& rowData) const
{
	if (offsets.empty())
	{
		return;
	}

	uint32_t rowValue(
		  KAOS::Imaging::MergePixels(rowData[0], rowData[1]) << 24
		| KAOS::Imaging::MergePixels(rowData[2], rowData[3]) << 16
		| KAOS::Imaging::MergePixels(rowData[4], rowData[5]) << 8
		| KAOS::Imaging::MergePixels(rowData[6], rowData[7]));

	output << "\tLDQ\t#$" << KAOS::Common::to_hex_string(rowValue, 8) << "\n";
	for (const auto& offset : offsets)
	{
		output << "\tSTQ\t$" << KAOS::Common::to_hex_string(offset * offsetMultiplier, 4) << ",y\n";
	}

}



//	FIXME: This specialization is unnecessary. Update the mainline compiler to extract the column as-is
//	and update the compiler to select a code generator based on the size of the image and include
//	additional command line arguments to control it.
void CodeGenerator::GenerateLeftHalfColumnCode(
	std::ostream& output,
	const std::vector<int64_t>& offsets,
	const BitmapRow& rowData) const
{
	if (offsets.empty())
	{
		return;
	}

	uint32_t rowValue(KAOS::Imaging::MergePixels(rowData[0], rowData[1]) << 8 | KAOS::Imaging::MergePixels(rowData[2], rowData[3]));

	output << "\tLDD\t#$" << KAOS::Common::to_hex_string(rowValue, 4) << "\n";
	for (const auto& offset : offsets)
	{
		output << "\tSTD\t$" << KAOS::Common::to_hex_string(offset, 4) << ",y\n";
	}

}



//	FIXME: This specialization is unnecessary. Update the mainline compiler to extract the column as-is
//	and update the compiler to select a code generator based on the size of the image and include
//	additional command line arguments to control it.
void CodeGenerator::GenerateRightHalfColumnCode(
	std::ostream& output,
	const std::vector<int64_t>& offsets,
	const BitmapRow& rowData) const
{
	if (offsets.empty())
	{
		return;
	}

	const uint32_t rowValue(KAOS::Imaging::MergePixels(rowData[4], rowData[5]) << 8 | KAOS::Imaging::MergePixels(rowData[6], rowData[7]));


	output << "\tLDD\t#$" << KAOS::Common::to_hex_string(rowValue, 4) << "\n";
	for (const auto& offset : offsets)
	{
		output << "\tSTD\t$" << KAOS::Common::to_hex_string(offset+2, 4) << ",y\n";
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
