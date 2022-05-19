//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#pragma once
#include <KAOS/Imaging/Color.h>
#include <KAOS/Imaging/Palette.h>
#include <vector>
#include <memory>


namespace KAOS { namespace Imaging
{

	class ColorImage
	{
	public:

		using row_type = std::vector<Color>;
		using row_list_type = std::vector<row_type>;


	public:

		ColorImage() = default;
		ColorImage(size_t width, size_t height, row_list_type rows);

		bool operator==(const ColorImage& other) const;

		size_t GetWidth() const;
		size_t GetHeight() const;
		const row_list_type& GetRows() const;
		Palette GeneratePalette() const;

		std::shared_ptr<ColorImage> Extract(size_t xPosition, size_t yPosition, size_t width, size_t height) const;


	protected:

		size_t			m_Width;	//	FIXME: Superfluous, can be pulled from first element of m_Rows
		size_t			m_Height;	//	FIXME: Superfluous, can be pulled from size of m_Rows
		row_list_type	m_Rows;
	};


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
