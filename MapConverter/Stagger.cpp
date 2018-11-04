//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include "Stagger.h"
#include <iostream>


bool Stagger::Parse(const pugi::xml_node& node)
{
	const auto axisAttr(node.attribute("staggeraxis"));
	if (axisAttr.empty())
	{
		std::cerr << "Map missing staggeraxis attribute\n";
		return false;
	}

	const std::string axisStr(axisAttr.as_string());
	auto axis(Axis::None);
	if (axisStr == "x")
	{
		axis = Axis::X;
	}
	else if (axisStr == "y")
	{
		axis = Axis::Y;
	}

	if (axis == Axis::None)
	{
		std::cerr << "Map uses unsupported stagger axis attribute\n";
		return false;
	}

	const auto indexAttr(node.attribute("staggerindex"));
	auto index(Index::None);
	if (indexAttr.empty())
	{
		std::cerr << "Map missing staggerindex attribute\n";
		return false;
	}

	const std::string indexStr(indexAttr.as_string());
	if (indexStr == "odd")
	{
		index = Index::Odd;
	}
	else if (indexStr == "even")
	{
		index = Index::Even;
	}

	if (index == Index::None)
	{
		std::cerr << "Map uses unsupported stagger index attribute\n";
		return false;
	}

	m_Axis = axis;
	m_Index = index;

	return true;

}




Stagger::Axis Stagger::GetAxis() const
{
	return m_Axis;
}


Stagger::Index Stagger::GetIndex() const
{
	return m_Index;
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
