//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include <Tiled/ObjectGroup.h>
#include <memory>
#include <iostream>
#include <algorithm>


namespace KAOS { namespace Tiled
{

	namespace
	{
		template<class Type_>
		std::vector<Type_> CloneVector(const std::vector<Type_>& objects)
		{
			std::vector<Type_> clonedObjects;
			transform(
				objects.begin(),
				objects.end(),
				back_inserter(clonedObjects),
				[](const Type_& ptr) -> Type_ { return ptr->Clone(); }
			);

			return move(clonedObjects);
		}
	}


	ObjectGroup::ObjectGroup(const ObjectGroup& other)
		: m_Objects(CloneVector<value_type>(other.m_Objects))
	{

	}

	ObjectGroup& ObjectGroup::operator=(const ObjectGroup& other)
	{
		if (this != &other)
		{
			operator=(ObjectGroup(other));
		}

		return *this;
	}

	bool ObjectGroup::Parse(const pugi::xml_node& objectGroupNode)
	{
		container_type objects;
		for (const auto& objectNode : objectGroupNode.children("object"))
		{
			auto object(std::make_unique<Object>());
			if (!object->Parse(objectNode))
			{
				return false;
			}

			objects.emplace_back(std::move(object));
		}

		m_Objects = move(objects);

		return true;
	}




	ObjectGroup::container_type::size_type ObjectGroup::size() const
	{
		return m_Objects.size();
	}


	ObjectGroup::const_iterator ObjectGroup::begin() const
	{
		return m_Objects.cbegin();
	}


	ObjectGroup::const_iterator ObjectGroup::end() const
	{
		return m_Objects.cend();
	}

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
