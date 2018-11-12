//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include "TilesetCompiler.h"
#include <KAOS/Imaging/ImageUtils.h>
#include <KAOS/Imaging/Palette.h>
#include <fstream>
#include <iostream>
#include <array>
#include <string>
#include <vector>
#include <type_traits>
#include <sstream>





bool TilemapCompiler::Compile(
	const KAOS::Imaging::Image& image,
	const KAOS::Imaging::Palette& palette,
	size_t count,
	const std::string& tileDirectory,
	const std::string& tilesetFilename,
	const std::string& paletteFilename,
	bool optimize,
	size_t displayPitch) const
{
	auto tileData(ExtractTextures(image, count));
	ConslidateDuplicates(tileData);
	SaveTiles(tileData, tileDirectory, optimize, displayPitch);
	SaveTilemap(tileData, tilesetFilename, tileDirectory);
	SavePalette(paletteFilename, palette);

	return true;
}



void TilemapCompiler::ConslidateDuplicates(TileContainerType& textureList) const
{
	for (auto texturePtr(textureList.begin()); texturePtr != textureList.end(); ++texturePtr)
	{
		if (!texturePtr->HasIdAlias())
		{
			auto nextTexturePtr(texturePtr);
			++nextTexturePtr;

			for (; nextTexturePtr < textureList.end(); ++nextTexturePtr)
			{
				if (!nextTexturePtr->HasIdAlias() && *nextTexturePtr->GetImage() == *texturePtr->GetImage())
				{
					nextTexturePtr->SetAliasId(texturePtr->GetId());
				}
			}
		}
	}
}


TilemapCompiler::TileContainerType TilemapCompiler::ExtractTextures(const KAOS::Imaging::Image& image, size_t count) const
{
	TileContainerType tiles;

	static const size_t Columns = 32;
	static const size_t Rows = 32;
	for (auto y(0U); count > 0 && y < Rows; ++y)
	{
		for (auto x(0U); count > 0 && x < Columns; ++x)
		{
			tiles.emplace_back(image.Extract(x * 8, y * 8, 8, 8), (y * Columns) + x);
			--count;
		}
	}

	return tiles;
}




void TilemapCompiler::SaveTiles(const TileContainerType& tileData, const std::string& directory, bool optimize, size_t displayPitch) const
{
	for (auto i(0U); i < tileData.size(); ++i)
	{
		auto outputFilename(directory + "/Tile" + std::to_string(i) + ".inc");
		std::ofstream output(outputFilename);
		if (!output.is_open())
		{
			std::cerr << "Unable to open `" + outputFilename + "`\n";

		}
		if (!tileData[i].HasIdAlias())
		{
			m_CodeGen.GenerateTileCode(output, *tileData[i].GetImage(), i, optimize, displayPitch);
		}
		else
		{
			m_CodeGen.GenerateTileCode(output, i, tileData[i].GetAliasOrId());
		}
	}

	std::cout << "Saved " << tileData.size() << " tiles\n";
}



void TilemapCompiler::SaveTilemap(const TileContainerType& tileData, const std::string& filename, const std::string& tileDirectory) const
{
	std::ofstream output(filename);
	output
		<< "***************************************************************\n"
		<< "* Tilemap\n"
		<< "***************************************************************\n"
		<< "*\n";

	output
		<< "*\n"
		<< "Tilemap:\n"
		<< "*\n";
#if 0
	for (const auto& texture : tileData)
	{
		output << "\t FDB\t" << "Tile" + std::to_string(texture.GetAliasOrId()) << "DataBegin\n";
	}
#else
	output << "\t FCC\t'TILESETX'\n";
#endif

	//////////
	for (auto i(0U); i < tileData.size(); ++i)
	{
		auto includeFilename("Tile" + std::to_string(i) + ".inc");

		output << "\tinclude\t" << tileDirectory + "/" + includeFilename << "\n";
	}

	output
		<< "\tFDB\t$FFFF\n"
		<< "\tFDB\t$0000\n*\n";


}



bool TilemapCompiler::SavePalette(const std::string& filename, const KAOS::Imaging::Palette& palette) const
{
	std::ofstream output(filename);

	if (!output.is_open())
	{
		std::cerr << "Unable to open or create `" << filename << "` for writing\n";
		return false;
	}

	output << "TilesetPalette\tFCB\t";

	for (const auto& color : palette)
	{
		output
			<< (&color != &palette.front() ? "," : "")
			<< static_cast<int>(KAOS::Imaging::ConvertColorToRGBRGB(color));
	}

	return true;
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
