//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include "Object.h"
#include <memory>
#include <iostream>


bool Object::Parse(const pugi::xml_node& objectNode)
{
	////
	const auto& nameAttr(objectNode.attribute("name"));
	if (nameAttr.empty())
	{
		std::cerr << "WARNING: Object does not have a name attribute\n";
		return false;
	}
	const std::string name(nameAttr.empty() ? std::string() : nameAttr.as_string());


	////
	const auto& typeAttr(objectNode.attribute("type"));
	if (typeAttr.empty())
	{
		std::cerr << "Object group does not have a type attribute\n";
		return false;
	}
	const std::string type(typeAttr.as_string());
	if (type.empty())
	{
		std::cerr << "Object group has an empty type attribute\n";

		return false;
	}

	////
	const auto& xPosAttr(objectNode.attribute("x"));
	if (xPosAttr.empty())
	{
		std::cerr << "Object group does not have a x position attribute\n";
		return false;
	}
	const auto xPos(xPosAttr.as_int());


	////
	const auto& yPosAttr(objectNode.attribute("y"));
	if (yPosAttr.empty())
	{
		std::cerr << "Object group does not have a y position attribute\n";
		return false;
	}
	const auto yPos(yPosAttr.as_int());


	m_Name = move(name);
	m_Type = move(type);
	m_XPos = xPos;
	m_YPos = yPos;

	return true;
}




std::string Object::GetName() const
{
	return m_Name;
}

std::string Object::GetType() const
{
	return m_Type;
}

int Object::GetXPos() const
{
	return m_XPos;
}

int Object::GetYPos() const
{
	return m_YPos;
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
