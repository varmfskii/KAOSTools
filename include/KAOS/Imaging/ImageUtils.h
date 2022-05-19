//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#pragma once
#include <KAOS/Imaging/Color.h>
#include <KAOS/Imaging/Image.h>
#include <KAOS/Imaging/ColorImage.h>
#include <KAOS/Imaging/Palette.h>
#include <optional>



namespace KAOS { namespace Imaging
{

	std::optional<Color> ColorFromString(std::string str);
	uint8_t ConvertColorToRGBRGB(const KAOS::Imaging::Color& color);
	uint32_t MergePixels(uint32_t msb, uint32_t lsb);

	std::optional<Image> LoadRawImage(const std::string& filename, size_t width, size_t height);
	std::optional<std::pair<Image, Palette>> LoadTiledPNGImage(
		const std::string& filename,
		size_t tileWidth,
		size_t tileHeight,
		size_t horizontalMargin,
		size_t verticalMargin,
		size_t horizontalSpacing,
		size_t verticalSpacing,
		KAOS::Imaging::Palette palette);
	std::optional<std::pair<Image, Palette>> LoadPNGImage(const std::string& filename, const Palette& palette, size_t transparentSlot);
	std::optional<ColorImage> LoadPNGColorImage(const std::string& filename);

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
