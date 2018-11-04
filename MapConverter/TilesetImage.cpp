//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include "Tileset.h"
#include <iostream>


bool TilesetImage::Parse(const pugi::xml_node& node)
{
	const auto& sourceAttr(node.attribute("source"));
	if (sourceAttr.empty())
	{
		std::cerr << "Tileset Image does not have a source attribute\n";
		return false;
	}

	const auto& widthAttr(node.attribute("width"));
	if (widthAttr.empty())
	{
		std::cerr << "Tileset Image does not have a width attribute\n";
		return false;
	}

	const auto& heightAttr(node.attribute("height"));
	if (heightAttr.empty())
	{
		std::cerr << "Tileset Image does not have a height attribute\n";
		return false;
	}

	std::string source(sourceAttr.as_string());

	m_Source = move(source);
	m_Dimensions = Size(widthAttr.as_uint(), heightAttr.as_uint());

	return true;
}




std::string TilesetImage::GetSource() const
{
	return m_Source;
}


Size TilesetImage::GetDimensions() const
{
	return  m_Dimensions;
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
