//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include <Tiled/NamedProperty.h>
#include <KAOS/Imaging/ImageUtils.h>
#include <iostream>


namespace KAOS { namespace Tiled
{

	NamedProperty::NamedProperty(std::string name, const bool_type& value)
		:
		Property(value),
		m_Name(move(name))
	{}


	NamedProperty::NamedProperty(std::string name, const int_type& value)
		:
		Property(value),
		m_Name(move(name))
	{}


	NamedProperty::NamedProperty(std::string name, const float_type& value)
		:
		Property(value),
		m_Name(move(name))
	{}


	NamedProperty::NamedProperty(std::string name, const color_type& value)
		:
		Property(value),
		m_Name(move(name))
	{}


	NamedProperty::NamedProperty(std::string name, string_type value)
		:
		Property(move(value)),
		m_Name(move(name))
	{}




	bool NamedProperty::Parse(const pugi::xml_node& node)
	{
		const auto& nameAttr(node.attribute("name"));
		if (nameAttr.empty())
		{
			std::cerr << "Property does not have a name attribute\n";
			return false;
		}

		string_type typeString(node.attribute("type").as_string());
		if (typeString.empty())
		{
			typeString = "string";
		}

		const auto& valueAttr(node.attribute("value"));
		if (valueAttr.empty())
		{
			std::cerr << "Property does not have a value attribute\n";
			return false;
		}
	
	
		if (typeString == "bool")
		{
			Set(bool_type(valueAttr.as_bool()));
		}
		else if (typeString == "int")
		{
			Set(int_type(valueAttr.as_int()));
		}
		else if (typeString == "float")
		{
			Set(float_type(valueAttr.as_float()));
		}
		else if (typeString == "string")
		{
			Set(string_type(valueAttr.as_string()));
		}
		else if (typeString == "color")
		{
			auto color(KAOS::Imaging::ColorFromString(valueAttr.as_string()));
			if (!color.has_value())
			{
				std::cerr << "Unable to parse color string `" << valueAttr.as_string() << "`\n";
				return false;
			}

			Set(*color);
		}
		else
		{
			std::cerr << "Unknown type `" << typeString << "` for property value type attribute\n";
			return false;
		}

		m_Name = nameAttr.as_string();

		return true;
	}




	std::string NamedProperty::GetName() const
	{
		return m_Name;
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
