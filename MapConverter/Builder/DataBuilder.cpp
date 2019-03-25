//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include "DataBuilder.h"
#include <KAOS/Common/Utilities.h>


namespace Builder
{

	bool DataBuilder::GenerateValue(
		formatter_type& formatter,
		const property_type& value,
		const std::string& symbol) const
	{
		switch (value.GetType())
		{
		case property_type::typeid_type::Byte:
			formatter.EmitByte(std::string(), value.GetByteValue(), symbol);
			break;

		case property_type::typeid_type::Word:
			formatter.EmitWord(std::string(), value.GetWordValue(), symbol);
			break;

		case property_type::typeid_type::Quad:
			formatter.EmitQuad(std::string(), value.GetQuadValue(), symbol);
			break;

		case property_type::typeid_type::String:
			formatter.EmitString(std::string(), value.GetStringValue(), symbol);
			break;

		default:
			throw std::runtime_error("Unknown type.");
		}

		return true;
	}




	void DataStreamBuilder::Reset()
	{
		m_Values.clear();
	}


	bool DataStreamBuilder::Comment(std::string comment)
	{
		m_Values.emplace_back(move(comment), std::optional<property_type>());

		return true;
	}


	bool DataStreamBuilder::AddValue(std::string symbol, property_type value)
	{
		m_Values.emplace_back(move(symbol), value);

		return true;
	}




	bool DataStreamBuilder::Generate(formatter_type& formatter) const
	{
		for (const auto& valuePair : m_Values)
		{
			if (valuePair.second.has_value())
			{
				const auto& value(valuePair.second.value());
				GenerateValue(formatter, value, valuePair.first);
			}
			else
			{
				formatter.Comment(valuePair.first);
			}
		}

		return true;
	}




	void ValueDataBuilder::Reset()
	{
		m_Value = 0;
	}


	bool ValueDataBuilder::Comment(std::string comment)
	{
		return true;
	}


	bool ValueDataBuilder::AddValue(std::string symbol, property_type value)
	{
		switch (value.GetType())
		{
		case property_type::typeid_type::Byte:
			m_Value |= value.GetByteValue();
			break;

		case property_type::typeid_type::Word:
			m_Value |= value.GetWordValue();
			break;

		case property_type::typeid_type::Quad:
			m_Value |= value.GetQuadValue();
			break;

		case property_type::typeid_type::String:
			return false;
		}

		return true;
	}


	uint64_t ValueDataBuilder::GetValue() const
	{
		return m_Value;
	}
	
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
