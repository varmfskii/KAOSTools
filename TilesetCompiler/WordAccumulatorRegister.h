//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#pragma once
#include "RegisterBase.h"
#include "ByteAccumulatorRegister.h"
#include "CodeSegment.h"
#include <string>
#include <optional>

//	FIXME: Templatize so it can be used with the Q register
class WordAccumulatorRegister : public Register<uint16_t>
{
public:

	using register_id_type = Register::register_id_type;
	using value_type = Register::value_type;
	using subvalue_type = uint8_t;


public:

	using Register::operator!=;
	using Register::operator==;

	CodeSegment GenerateLoad(value_type newWord) override;
	

protected:

	WordAccumulatorRegister(
		const RegisterConfig& config,
		const RegisterConfig& hiRegConfig,
		const RegisterConfig& loRegConfig);

	WordAccumulatorRegister(
		const RegisterConfig& config,
		const RegisterConfig& hiRegTraits,
		const RegisterConfig& loRegConfig,
		value_type value);

	WordAccumulatorRegister& operator=(const WordAccumulatorRegister& other);


private:

	ByteAccumulatorRegister		m_HiByte;
	ByteAccumulatorRegister		m_LoByte;
};




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
