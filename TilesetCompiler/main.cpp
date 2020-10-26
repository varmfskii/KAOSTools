//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include "TilesetBuilder.h"
#include "DataGenerator.h"
#include "CodeGenerator.h"
#include <KAOS/Imaging/Image.h>
#include <KAOS/Imaging/ImageUtils.h>
#include <KAOS/Common/Utilities.h>
#include <iostream>
#include <deque>
#include <string>
#include <optional>


std::vector<Tile> ExtractTiles(const KAOS::Imaging::Image& image, size_t count)
{
	std::vector<Tile> tiles;

	static const size_t Columns = image.GetWidth() / 8;
	static const size_t Rows = image.GetHeight() / 8;
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



void ConslidateDuplicates(std::vector<Tile>& textureList)
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




int main(int argc, const char** argv)
{
	std::deque<std::string> args(argv + 1, argv + argc);

	std::optional<std::string> bitmapFilename;
	std::optional<std::string> paletteFilename;
	std::optional<std::string> outputDirectory;
	std::optional<std::string> outputName;
	std::optional<unsigned int> pitch;
	bool compileToCode = false;

	while (!args.empty())
	{
		const auto originalArg(args.front());
		args.pop_front();

		if (!originalArg.empty() && originalArg[0] == '-')
		{
			auto arg(originalArg);

			arg.erase(arg.begin());
			if (arg.empty() || arg[0] != '-')
			{
				std::cerr << "Unknown option `" << arg << "`\n";
				return EXIT_FAILURE;
			}
			arg.erase(arg.begin());


			const auto argOffset(arg.find('='));
			std::string value;

			if (argOffset != arg.npos)
			{
				value = arg.substr(argOffset + 1);
				arg.resize(argOffset);
			}


			if (arg == "image")
			{
				if (bitmapFilename.has_value())
				{
					std::cerr << "Warning: image file already set to `" << *bitmapFilename << "`\n";
				}
				else if (value.empty())
				{
					std::cerr << "`image` argument cannot be set to an empty value\n";
				}
				else
				{
					bitmapFilename = value;
				}
			}
			else if (arg == "palette")
			{
				if (paletteFilename.has_value())
				{
					std::cerr << "Warning: palette file already set to `" << *paletteFilename << "`\n";
				}
				else if (value.empty())
				{
					std::cerr << "`palette` argument cannot be set to an empty value\n";
				}
				else
				{
					paletteFilename = value;
				}
			}
			else if (arg == "outdir")
			{
				if (outputDirectory.has_value())
				{
					std::cerr << "Warning: output already set to `" << *outputDirectory << "`\n";
				}
				else if (value.empty())
				{
					std::cerr << "`outdir` argument cannot be set to an empty value\n";
				}
				else
				{
					outputDirectory = value;
				}
			}
			else if (arg == "outname")
			{
				if (outputName.has_value())
				{
					std::cerr << "Warning: output name already set to `" << *outputName << "`\n";
				}
				else if (value.empty())
				{
					std::cerr << "`outname` argument cannot be set to an empty value\n";
				}
				else
				{
					outputName = value;
				}
			}
			else if (arg == "pitch")
			{
				if (pitch.has_value())
				{
					std::cerr << "Warning: pitch already set to `" << *pitch << "`\n";
				}
				else if (value.empty())
				{
					std::cerr << "`pitch` argument cannot be set to an empty value\n";
				}
				else
				{
					pitch = std::stoul(value);
				}
			}
			else
			{
				std::cerr << "Unknown argument `" << originalArg << "`\n";
				return EXIT_FAILURE;
			}
		}

		else
		{
			std::cerr << "Unknown argument `" << originalArg << "`\n";
			return EXIT_FAILURE;
		}
	}

	if (!bitmapFilename.has_value())
	{
		std::cerr << "Bitmap file not set\n";
		return EXIT_FAILURE;
	}

	if (!outputDirectory.has_value())
	{
		std::cerr << "Output directory not set\n";
		return EXIT_FAILURE;
	}

	if (!outputName.has_value())
	{
		std::cerr << "Output name not set\n";
		return EXIT_FAILURE;
	}

	if (!pitch.has_value())
	{
		std::cerr << "WARNING: Pitch not set. Defaulting to 256\n";
		pitch = 256;
	}


	const auto imageFileExtension(KAOS::Common::GetFileExtension(*bitmapFilename));
	std::optional<KAOS::Imaging::Palette> palette;
	std::optional<KAOS::Imaging::Image> image;
	if (imageFileExtension == ".png")
	{
		if (paletteFilename.has_value())
		{
			palette = KAOS::Imaging::LoadGimpPalette(*paletteFilename, 16);
			if (!palette)
			{
				return EXIT_FAILURE;
			}

		}
		if (!palette.has_value())
		{
			palette = KAOS::Imaging::Palette();
		}
		auto loadedImage(KAOS::Imaging::LoadTiledPNGImage(*bitmapFilename, 8, 8, 1, 1, 1, 1, *palette));
		if (loadedImage.has_value())
		{
			image = std::move(loadedImage->first);
			palette = std::move(loadedImage->second);
		}
	}
	else if (imageFileExtension == ".raw" || imageFileExtension == ".data")
	{
		if (!paletteFilename.has_value())
		{
			std::cerr << "Loading raw images requires specifying a palette but no palette file was specified.\n";
			return EXIT_FAILURE;
		}

		palette = KAOS::Imaging::LoadRawRGBPalette(*paletteFilename, 16);
		if (!palette.has_value())
		{
			return EXIT_FAILURE;
		}

		image = KAOS::Imaging::LoadRawImage(*bitmapFilename, 256, 256);
	}

	if (!palette.has_value())
	{
		return EXIT_FAILURE;
	}

	if (palette->size() > 16)
	{
		std::cerr << "Palette file `" << *paletteFilename << "` has" << palette->size() << " colors. Expected 16 or less.\n";
		return EXIT_FAILURE;
	}

	if (!image.has_value())
	{
		std::cerr << "Unable to load image `" << *bitmapFilename << "`\n";
		return EXIT_FAILURE;
	}


	auto tiles(ExtractTiles(*image, 256));


	std::unique_ptr<Generator> codeGenerator;
	if (compileToCode)
	{
		codeGenerator = std::make_unique<CodeGenerator>();
	}
	else
	{
		codeGenerator = std::make_unique<DataGenerator>(true);
	}

	if (codeGenerator->CanGenerateAlias())
	{
		ConslidateDuplicates(tiles);
	}

	TilesetBuilder(move(codeGenerator)).Compile(
		tiles,
		*palette,
		*outputDirectory,
		*outputDirectory + "/" + *outputName + ".inc",
		*outputDirectory + "/" + *outputName + ".pal.inc",
		*pitch);
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
