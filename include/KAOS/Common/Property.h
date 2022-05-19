//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#pragma once
#include <KAOS/Imaging/Color.h>
#include <string>
#include <variant>
#include <cmath>

namespace KAOS { namespace Common
{

	class Property
	{
	private:

		enum class IdType
		{
			Boolean,
			Integer,
			Float,
			Color,
			String
		};


	public:

		using id_type = IdType;
		using bool_type = bool;
		using int_type = int64_t;
		using float_type = float_t;
		using color_type = Imaging::Color;
		using string_type = std::string;
		using value_type = std::variant<bool_type, int_type, float_type, color_type, string_type>;


	public:

		Property() = default;
		Property(const bool_type& value);
		Property(const int_type& value);
		Property(const float_type& value);
		Property(const color_type& value);
		Property(string_type value);


		id_type GetType() const;

		bool IsIntegerType() const;
		int_type ToInteger() const;

		bool IsStringType() const;
		string_type ToString() const;

		template<class Type_>
		bool QueryValue(Type_ &valueOut) const;


		void Set(const bool_type& value);
		void Set(const int_type& value);
		void Set(const float_type& value);
		void Set(const color_type& value);
		void Set(string_type value);


	private:

		id_type		m_Type;
		value_type	m_Value;
	};


	template<class Type_>
	inline bool Property::QueryValue(Type_ &valueOut) const
	{
		auto value(std::get_if<Type_>(&m_Value));
		if (!value)
		{
			return false;
		}

		valueOut = *value;

		return true;
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
