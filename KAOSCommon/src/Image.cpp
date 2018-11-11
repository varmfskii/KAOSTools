#include <KAOS/Imaging/Image.h>
#include <fstream>
#include <iostream>


namespace KAOS { namespace Imaging
{

	Image::Image(size_t width, size_t height, row_list_type rows)
		:
		m_Width(width),
		m_Height(height),
		m_Rows(move(rows))
	{
	}


	bool Image::operator==(const Image& other) const
	{
		return m_Rows == other.m_Rows;
	}




	std::shared_ptr<Image> Image::Extract(
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

		return std::make_shared<Image>(width, height, tileData);
	}


}}
