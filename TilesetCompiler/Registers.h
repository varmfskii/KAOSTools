//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#pragma once
#include <string>
#include <optional>


class MeasuredCode
{
public:

	MeasuredCode() = default;
	MeasuredCode(std::string code, unsigned int cycleCount)
		:
		m_Code(move(code)),
		m_CycleCount(cycleCount)
	{}

	std::string GetCode() const
	{
		return m_Code;
	}

	unsigned int GetCycleCount() const
	{
		return m_CycleCount;
	}

	MeasuredCode operator+(const MeasuredCode& other) const
	{
		return MeasuredCode(m_Code + "\n" + other.m_Code, m_CycleCount + other.m_CycleCount);
	}


private:

	std::string		m_Code;
	unsigned int	m_CycleCount = 0;
};




class WordAccRegister
{
public:

	using value_type = uint16_t;
	using subvalue_type = uint8_t;


public:

	WordAccRegister& operator=(const WordAccRegister& other);

	bool operator!=(const WordAccRegister& other) const;

	//bool operator==(const WordAccRegister& other) const;

	bool HasValue() const;
	value_type GetValue() const;
	subvalue_type GetHiByte() const;
	subvalue_type GetLoByte() const;

	MeasuredCode GenerateLoad(value_type newWord);
	

protected:

	WordAccRegister(
		std::string	fullRegName,
		std::string	hiRegName,
		std::string	loRegName,
		unsigned int cycleCountAdjust,
		bool hasSubShiftInstruction);

	WordAccRegister(
		value_type value,
		std::string	fullRegName,
		std::string	hiRegName,
		std::string	loRegName,
		unsigned int cycleCountAdjust,
		bool hasSubShiftInstruction);


private:

	const std::string			m_FullRegName;
	const std::string			m_HiRegName;
	const std::string			m_LoRegName;
	const unsigned int			m_CycleCountAdjust;
	const bool					m_HasSubShiftInstruction;
	std::optional<value_type>	m_Value;
	subvalue_type				m_LoHalf = 0;
	subvalue_type				m_HiHalf = 0;
};




class WRegister : public WordAccRegister
{
public:

	WRegister();
	explicit WRegister(value_type value);

	using WordAccRegister::operator=;
};


class DRegister : public WordAccRegister
{
public:

	DRegister();
	explicit DRegister(value_type value);

	using WordAccRegister::operator=;
};




class QRegister
{
public:

	using value_type = uint32_t;

	std::string GenerateLoad(value_type newQuad);

protected:

	MeasuredCode GenerateRegisterLoad(
		value_type newQuadValue,
		const DRegister& newAccd,
		const WRegister& newAccw);


private:

	DRegister	m_Accd;
	WRegister	m_Accw;
	std::optional<value_type>	m_Value;
};




inline bool operator!=(const WordAccRegister::value_type& value, const WordAccRegister& other)
{
	return !other.HasValue() || value != other.GetValue();
}


//bool operator==(const WordAccRegister::value_type& value, const WordAccRegister& other);




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
