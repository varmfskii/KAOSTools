//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#pragma once
#include "Layer.h"
#include "Size.h"
#include <string>
#include <vector>


namespace KAOS { namespace Tiled
{

	class TilesetLayer : public Layer
	{
	public:

		using cell_type = unsigned int;
		using container_type = std::vector<cell_type>;
		using size_type = container_type::size_type;
		using const_iterator = container_type::const_iterator;


	public:

		TilesetLayer() = default;
		TilesetLayer(const TilesetLayer&) = delete;
		TilesetLayer(TilesetLayer&&) = delete;


		bool Parse(const pugi::xml_node& layer) override;


		Size GetDimensions() const;

		size_type size() const;
		const_iterator begin() const;
		const_iterator end() const;


	protected:

		bool ParseCSV(Size dimensions, const std::string& data);


	private:

		Size			m_Dimensions;
		container_type	m_Data;
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
