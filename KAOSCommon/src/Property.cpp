//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include <KAOS/Common/Property.h>
#include <KAOS/Imaging/ImageUtils.h>
#include <iostream>


namespace KAOS { namespace Common
{

	Property::Property(const bool_type& value)
		:
		m_Type(id_type::Boolean),
		m_Value(value)
	{}


	Property::Property(const int_type& value)
		:
		m_Type(id_type::Integer),
		m_Value(value)
	{}


	Property::Property(const float_type& value)
		:
		m_Type(id_type::Float),
		m_Value(value)
	{}


	Property::Property(const color_type& value)
		:
		m_Type(id_type::Color),
		m_Value(value)
	{}


	Property::Property(string_type value)
		:
		m_Type(id_type::String),
		m_Value(move(value))
	{}




	Property::id_type Property::GetType() const
	{
		return m_Type;
	}


	bool Property::IsIntegerType() const
	{
		switch (GetType())
		{
		case id_type::Integer:
		case id_type::Boolean:
			return true;
		}
			
		return false;
	}


	Property::int_type Property::ToInteger() const
	{
		const auto type(GetType());

		bool hasValue(false);
		int_type value(0);
		if (type == id_type::Integer)
		{
			hasValue = QueryValue<int_type>(value);
		}
		else if(type == id_type::Boolean)
		{
			bool boolValue(false);
			hasValue = QueryValue<bool_type>(boolValue);
			value = boolValue;
		}
			
		if (!hasValue)
		{
			throw std::runtime_error("Cannot convert to integer");
		}

		return value;
	}




	bool Property::IsStringType() const
	{
		return GetType() == id_type::String;
	}

	Property::string_type Property::ToString() const
	{
		const auto type(GetType());

		bool hasValue(false);
		string_type value;
		if (type == id_type::String)
		{
			hasValue = QueryValue<string_type>(value);
		}
		else if (type == id_type::Integer)
		{
			int_type intValue(0);
			hasValue = QueryValue<int_type>(intValue);
			value = std::to_string(intValue);
		}
		else if (type == id_type::Boolean)
		{
			bool boolValue(false);
			hasValue = QueryValue<bool_type>(boolValue);
			value = std::to_string(boolValue);
		}

		if (!hasValue)
		{
			throw std::runtime_error("Cannot convert to integer");
		}

		return value;
	}



	void Property::Set(const bool_type& value)
	{
		m_Value = value;
		m_Type = id_type::Boolean;
	}

	void Property::Set(const int_type& value)
	{
		m_Value = value;
		m_Type = id_type::Integer;
	}

	void Property::Set(const float_type& value)
	{
		m_Value = value;
		m_Type = id_type::Float;
	}

	void Property::Set(const color_type& value)
	{
		m_Value = value;
		m_Type = id_type::Color;
	}

	void Property::Set(string_type value)
	{
		m_Value = move(value);
		m_Type = id_type::String;
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
