//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include <KAOS/Imaging/PackedImage.h>
#include <iostream>



namespace KAOS { namespace Imaging
{

	PackedImage::PackedImage(container_type rows)
		: m_Rows(move(rows))
	{}


	std::optional<std::pair<PackedImage, PackedImage>> PackedImage::CreateFromImage(
		const KAOS::Imaging::ColorImage& image,
		const KAOS::Imaging::Palette& palette)
	{
		std::optional<std::pair<PackedImage, PackedImage>> packedImage;
		if (palette.size() > 16)
		{
			std::cerr << "Palette must 16 colors or less\n";
			return packedImage;
		}

		if (image.GetWidth() & 1)
		{
			std::cerr << "Images must be an even number of pixels wide.\n";
			return packedImage;
		}

		container_type packedBitmapRows;
		container_type packedMaslRows;
		for(const auto& currentRow : image.GetRows())
		{
			PackedImageRow::row_data_type rowData;
			PackedImageRow::row_data_type rowMask;
			for (auto pixel(currentRow.begin()); pixel != currentRow.end(); )
			{
				const auto highColor(*pixel++);
				const auto lowColor(*pixel++);
				uint8_t highPixel, highMask;
				uint8_t lowPixel, lowMask;

				if (highColor.isFullOpaque())
				{
					auto index(palette.getIndex(highColor));
					if (!index.has_value())
					{
						std::cerr << "Color " << highColor.toString() <<" not in palette\n";
						return packedImage;
					}

					highMask = 0x00;
					highPixel = static_cast<uint8_t>(*index) << 4;
				}
				else
				{
					highMask = 0xf0;
					highPixel = 0;
				}


				if (lowColor.isFullOpaque())
				{
					auto index(palette.getIndex(lowColor));
					if (!index.has_value())
					{
						std::cerr << "Color " << lowColor.toString() <<" not in palette\n";
						return packedImage;
					}

					lowMask = 0x00;
					lowPixel = static_cast<uint8_t>(*index);
				}
				else
				{
					lowMask = 0x0f;
					lowPixel = 0;
				}


				rowData.push_back(highPixel | lowPixel);
				rowMask.push_back(highMask | lowMask);
			}

			packedBitmapRows.emplace_back(PackedImageRow(std::move(rowData)));
			packedMaslRows.emplace_back(PackedImageRow(std::move(rowMask)));
		}

		return std::make_pair(PackedImage(move(packedBitmapRows)), PackedImage(move(packedMaslRows)));
	}



	PackedImage::value_type& PackedImage::operator[](size_type index)
	{
		return m_Rows[index];
	}


	const PackedImage::value_type& PackedImage::operator[](size_type index) const
	{
		return m_Rows[index];
	}




	PackedImage::size_type PackedImage::GetWidthInPixels() const
	{
		return m_Rows.empty() ? 0 : m_Rows.front().GetWidthInPixels();
	}


	PackedImage::size_type PackedImage::GetHeightInPixels() const
	{
		return m_Rows.size();
	}


	PackedImage::size_type PackedImage::GetPackedRowSize() const
	{
		return m_Rows.empty() ? 0 : m_Rows.front().GetPackedSize();
	}


	PackedImage::size_type PackedImage::size() const
	{
		return m_Rows.size();
	}


	PackedImage::iterator PackedImage::begin()
	{
		return m_Rows.begin();
	}


	PackedImage::iterator PackedImage::end()
	{
		return m_Rows.end();
	}


	PackedImage::const_iterator PackedImage::begin() const
	{
		return m_Rows.begin();
	}


	PackedImage::const_iterator PackedImage::end() const
	{
		return m_Rows.end();
	}


	void PackedImage::erase_at(size_type index)
	{
		if (index < size())
		{
			auto it(m_Rows.begin());
			advance(it, index);
			m_Rows.erase(it);
		}
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
