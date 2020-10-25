//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include <KAOS/Imaging/PackedImageRow.h>


namespace KAOS { namespace Imaging
{

	PackedImageRow::PackedImageRow(row_data_type data)
		: m_Data(move(data))
	{}


	bool PackedImageRow::operator==(const PackedImageRow& other) const
	{
		return m_Data == other.m_Data;
	}


	PackedImageRow::value_type& PackedImageRow::operator[](size_type index)
	{
		return m_Data[index];
	}


	const PackedImageRow::value_type& PackedImageRow::operator[](size_type index) const
	{
		return m_Data[index];
	}




	uint8_t PackedImageRow::GetPixelsAsByte(size_type offset) const
	{
		if (offset >= m_Data.size())
		{
			throw std::out_of_range("Cannot retrieve pixels as quad. Offset too large.");
		}

		return m_Data[offset];
	}


	uint16_t PackedImageRow::GetPixelsAsWord(size_type offset) const
	{
		if (offset + 2 > m_Data.size())
		{
			throw std::out_of_range("Cannot retrieve pixels as word. Offset too large.");
		}

		auto ptr(m_Data.begin());
		advance(ptr, offset);

		uint16_t value;
	
		value = uint16_t(*(ptr++)) << 8;
		value |= *ptr;

		return value;
	}


	uint32_t PackedImageRow::GetPixelsAsQuad(size_type offset) const
	{
		if (offset + 4 > m_Data.size())
		{
			throw std::out_of_range("Cannot retrieve pixels as quad. Offset too large.");
		}

		auto ptr(m_Data.begin());
		advance(ptr, offset);

		uint32_t value;

		value = uint32_t(*(ptr++)) << 24;
		value |= uint32_t(*(ptr++)) << 16;
		value |= uint32_t(*(ptr++)) << 8;
		value |= *ptr;

		return value;
	}


	PackedImageRow::size_type PackedImageRow::GetWidthInPixels() const
	{
		return GetPackedSize() * 2;
	}


	PackedImageRow::size_type PackedImageRow::GetPackedSize() const
	{
		return m_Data.size();
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
