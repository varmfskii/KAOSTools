//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include "IntermediateImageRow.h"


IntermediateImageRow::IntermediateImageRow(row_data_type data, int64_t offset)
	:
	m_Data(move(data)),
	m_Offsets(1, offset)
{}


bool IntermediateImageRow::ComparePixels(const IntermediateImageRow& other) const
{
	return m_Data == other.m_Data;
}


const IntermediateImageRow::row_data_type& IntermediateImageRow::GetPixels() const
{
	return m_Data;
}


IntermediateImageRow::size_type IntermediateImageRow::GetOffsetCount() const
{
	return m_Offsets.size();
}


const IntermediateImageRow::offset_list_type& IntermediateImageRow::GetOffsets() const
{
	return m_Offsets;
}


void IntermediateImageRow::AddOffsets(const offset_list_type& offsets)
{
	m_Offsets.insert(m_Offsets.end(), offsets.begin(), offsets.end());
}


void IntermediateImageRow::ClearOffsets()
{
	m_Offsets.clear();
}


IntermediateImageRow::size_type IntermediateImageRow::GetWidth() const
{
	return size() * 2;
}


IntermediateImageRow::size_type IntermediateImageRow::size() const
{
	return m_Data.size();
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
