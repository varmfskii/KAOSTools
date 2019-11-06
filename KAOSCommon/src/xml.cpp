//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include <KAOS/Common/xml.h>
#include <KAOS/Common/Logging.h>


namespace KAOS { namespace Common { namespace XML
{

	namespace
	{

		template<class OutputType_, class ConverterFunctionType_, class DefaultValueType_>
		bool LoadAttributeT(
			const pugi::xml_node& node,
			const std::string& name,
			OutputType_& output,
			ConverterFunctionType_ function,
			DefaultValueType_ defaultValue)
		{
			const auto& attribute(node.attribute(name.c_str()));
			if (attribute.empty())
			{
				KAOS::Logging::MissingAttributeError(node, name);
				return false;
			}

			output = (attribute.*function)(defaultValue);

			return true;
		}


		template<class OutputType_, class ConverterFunctionType_, class DefaultValueType_>
		bool LoadAttributeT(
			const pugi::xml_node& node,
			const std::string& name,
			std::optional<OutputType_>& output,
			ConverterFunctionType_ function,
			DefaultValueType_ defaultValue)
		{
			const auto& attribute(node.attribute(name.c_str()));
			if (!attribute.empty())
			{
				output = (attribute.*function)(defaultValue);
			}
			else
			{
				output.reset();
			}

			return true;
		}

	}


	bool LoadAttribute(const pugi::xml_node& node, const std::string& name, std::string& output)
	{
		return LoadAttributeT(node, name, output, &pugi::xml_attribute::as_string, "");
	}


	bool LoadAttribute(const pugi::xml_node& node, const std::string& name, int64_t& output)
	{
		return LoadAttributeT(node, name, output, &pugi::xml_attribute::as_int, 0);
	}


	bool LoadAttribute(const pugi::xml_node& node, const std::string& name, uint64_t& output)
	{
		return LoadAttributeT(node, name, output, &pugi::xml_attribute::as_uint, 0);
	}


	bool LoadAttribute(const pugi::xml_node& node, const std::string& name, std::optional<uint64_t>& output)
	{
		return LoadAttributeT(node, name, output, &pugi::xml_attribute::as_uint, 0);
	}



}}}


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
