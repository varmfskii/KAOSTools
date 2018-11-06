//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#pragma once
#include "NamedProperty.h"
#include <string>
#include <vector>
#include <optional>
#include <map>


namespace KAOS { namespace Tiled
{

	class PropertyBag
	{
	public:

		using value_type = NamedProperty;
		using container_type = std::map<std::string, value_type>;
		using const_iterator = container_type::const_iterator;


	public:

		PropertyBag() = default;
		PropertyBag(const PropertyBag&) = default;
		PropertyBag(PropertyBag&&) = default;

		PropertyBag& operator=(PropertyBag&&) = default;


		bool Parse(const pugi::xml_node& rootNode);


		const_iterator begin() const
		{
			return m_Properties.cbegin();
		}

		const_iterator end() const
		{
			return m_Properties.cend();
		}

		std::optional<value_type> find(const std::string& name) const
		{
			const auto property(m_Properties.find(name));

			return property == m_Properties.end() ? std::optional<value_type>() : property->second;
		}


	private:

		container_type	m_Properties;
	};

}}




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
