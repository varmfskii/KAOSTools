//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#pragma once
#include <KAOS/Imaging/Image.h>
#include <KAOS/Imaging/Palette.h>
#include <vector>


namespace KAOS { namespace Imaging
{

	struct PackedImageRow
	{
	public:

		using row_data_type = std::vector<uint8_t>;
		using value_type = row_data_type::value_type;
		using size_type = row_data_type::size_type;
		using offset_list_type = std::vector<int64_t>;
		using const_iterator = row_data_type::const_iterator;

	public:

		explicit PackedImageRow(row_data_type data);
		PackedImageRow(const PackedImageRow&) = default;
		PackedImageRow(PackedImageRow&&) = default;
		PackedImageRow& operator=(const PackedImageRow&) = default;
		PackedImageRow& operator=(PackedImageRow&&) = default;

		size_type GetWidthInPixels() const;
		size_type GetPackedSize() const;

		uint8_t GetPixelsAsByte(size_type offset) const;
		uint16_t GetPixelsAsWord(size_type offset) const;
		uint32_t GetPixelsAsQuad(size_type offset) const;

		const_iterator begin() const
		{
			return m_Data.begin();
		}

		const_iterator end() const
		{
			return m_Data.end();
		}


		bool operator==(const PackedImageRow& other) const;

		value_type& operator[](size_type index);
		const value_type& operator[](size_type index) const;


	private:

		row_data_type		m_Data;
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
