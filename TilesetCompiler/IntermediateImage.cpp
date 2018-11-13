//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include "IntermediateImage.h"


namespace
{
	IntermediateImage::row_list_type GenerateRows(const KAOS::Imaging::Image& image, int64_t renderStride)
	{
		if (image.GetWidth() & 1)
		{
			throw std::invalid_argument("Images must be an even number of pixels wide.");
		}

		const auto& rows(image.GetRows());

		std::vector<IntermediateImageRow> intermediateRows;
		for (auto i(0u); i < rows.size(); ++i)
		{
			const auto& currentRow(rows[i]);
			IntermediateImageRow::row_data_type rowData;
			for (auto pixel(currentRow.begin()); pixel != currentRow.end(); )
			{
				const auto high(*pixel++);
				const auto low(*pixel++);

				if (high >= 16 || low >= 16)
				{
					throw std::runtime_error("Intermediate images only support 4bpp.");
				}

				rowData.push_back(high << 4 | low);
			}

			intermediateRows.emplace_back(rowData, i * renderStride);
		}

		return intermediateRows;
	}
}


IntermediateImage::IntermediateImage(const KAOS::Imaging::Image& image, int64_t renderStride)
	:
	m_Rows(GenerateRows(image, renderStride)),
	m_RenderStride(renderStride)
{}




IntermediateImage::row_list_type::value_type& IntermediateImage::operator[](size_type index)
{
	return m_Rows[index];
}


const IntermediateImage::row_list_type::value_type& IntermediateImage::operator[](size_type index) const
{
	return m_Rows[index];
}




IntermediateImage::size_type IntermediateImage::GetWidth() const
{
	return m_Rows.empty() ? 0 : m_Rows.front().GetWidth();
}


IntermediateImage::size_type IntermediateImage::GetHeight() const
{
	return m_Rows.size();
}


IntermediateImage::size_type IntermediateImage::GetRowSize() const
{
	return m_Rows.empty() ? 0 : m_Rows.front().size();
}


IntermediateImage::size_type IntermediateImage::size() const
{
	return m_Rows.size();
}


IntermediateImage::iterator IntermediateImage::begin()
{
	return m_Rows.begin();
}


IntermediateImage::iterator IntermediateImage::end()
{
	return m_Rows.end();
}


IntermediateImage::const_iterator IntermediateImage::begin() const
{
	return m_Rows.begin();
}


IntermediateImage::const_iterator IntermediateImage::end() const
{
	return m_Rows.end();
}


void IntermediateImage::erase_at(size_type index)
{
	if (index < size())
	{
		auto it(m_Rows.begin());
		advance(it, index);
		m_Rows.erase(it);
	}
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
