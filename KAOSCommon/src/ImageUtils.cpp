//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include <KAOS/Imaging/ImageUtils.h>
#include <KAOS/Imaging/Palette.h>
#include <KAOS/Common/Utilities.h>
#include <loadpng/lodepng.h>
#include <fstream>
#include <iostream>
#include <sstream>


namespace KAOS { namespace Imaging
{

	namespace
	{
		unsigned char GetColorBits(unsigned channelValue)
		{
			if (channelValue < 43)
			{
				return 0x00;
			}

			if (channelValue < 128)
			{
				return 0x01;
			}

			if (channelValue < 192)
			{
				return 0x08;
			}

			return 0x09;
		}

	}


	uint8_t ConvertColorToRGBRGB(const KAOS::Imaging::Color& color) 
	{
		return (GetColorBits(color.red) << 2) | (GetColorBits(color.green) << 1) | GetColorBits(color.blue);
	}


	uint32_t MergePixels(uint32_t msb, uint32_t lsb)
	{
		return (msb & 0x0f) << 4 | (lsb & 0xff);
	}


	std::optional<Color> ColorFromString(std::string str)
	{
		str = Common::TrimString(str);
		while (str.front() == '#')
		{
			str.erase(0, 1);
		}

		if (str.size() == 6 || str.size() == 8)
		{
			uint32_t value;
			std::stringstream(str) >> std::hex >> value;

			const auto r((value >> 16) & 0xff);
			const auto g((value >> 8) & 0xff);
			const auto b(value & 0xff);
			const auto a(str.size() == 8 ? (value >> 24) & 0xff : 0);

			return Color(std::uint8_t(r), std::uint8_t(g), std::uint8_t(b), std::uint8_t(a));
		}

		std::cerr << "Unable to convert `" << str << "` to color\n";

		return std::optional<Color>();
	}


	std::optional<Image> LoadRawImage(const std::string& filename, size_t width, size_t height)
	{
		std::ifstream input(filename);
		if (!input.is_open())
		{
			std::cerr << "Unable to open bitmap file `" << filename << "`\n";
			return std::optional<Image>();
		}


		Image::row_list_type rows(height, Image::row_type(width));
		for (auto& row : rows)
		{
			if (!input.read(reinterpret_cast<char*>(&row.front()), row.size()))
			{
				std::cerr << "Unable to load bitmap file `" << filename << "`\n";
				return std::optional<Image>();
			}
		}

		return Image(width, height, move(rows));
	}


	std::optional<std::pair<Image, Palette>> LoadTiledPNGImage(
		const std::string& filename,
		size_t tileWidth,
		size_t tileHeight,
		size_t horizontalMargin,
		size_t verticalMargin,
		size_t horizontalSpacing,
		size_t verticalSpacing,
		KAOS::Imaging::Palette palette)
	{
		std::vector<unsigned char> rawImage; //the raw pixels
		unsigned width;
		unsigned height;
		const auto error(lodepng::decode(rawImage, width, height, filename));
		if (error)
		{
			std::cerr << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
			return std::optional<std::pair<Image, Palette>>();
		}

		Image::row_list_type imageRows; //the raw pixels
		imageRows.reserve(height);
		auto dataPtr(rawImage.begin());
		advance(dataPtr, verticalMargin * width * 4);
		for (auto imageY = verticalMargin; imageY < height - verticalMargin; imageY += tileWidth + verticalSpacing)
		{
			for (auto tileY = imageY; tileY < imageY + tileHeight; ++tileY)
			{
				auto savedDataPtr(dataPtr);

				Image::row_type row;
				row.reserve(width);

				//
				advance(dataPtr, horizontalMargin * 4);
				for (auto imageX = horizontalMargin; imageX < width - horizontalMargin; imageX += tileWidth + horizontalSpacing)
				{
					for (auto tileX = imageX; tileX < imageX + tileWidth; ++tileX)
					{
						const Color color(dataPtr[0], dataPtr[1], dataPtr[2], dataPtr[3]);	//	FIXME: Check bounds
						advance(dataPtr, 4);

						const auto index(palette.add(color));
						if (index >= 256)
						{
							std::cerr << "Image `" << filename << "` exceeds 256 colors\n";
							return std::optional<std::pair<Image, Palette>>();
						}

						row.emplace_back(static_cast<uint8_t>(index));
					}

					advance(dataPtr, horizontalSpacing * 4);
				}

				dataPtr = savedDataPtr;
				advance(dataPtr, width * 4);

				imageRows.emplace_back(move(row));
			}

			advance(dataPtr, verticalSpacing * width * 4);
		}

		if (imageRows.empty())
		{
			return {};
		}

		width = imageRows[0].size();
		height = imageRows.size();

		return std::make_pair(Image(width, height, move(imageRows)), std::move(palette));
	}


	std::optional<std::pair<Image, Palette>> LoadPNGImage(const std::string& filename, const Palette& palette, size_t transparentSlot)
	{
		std::vector<unsigned char> rawImage; //the raw pixels
		unsigned width;
		unsigned height;
		const auto error(lodepng::decode(rawImage, width, height, filename));
		if (error)
		{
			std::cerr << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
			return std::optional<std::pair<Image, Palette>>();
		}

		Image::row_list_type imageRows; //the raw pixels
		imageRows.reserve(height);
		auto dataPtr(rawImage.begin());
		for (auto y = 0U; y < height; ++y)
		{
			Image::row_type row;
			row.reserve(width);
			for (auto x = 0U; x < width; ++x)
			{
				const Color color(dataPtr[0], dataPtr[1], dataPtr[2], dataPtr[3]);	//	FIXME: Check bounds
				advance(dataPtr, 4);

				std::optional<size_t> index;
				if(color.alpha != 0xff)
				{
					index = transparentSlot;
				}
				else
				{
					index = palette.getIndex(color);

				}
				if (!index.has_value())
				{
					std::cerr << "Image `" << filename << "` contains color not in palette\n";
					return std::optional<std::pair<Image, Palette>>();
				}

				row.emplace_back(static_cast<uint8_t>(*index));
			}

			imageRows.emplace_back(move(row));
		}


		return std::make_pair(Image(width, height, move(imageRows)), std::move(palette));
	}

	std::optional<ColorImage> LoadPNGColorImage(const std::string& filename)
	{
		std::vector<unsigned char> rawImage; //the raw pixels
		unsigned width;
		unsigned height;
		const auto error(lodepng::decode(rawImage, width, height, filename));
		if (error)
		{
			std::cerr << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
			return std::optional<ColorImage>();
		}

		ColorImage::row_list_type imageRows; //the raw pixels
		imageRows.reserve(height);
		auto dataPtr(rawImage.begin());
		for (auto y = 0U; y < height; ++y)
		{
			ColorImage::row_type row;
			row.reserve(width);
			for (auto x = 0U; x < width; ++x)
			{
				row.emplace_back(Color(dataPtr[0], dataPtr[1], dataPtr[2], dataPtr[3]));
				advance(dataPtr, 4);
			}

			imageRows.emplace_back(move(row));
		}


		return ColorImage(width, height, move(imageRows));
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
