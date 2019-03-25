//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include <DefinitionNodes/Defintion.h>
#include <DefinitionNodes/SymbolicValue.h>
#include <DefinitionNodes/Variable.h>


namespace DefinitionNodes
{

	Defintion::Defintion(name_type name)
		: Node(move(name))
	{}




	bool Defintion::AddVariable(name_type name, typeid_type type)
	{
		m_Members.emplace_back(std::make_unique<Variable>(name, type));

		return true;
	}


	bool Defintion::AddSymbolic(name_type name, symbolic_value_type value)
	{
		m_Members.emplace_back(std::make_unique<SymbolicValue>(name, value));

		return true;
	}




	bool Defintion::Generate(formatter_type& formatter) const
	{
		formatter.BeginStruct(GetName());

		for (const auto& member : m_Members)
		{
			if (!member->Generate(formatter))
			{
				return false;
			}
		}

		formatter.EndStruct(GetName());

		return true;
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
