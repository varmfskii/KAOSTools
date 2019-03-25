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


namespace KAOS { namespace Common
{

	class NativeProperty
	{
	private:

		enum class IdType
		{
			Empty,
			Byte,
			Word,
			Quad,
			String
		};

	public:

		using typeid_type = IdType;
		using byte_type = int8_t;
		using word_type = int16_t;
		using quad_type = int32_t;
		using string_type = std::string;
		using value_type = std::variant<byte_type, word_type, quad_type, string_type>;


	public:

		NativeProperty() = default;
		NativeProperty(const byte_type& value);
		NativeProperty(const word_type& value);
		NativeProperty(const quad_type& value);
		NativeProperty(string_type value);


		bool IsEmpty() const
		{
			return GetType() == typeid_type::Empty;
		}

		typeid_type GetType() const;

		byte_type GetByteValue() const;
		word_type GetWordValue() const;
		quad_type GetQuadValue() const;
		string_type GetStringValue() const;
		string_type::size_type GetStringValueLength() const;

		void Set(const byte_type& value);
		void Set(const word_type& value);
		void Set(const quad_type& value);
		void Set(string_type value);


	private:

		typeid_type	m_Type = typeid_type::Empty;
		value_type	m_Value;
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
