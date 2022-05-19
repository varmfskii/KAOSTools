//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#pragma once
#include <KAOS/Imaging/PackedImageRow.h>
#include <KAOS/Imaging/ColorImage.h>
#include <KAOS/Imaging/Palette.h>
#include <vector>


namespace KAOS { namespace Imaging
{
	enum class PixelBlitOp
	{
		MixLeft,
		MixRight,
		Fill,
		Skip = 255
	};

	struct PackedImage
	{
	public:

		using PackedRow = PackedImageRow;
		using container_type = std::vector<PackedImageRow>;
		using value_type = container_type::value_type;
		using size_type = container_type::size_type;
		using iterator = container_type::iterator;
		using const_iterator = container_type::const_iterator;


	public:

		value_type& operator[](size_type index);
		const value_type& operator[](size_type index) const;

		static std::optional<std::pair<PackedImage, PackedImage>> CreateFromImage(
			const KAOS::Imaging::ColorImage& image,
			const KAOS::Imaging::Palette& palette);

		size_type GetWidthInPixels() const;
		size_type GetHeightInPixels() const;

		size_type GetPackedRowSize() const;

		size_type size() const;

		iterator begin();
		iterator end();
		const_iterator begin() const;
		const_iterator end() const;

		void erase_at(size_type index);


	protected:

		explicit PackedImage(container_type bitmapRows);


	private:

		container_type	m_Rows;
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
