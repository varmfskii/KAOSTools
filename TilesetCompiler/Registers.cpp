//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include "Registers.h"
#include <KAOS/Common/Utilities.h>
#include <sstream>


namespace
{

	struct Constants
	{
		static const unsigned int	FillWidth = 4;
		static const unsigned int	HiShift = 8;
		static const WordAccRegister::subvalue_type	SubValueMask = 0xff;
	};


	template<class ValueType_>
	std::string GenerateRegisterLoad(
		ValueType_ oldValue,
		ValueType_ newValue,
		const std::string& registerName,
		size_t fillWidth)
	{
		std::string output("\t");

		if (newValue == 0)
		{
			output += "CLR" + registerName;
		}
		else if (~oldValue == newValue)
		{
			output += "COM" + registerName;
		}
		else if (oldValue + 1 == newValue)
		{
			output += "INC" + registerName;
		}
		else if (oldValue - 1 == newValue)
		{
			output += "DEC" + registerName;
		}
		else if (oldValue << 1 == newValue)
		{
			output += "LSL" + registerName;
		}
		else if (oldValue >> 1 == newValue)
		{
			output += "LSR" + registerName;
		}
		//	TODO: DAA, neg, sex, sexw
		else
		{
			output += "LD" + registerName + "\t#$" + KAOS::Common::to_hex_string(uint64_t(newValue), fillWidth);
		}

		return output;
	}

}

WordAccRegister::WordAccRegister(
	std::string	fullRegName,
	std::string	hiRegName,
	std::string	loRegName)
	:
	m_FullRegName(fullRegName),
	m_HiRegName(hiRegName),
	m_LoRegName(loRegName),
	m_Value(),
	m_LoHalf(),
	m_HiHalf()
{}



WordAccRegister::WordAccRegister(
	value_type value,
	std::string	fullRegName,
	std::string	hiRegName,
	std::string	loRegName)
	:
	m_FullRegName(fullRegName),
	m_HiRegName(hiRegName),
	m_LoRegName(loRegName),
	m_Value(value),
	m_LoHalf(value & Constants::SubValueMask),
	m_HiHalf((value >> Constants::HiShift) & Constants::SubValueMask)
{}





WordAccRegister& WordAccRegister::operator=(const WordAccRegister& other)
{
	if (this != &other)
	{
		m_Value = other.m_Value;
		m_LoHalf = other.m_LoHalf;
		m_HiHalf = other.m_HiHalf;
	}

	return *this;
}


bool WordAccRegister::operator!=(const WordAccRegister& other) const
{
	return m_Value.has_value() != other.m_Value.has_value() || m_Value != other.m_Value;
}



//bool operator==(const WordAccRegister& other) const
//{
//	if (m_HasValue != other.m_HasValue)
//	{
//		return false;
//	}
//	return m_HasValue == other.m_HasValue && m_Value == other.m_Value;
//}




bool WordAccRegister::HasValue() const
{
	return m_Value.has_value();
}


WordAccRegister::value_type WordAccRegister::GetValue() const
{
	return *m_Value;
}




std::string WordAccRegister::GenerateLoad(value_type newWord)
{
	std::string codeString;

	const subvalue_type newHi((newWord >> Constants::HiShift) & Constants::SubValueMask);
	const subvalue_type newLo(newWord & Constants::SubValueMask);

	if (!m_Value.has_value())
	{
		codeString = "\tLD" + m_FullRegName + "\t#$" + KAOS::Common::to_hex_string(newWord, Constants::FillWidth);
	}
	else if (newWord != m_Value)
	{
		if (newHi != m_HiHalf && newLo != m_LoHalf)
		{
			codeString = ::GenerateRegisterLoad(*m_Value, newWord, m_FullRegName, Constants::FillWidth);
		}
		else if (newLo != m_LoHalf)
		{
			codeString = ::GenerateRegisterLoad(m_LoHalf, newLo, m_LoRegName, Constants::FillWidth / 2);
		}
		else if (newHi != m_HiHalf)
		{
			codeString = ::GenerateRegisterLoad(m_HiHalf, newHi, m_HiRegName, Constants::FillWidth / 2);
		}
		else
		{
			throw std::runtime_error("Encountered repeating row where a repeating row should not exist.");
		}

	}

	m_HiHalf = newHi;
	m_LoHalf = newLo;
	m_Value = newWord;

	return codeString;
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
