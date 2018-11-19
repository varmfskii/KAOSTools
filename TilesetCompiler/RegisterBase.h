//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#pragma once
#include "CodeSegment.h"
#include <KAOS/Common/Utilities.h>
#include <string>
#include <optional>


enum RegisterId
{
	Zero,
	DP,CC,
	A,B,D,
	E,F,W,
	X,Y,
	S,U,
	PC,
	V
};


std::string GetRegisterName(RegisterId id);


struct RegisterConfig
{
public:

	using register_id_type = RegisterId;


	RegisterConfig(
		register_id_type id,
		unsigned int loadRegisterCycleCount,
		unsigned int mutateCycleCount,
		bool hasSubShiftInstruction)
		:
		m_Id(id),
		m_LoadCycleCount(loadRegisterCycleCount),
		m_MutateCycleCount(mutateCycleCount),
		m_HasShiftInstruction(hasSubShiftInstruction)
	{}

	const register_id_type	m_Id;
	const unsigned int		m_LoadCycleCount;
	const unsigned int		m_MutateCycleCount;
	const bool				m_HasShiftInstruction;
};




template<class ValueType_>
class Register 
{
public:

	using register_id_type = RegisterId;
	using value_type = ValueType_;

	static const unsigned int FillWidth = sizeof(value_type) * 2;


	bool operator!=(const Register& other) const
	{
		return m_Value.has_value() != other.m_Value.has_value() || m_Value != other.m_Value;
	}

	bool operator==(const Register& other) const
	{
		return m_Value.has_value() && other.m_Value.has_value() && m_Value == other.m_Value;
	}


	RegisterId GetId() const
	{
		return m_Config.m_Id;
	}

	std::string GetName() const
	{
		return GetRegisterName(m_Config.m_Id);
	}

	bool HasValue() const
	{
		return m_Value.has_value();
	}

	value_type GetValue() const
	{
		return *m_Value;
	}


	virtual CodeSegment GenerateLoad(value_type newValue)
	{
		CodeSegment codeSegment;

		//	See if we can generate an alternative to the load
		codeSegment = GenerateLoadAlternative(newValue);

		//	No alternative generated so we just load the whole value
		if (!codeSegment.GetCycleCount())
		{
			codeSegment.Append(
				"LD" + GetName(),
				"#$" + KAOS::Common::to_hex_string(uint64_t(newValue), FillWidth),
				m_Config.m_LoadCycleCount);
		}

		m_Value = newValue;

		return codeSegment;
	}


	//	FIXME: Return std::optional<CodeSegment>
	CodeSegment GenerateLoadAlternative(value_type newValue)
	{
		const auto registerName(GetName());
		CodeSegment	codeSegment;

		//	Clearing the register is based only on the new value. The current value is not important
		if (newValue == 0)
		{
			//	a,b,d,e,f,w
			//	1,1,2,2,2,2
			codeSegment.Append("CLR" + registerName, m_Config.m_MutateCycleCount);

			return codeSegment;
		}


		//	If we have a value apply mutation based on difference with new value
		if (m_Value.has_value())
		{
			const auto currentValue(*m_Value);

			if (~currentValue == newValue)
			{
				//	a,b,d,e,f,w
				//	1,1,2,2,2,2
				codeSegment.Append("COM" + registerName, m_Config.m_MutateCycleCount);
			}
			else if (currentValue + 1 == newValue)
			{
				//	a,b,d,e,f,w
				//	1,1,2,2,2,2
				codeSegment.Append("INC" + registerName, m_Config.m_MutateCycleCount);
			}
			else if (currentValue - 1 == newValue)
			{
				//	a,b,d,e,f,w
				//	1,1,2,2,2,2
				codeSegment.Append("DEC" + registerName, m_Config.m_MutateCycleCount);
			}
			//	FIXME: Check for multiple shifts where applicable
			else if (m_Config.m_HasShiftInstruction && currentValue << 1 == newValue)
			{
				//	a,b,d,e,f,w
				//	1,1,2,-,-,-
				codeSegment.Append("LSL" + registerName, m_Config.m_MutateCycleCount);
			}
			//	FIXME: Check for multiple shifts where applicable
			else if (m_Config.m_HasShiftInstruction && currentValue >> 1 == newValue)
			{
				//	a,b,d,e,f,w
				//	1,1,2,-,-,2
				codeSegment.Append("LSR" + registerName, m_Config.m_MutateCycleCount);
			}
			//	FIXME: Validate
			else if (m_Config.m_HasShiftInstruction && -currentValue == newValue)
			{
				//	a,b,d,e,f,w
				//	1,1,2,-,-,-
				codeSegment.Append("NEG" + registerName, m_Config.m_MutateCycleCount);
			}
			else
			{
				//	Load
				//	a,b,d,e,f,w
				//	2,2,3,3,3,4
			}
		}

		return codeSegment;
	}


protected:

	explicit Register(const RegisterConfig& config)
		: m_Config(config)
	{}

	Register(const RegisterConfig& config, value_type value)
		:
		m_Config(config),
		m_Value(value)
	{}



	Register& operator=(const Register& other)
	{
		//	FIXME: should be copy/swap
		if (this != &other)
		{
			m_Value = other.m_Value;
		}

		return *this;
	}


protected:

	const RegisterConfig		m_Config;
	std::optional<value_type>	m_Value;
};




template<class ValueType_>
inline bool operator!=(const typename Register<ValueType_>::value_type& value, const Register<ValueType_>& other)
{
	return !other.HasValue() || value != other.GetValue();
}


//bool operator==(const WordAccumulatorRegister::value_type& value, const WordAccumulatorRegister& other);




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
