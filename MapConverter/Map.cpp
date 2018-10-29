//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2006-2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include "Map.h"
#include "Utils.h"
#include <iostream>


bool Map::Load(std::string filename, unsigned int emptyId)
{
	pugi::xml_document docx;

	auto result(docx.load_file(filename.c_str()));
	if (!result)
	{
		std::cerr << "Unable to open `" << filename << "`\n";
		return false;
	}


	auto mapNode(docx.child("map"));
	if (mapNode.empty())
	{
		std::cerr << "File does not appear to contain a map\n";
		return false;
	}

	const auto& widthAttr(mapNode.attribute("width"));
	if (widthAttr.empty())
	{
		std::cerr << "Layer does not have a width attribute\n";
		return false;
	}

	const auto& heightAttr(mapNode.attribute("height"));
	if (heightAttr.empty())
	{
		std::cerr << "Layer does not have a height attribute\n";
		return false;
	}

	const auto& properties(mapNode.child("properties"));
	if (properties.empty())
	{
		std::cerr << "Layer does not have any custom properties defined.\n";
		return false;
	}

	PropertyBag propertyBag;
	if (!propertyBag.Load(properties))
	{
		return false;
	}

	auto nameProp(propertyBag.get("Name"));
	if (!nameProp.has_value())
	{
		nameProp = "<UNNAMED>";
	}
	else if (nameProp->empty())
	{
		nameProp = "<EMPTY>";
	}

	layer_container_type layers;
	objectgroup_container_type objectGroups;
	if (!LoadBodyNode(mapNode, emptyId, layers, objectGroups))
	{
		return false;
	}


	m_Name = move(*nameProp);
	m_Filename = move(filename);
	m_Width = widthAttr.as_int();	//	FIXME: Convert before assigning
	m_Height = heightAttr.as_int();	//	FIXME: Convert before assigning
	m_PropertyBag = std::move(propertyBag);
	m_TilesetLayers = move(layers);
	m_ObjectGroups = move(objectGroups);

	return true;
}




bool Map::LoadBodyNode(
	const pugi::xml_node& rootNode,
	unsigned int emptyId,
	layer_container_type& layersOut,
	objectgroup_container_type& objectGroupsOut) const
{
	layer_container_type layers;
	objectgroup_container_type objectGroups;

	for (const auto& child : rootNode.children())
	{
		const std::string childName(child.name());

		if (childName == "properties")
		{
			//	DO nothing
		}
		else if (childName == "tileset")
		{
			//	DO nothing
		}
		else if (childName == "layer")
		{
			auto layer(LoadLayerNode(child, emptyId));
			if (!layer)
			{
				return false;
			}

			layers.push_back(std::move(layer));
		}
		else if (childName == "objectgroup")
		{
			auto objectGroup(LoadObjectGroupNode(child));
			if (!objectGroup)
			{
				return false;
			}

			objectGroups.push_back(move(objectGroup));
		}
		else
		{
			std::cerr << "WARNING: Unknown node `" << childName << "` encountered in map file\n";
		}

	}

	if (layers.empty())
	{
		std::cerr << "No layers in map.\n";
		return false;
	}

	if (layers.size() > 1)
	{
		std::cerr << "Multiple layers not supported\n";
		return false;
	}


	layersOut = move(layers);
	objectGroupsOut = move(objectGroups);


	return true;
}




std::unique_ptr<MapLayer> Map::LoadLayerNode(const pugi::xml_node& layerNode, unsigned int emptyId) const
{
	auto layer(std::make_unique<MapLayer>());
	if (!layer->Load(layerNode, emptyId))
	{
		return nullptr;
	}

	return move(layer);
}


std::unique_ptr<ObjectGroup> Map::LoadObjectGroupNode(const pugi::xml_node& objectGroupNode) const
{
	auto objectGroup(std::make_unique<ObjectGroup>());
	if (!objectGroup->Load(objectGroupNode))
	{
		return nullptr;
	}

	return move(objectGroup);
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
