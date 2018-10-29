//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2006-2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#pragma once
#include "MapLayer.h"
#include "ObjectGroup.h"
#include "PropertyBag.h"
#include "pugixml.hpp"
#include <string>
#include <vector>
#include <memory>


class Map
{
public:

	using layer_container_type = std::vector<std::unique_ptr<MapLayer>>;
	using layer_const_iterator = layer_container_type::const_iterator;
	using layer_object_range = pugi::xml_object_range<layer_const_iterator>;
	using objectgroup_container_type = std::vector<std::unique_ptr<ObjectGroup>>;
	using objectgroup_const_iterator = objectgroup_container_type::const_iterator;
	using objectgroup_range = pugi::xml_object_range<objectgroup_const_iterator>;


public:

	Map() = default;
	Map(const Map&) = default;
	Map(Map&&) = default;

	bool Load(std::string filename, unsigned int emptyId);


	std::string GetFilename() const
	{
		return m_Filename;
	}

	std::string GetName() const
	{
		return m_Name;
	}

	unsigned int GetWidth() const
	{
		return m_Width;
	}

	unsigned int GetHeight() const
	{
		return m_Height;
	}

	layer_object_range GetLayers() const
	{
		return layer_object_range(m_TilesetLayers.begin(), m_TilesetLayers.end());
	}

	objectgroup_range GetObjectGroups() const
	{
		return objectgroup_range(m_ObjectGroups.begin(), m_ObjectGroups.end());
	}


protected:

	bool LoadBodyNode(
		const pugi::xml_node& rootNode,
		unsigned int emptyId,
		layer_container_type& layers,
		objectgroup_container_type& objectGroups) const;


	std::unique_ptr<MapLayer> LoadLayerNode(
		const pugi::xml_node& layerNode,
		unsigned int emptyId) const;

	std::unique_ptr<ObjectGroup> LoadObjectGroupNode(const pugi::xml_node& objectGroupNode) const;


private:

	std::string					m_Filename;
	std::string					m_Name;
	unsigned int				m_Width;
	unsigned int				m_Height;
	PropertyBag					m_PropertyBag;
	layer_container_type		m_TilesetLayers;
	objectgroup_container_type	m_ObjectGroups;
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
