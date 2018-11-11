#pragma once
#include <vector>
#include <string>
#include <memory>
#include <optional>


namespace KAOS { namespace Imaging
{

	class Image
	{
	public:

		using row_type = std::vector<unsigned char>;
		using row_list_type = std::vector<row_type>;


	public:

		Image() = default;
		Image(size_t width, size_t height, row_list_type rows);

		bool operator==(const Image& other) const;

		//	FIXME: UGH!
		const row_list_type& GetRows() const
		{
			return m_Rows;
		}

		std::shared_ptr<Image> Extract(size_t xPosition, size_t yPosition, size_t width, size_t height) const;


	protected:

		size_t			m_Width;	//	FIXME: Superfluous, can be pulled from first element of m_Rows
		size_t			m_Height;	//	FIXME: Superfluous, can be pulled from size of m_Rows
		row_list_type	m_Rows;
	};

}}

