//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include "DescriptorNodes/TypedValue.h"
#include "DescriptorNodes/Option.h"


namespace DescriptorNodes
{

	const TypedValue::factory_type& TypedValue::GetExemplars() const
	{
		static const factory_type exemplars
		{
			{ "Option", std::make_unique<Option> }
		};

		return exemplars;
	}


	bool TypedValue::Parse(const pugi::xml_node& node)
	{
		auto query(std::make_unique<PropertyQuery>());
		if (!query->Parse(node))
		{
			return false;
		}


		if (!TypedNode::Parse(node))
		{
			return false;
		}

		m_Query = move(query);

		return true;
	}




	bool TypedValue::CompileInstance(databuilder_type& builder, const datasource_type& dataSource) const
	{
		const auto tileProperty(Resolve(dataSource));
		if (!tileProperty.has_value())
		{
			return false;
		}


		AddValue(builder, GetOutputType(), *tileProperty);

		return true;
	}


	std::optional<KAOS::Tiled::NamedProperty> TypedValue::Resolve(const datasource_type& dataSource) const
	{
		if (!m_Query)
		{
			return std::optional<KAOS::Tiled::NamedProperty>();
		}

		return m_Query->Resolve(dataSource);
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
