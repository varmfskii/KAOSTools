//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include "Registers.h"
#include <KAOS/Common/Utilities.h>
#include <sstream>


QRegister::value_type QRegister::GetValue() const
{
	return m_Value.has_value() ? *m_Value : 0;
}


CodeSegment QRegister::GenerateLoad(value_type newQuad)
{
	CodeSegment codeSegment;

	const DRegister::value_type newAccd((newQuad >> 16) & 0xffff);
	const WRegister::value_type newAccw(newQuad & 0xffff);

	if (!m_Value.has_value())
	{
		codeSegment = GenerateRegisterLoad(newQuad, DRegister(newAccd), WRegister(newAccw));
	}
	else if (newQuad != m_Value)
	{
		if (newAccw != m_Accw && newAccd != m_Accd)
		{
			codeSegment = GenerateRegisterLoad(newQuad, DRegister(newAccd), WRegister(newAccw));
		}
		else if (newAccd != m_Accd)
		{
			codeSegment = m_Accd.GenerateLoad(newAccd);
		}
		else if (newAccw != m_Accw)
		{
			codeSegment = m_Accw.GenerateLoad(newAccw);
		}
		else
		{
			throw std::runtime_error("Encountered repeating row where a repeating row should not exist.");
		}
	}

	m_Accw = WRegister(newAccw);
	m_Accd = DRegister(newAccd);
	m_Value = newQuad;

	return codeSegment;
}




CodeSegment QRegister::GenerateRegisterLoad(
	value_type newQuadValue,
	const DRegister& newAccd,
	const WRegister& newAccw)
{
	CodeSegment codeSegment;

	//	Check for special case of using exg to reduce size
	if (m_Accd.HasValue() && m_Accw.HasValue() && newAccd.HasValue() && newAccw.HasValue()
		&& m_Accd.GetValue() == newAccw.GetValue() && m_Accw.GetValue() == newAccd.GetValue())
	{
		codeSegment.Append("exg", m_Accd.GetName() + "," + m_Accw.GetName(), "optimized for size", 5);

		return codeSegment;
	}



	DRegister tmpAccd(m_Accd);
	WRegister tmpAccw(m_Accw);
	const auto accdCode(tmpAccd.GenerateLoad(newAccd.GetValue()));
	const auto accdCycleCount(accdCode.GetCycleCount());
	const auto accwCode(tmpAccw.GenerateLoad(newAccw.GetValue()));
	const auto accwCycleCount(accwCode.GetCycleCount());

	if (accdCycleCount && accwCycleCount && accdCycleCount + accwCycleCount < 5)
	{
		codeSegment = accdCode + accwCode;
	}
	else
	{
		codeSegment.Append("LDQ", "#$" + KAOS::Common::to_hex_string(newQuadValue, 8), 5);
	}

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
