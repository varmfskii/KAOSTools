//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include "Registers.h"
#include "ByteAccumulatorRegister.h"
#include <KAOS/Common/Utilities.h>
#include <sstream>


namespace
{

	struct Constants
	{
		static const unsigned int	FillWidth = 4;
		static const unsigned int	HiShift = 8;
		static const WordAccumulatorRegister::subvalue_type	SubValueMask = 0xff;
	};

}



WordAccumulatorRegister::WordAccumulatorRegister(
	const RegisterConfig& config,
	const RegisterConfig& hiRegConfig,
	const RegisterConfig& loRegConfig)
	:
	Register(config),
	m_HiByte(hiRegConfig),
	m_LoByte(loRegConfig)
{}



WordAccumulatorRegister::WordAccumulatorRegister(
	const RegisterConfig& config,
	const RegisterConfig& hiRegConfig,
	const RegisterConfig& loRegConfig,
	value_type value)
	:
	Register(config, value),
	m_HiByte(hiRegConfig, (value >> Constants::HiShift) & Constants::SubValueMask),
	m_LoByte(loRegConfig, value & Constants::SubValueMask)
{}





WordAccumulatorRegister& WordAccumulatorRegister::operator=(const WordAccumulatorRegister& other)
{
	if (this != &other)
	{
		//	FIXME: should be copy/swap
		Register::operator=(other);
		m_LoByte = other.m_LoByte;
		m_HiByte = other.m_HiByte;
	}

	return *this;
}




CodeSegment WordAccumulatorRegister::GenerateLoad(value_type newWord)
{
	CodeSegment codeSegment;

	const subvalue_type newHi((newWord >> Constants::HiShift) & Constants::SubValueMask);
	const subvalue_type newLo(newWord & Constants::SubValueMask);

	if (!m_Value.has_value())
	{
		codeSegment = Register<uint16_t>::GenerateLoad(newWord);
	}
	else if (newWord != m_Value)
	{
		//	Meh dump this and just perform the loads on each and compare the cycle count and size
		if (newHi != m_HiByte && newLo != m_LoByte)
		{
			codeSegment = Register<uint16_t>::GenerateLoad(newWord);
		}
		else if (newLo != m_LoByte)
		{
			codeSegment = m_LoByte.GenerateLoad(newLo);
		}
		else if (newHi != m_HiByte)
		{
			codeSegment = m_HiByte.GenerateLoad(newHi);
		}
		else
		{
			throw std::runtime_error("Encountered repeating row where a repeating row should not exist.");
		}

		//	We expect a load to have been performed here
		if (codeSegment.GetCycleCount() == 0)
		{
			throw std::runtime_error("Code expected but none generated for lobyte load");
		}
	}

	m_HiByte.LoadValue(newHi);
	m_LoByte.LoadValue(newLo);
	m_Value = newWord;

	return codeSegment;
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
