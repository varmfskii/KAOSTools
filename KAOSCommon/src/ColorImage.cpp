//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include <KAOS/Imaging/ColorImage.h>
#include <fstream>
#include <iostream>


namespace KAOS { namespace Imaging
{

	ColorImage::ColorImage(size_t width, size_t height, row_list_type rows)
		:
		m_Width(width),
		m_Height(height),
		m_Rows(move(rows))
	{
	}


	bool ColorImage::operator==(const ColorImage& other) const
	{
		return m_Rows == other.m_Rows;
	}


	size_t ColorImage::GetWidth() const
	{
		return m_Width;
	}

	size_t ColorImage::GetHeight() const
	{
		return m_Height;
	}

	//	FIXME: UGH!
	const ColorImage::row_list_type& ColorImage::GetRows() const
	{
		return m_Rows;
	}

	Palette ColorImage::GeneratePalette() const
	{
		Palette palette;
		for (const auto& row : m_Rows)
		{
			for (const auto& pixel : row)
			{
				palette.add(pixel);
			}
		}

		return move(palette);
	}

	std::shared_ptr<ColorImage> ColorImage::Extract(
		size_t xPosition,
		size_t yPosition,
		size_t width,
		size_t height) const
	{
		//	FIXME: Check bounds!


		row_list_type tileData;
		for (auto y(0U); y < height; ++y)
		{
			const auto& row(m_Rows[yPosition + y]);
			auto start(row.begin());
			advance(start, xPosition);
			auto end(start);
			advance(end, width);

			tileData.emplace_back(start, end);
		}

		return std::make_shared<ColorImage>(width, height, tileData);
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
