//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#pragma once
#include "DescriptorNodes/CompositeNode.h"


namespace DescriptorNodes
{

	class TilesetDescriptor : public DescriptorNode
	{
	public:

		using string_type = std::string;
		using signature_type = databuilder_type::property_type::word_type;

		static const signature_type Signature = ('T' << 8) | 'D';

		bool Parse(const pugi::xml_node& node) override;


	  //using signature_type = databuilder_type::property_type::word_type;


	protected:

		virtual bool CompileInstance(databuilder_type& builder, const datasource_type& dataSource, const Configuration& configuration) const;
		virtual bool CompileInstanceHeader(databuilder_type& builder, const datasource_type& dataSource, const Configuration& configuration) const;
		virtual bool CompileInstanceFooter(databuilder_type& builder, const datasource_type& dataSource, const Configuration& configuration) const;
		virtual bool CompileInstanceBody(databuilder_type& builder, const datasource_type& dataSource, const Configuration& configuration) const;
		bool CompileDefinition(definitionbuilder_type& builder) const override;


	protected:

		string_type				m_SourceName;
		std::optional<uint64_t>	m_Signature;
	};

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
