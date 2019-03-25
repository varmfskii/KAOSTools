//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include <KAOS/Common/NativeProperty.h>
#include <iostream>


namespace KAOS { namespace Common
{

	namespace
	{
		template<class Type_, class VariantType_>
		inline Type_ QueryValue(const VariantType_ &value)
		{
			auto valuePtr(std::get_if<Type_>(&value));
			if (!valuePtr)
			{
				throw std::runtime_error("Unable to retrieve type. Type mismatch.");
			}

			return std::move(*valuePtr);
		}

	}
	NativeProperty::NativeProperty(const byte_type& value)
		:
		m_Type(typeid_type::Byte),
		m_Value(value)
	{}


	NativeProperty::NativeProperty(const word_type& value)
		:
		m_Type(typeid_type::Word),
		m_Value(value)
	{}


	NativeProperty::NativeProperty(const quad_type& value)
		:
		m_Type(typeid_type::Quad),
		m_Value(value)
	{}


	NativeProperty::NativeProperty(string_type value)
		:
		m_Type(typeid_type::String),
		m_Value(move(value))
	{}




	NativeProperty::typeid_type NativeProperty::GetType() const
	{
		return m_Type;
	}



	NativeProperty::byte_type NativeProperty::GetByteValue() const
	{
		return QueryValue<byte_type>(m_Value);
	}

	NativeProperty::word_type NativeProperty::GetWordValue() const
	{
		return QueryValue<word_type>(m_Value);
	}

	NativeProperty::quad_type NativeProperty::GetQuadValue() const
	{
		return QueryValue<quad_type>(m_Value);
	}

	NativeProperty::string_type NativeProperty::GetStringValue() const
	{
		return QueryValue<string_type>(m_Value);
	}

	NativeProperty::string_type::size_type NativeProperty::GetStringValueLength() const
	{
		return QueryValue<string_type>(m_Value).size();
	}


	void NativeProperty::Set(const byte_type& value)
	{
		m_Value = value;
		m_Type = typeid_type::Byte;
	}

	void NativeProperty::Set(const word_type& value)
	{
		m_Value = value;
		m_Type = typeid_type::Word;
	}

	void NativeProperty::Set(const quad_type& value)
	{
		m_Value = value;
		m_Type = typeid_type::Quad;
	}


	void NativeProperty::Set(string_type value)
	{
		m_Value = move(value);
		m_Type = typeid_type::String;
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
