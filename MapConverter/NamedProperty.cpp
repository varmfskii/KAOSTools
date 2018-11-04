//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include "NamedProperty.h"
#include "Utils.h"
#include <iostream>


bool NamedProperty::Parse(const pugi::xml_node& node)
{
	const auto& nameAttr(node.attribute("name"));
	if (nameAttr.empty())
	{
		std::cerr << "Property does not have a name attribute\n";
		return false;
	}

	string_type type(node.attribute("type").as_string());
	if (type.empty())
	{
		type = "string";
	}

	const auto& valueAttr(node.attribute("value"));
	if (valueAttr.empty())
	{
		std::cerr << "Property does not have a value attribute\n";
		return false;
	}
	
	
	value_type value;
	if (type == "bool")
	{
		value = bool_type(valueAttr.as_bool());
	}
	else if (type == "int")
	{
		value = int_type(valueAttr.as_int());
	}
	else if (type == "float")
	{
		value = float_type(valueAttr.as_float());
	}
	else if (type == "string")
	{
		value = string_type(valueAttr.as_string());
	}
	else if (type == "color")
	{
		value = color_type(valueAttr.as_string());
	}
	else
	{
		std::cerr << "Unknown type `" << type << "` for property value type attribute\n";
		return false;
	}

	m_Name = nameAttr.as_string();
	m_Value = move(value);

	return true;
}




std::string NamedProperty::GetName() const
{
	return m_Name;
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
