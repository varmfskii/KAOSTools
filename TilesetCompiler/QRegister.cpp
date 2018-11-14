//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include "Registers.h"
#include <KAOS/Common/Utilities.h>
#include <sstream>


std::string QRegister::GenerateLoad(value_type newQuad)
{
	MeasuredCode code;

	const WordAccRegister::value_type newAccd((newQuad >> 16) & 0xffff);
	const WordAccRegister::value_type newAccw(newQuad & 0xffff);

	if (!m_Value.has_value())
	{
		code = MeasuredCode("\tLDQ\t#$" + KAOS::Common::to_hex_string(newQuad, 8), 5);
	}
	else if (newQuad != m_Value)
	{
		if (newAccw != m_Accw && newAccd != m_Accd)
		{
			code = GenerateRegisterLoad(newQuad, DRegister(newAccd), WRegister(newAccw));
		}
		else if (newAccd != m_Accd)
		{
			code = m_Accd.GenerateLoad(newAccd);
		}
		else if (newAccw != m_Accw)
		{
			code = m_Accw.GenerateLoad(newAccw);
		}
		else
		{
			throw std::runtime_error("Encountered repeating row where a repeating row should not exist.");
		}
	}

	m_Accw = WRegister(newAccw);
	m_Accd = DRegister(newAccd);
	m_Value = newQuad;

	return code.GetCode();
}




MeasuredCode QRegister::GenerateRegisterLoad(
	value_type newQuadValue,
	const DRegister& newAccd,
	const WRegister& newAccw)
{
	MeasuredCode code;

	//	Check for same outer bytes but different inner bytes
	if (newAccd.GetHiByte() == m_Accd.GetHiByte() && newAccw.GetLoByte() == m_Accw.GetLoByte())
	{
		DRegister tmpAccd(m_Accd);
		WRegister tmpAccw(m_Accw);
		const auto accdCode(tmpAccd.GenerateLoad(newAccd.GetValue()));
		const auto accdCycleCount(accdCode.GetCycleCount());
		const auto accwCode(tmpAccw.GenerateLoad(newAccw.GetValue()));
		const auto accwCycleCount(accwCode.GetCycleCount());

		if (accdCycleCount && accwCycleCount && accdCycleCount + accwCycleCount < 5)
		{
			code = accdCode + accwCode;
		}
		else
		{
			code = MeasuredCode("\tLDQ\t#$" + KAOS::Common::to_hex_string(newQuadValue, 8) + " * TODO: Optimize inner bytes only", 5);
		}
	}
	//	Check for same inner bytes but different outer bytes
	else if (newAccd.GetLoByte() == m_Accd.GetLoByte() && newAccw.GetHiByte() == m_Accw.GetHiByte())
	{
		code = MeasuredCode("\tLDQ\t#$" + KAOS::Common::to_hex_string(newQuadValue, 8) + " * TODO: Optimize outer bytes only", 5);
	}
	//	Load whole register
	else
	{
		code = MeasuredCode("\tLDQ\t#$" + KAOS::Common::to_hex_string(newQuadValue, 8), 5);
	}


	return code;
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
