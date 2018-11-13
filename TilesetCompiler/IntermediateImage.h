//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#pragma once
#include "IntermediateImageRow.h"
#include <KAOS/Imaging/Image.h>
#include <KAOS/Imaging/Palette.h>
#include <vector>


struct IntermediateImage
{
public:

	using row_data_type = IntermediateImageRow;
	using row_list_type = std::vector<row_data_type>;
	using size_type = row_list_type::size_type;
	using iterator = row_list_type::iterator;
	using const_iterator = row_list_type::const_iterator;


public:

	IntermediateImage(const KAOS::Imaging::Image& image, int64_t renderStride);

	row_list_type::value_type& operator[](size_type index);
	const row_list_type::value_type& operator[](size_type index) const;

	size_type GetWidth() const;
	size_type GetHeight() const;
	size_type GetRowSize() const;

	size_type size() const;

	iterator begin();
	iterator end();
	const_iterator begin() const;
	const_iterator end() const;

	void erase_at(size_type index);


private:

	row_list_type	m_Rows;
	int64_t			m_RenderStride;
};




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
