//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include "Map.h"
#include "Utils.h"
#include <iostream>



std::string Map::GetFilename() const
{
	return m_Filename;
}


std::string Map::GetName() const
{
	auto name(m_PropertyBag.find("Name"));
	if (!name.has_value())
	{
		name = "<UNNAMED>";
	}
	else if (name->empty())
	{
		name = "<EMPTY>";
	}

	return *name;
}


Size Map::GetSize() const
{
	return m_MapSize;
}


std::optional<PropertyBag::value_type> Map::QueryProperty(const std::string& name) const
{
	return m_PropertyBag.find(name);
}


Map::layer_object_range Map::GetLayers() const
{
	return layer_object_range(m_Layers.begin(), m_Layers.end());
}




bool Map::Load(std::string filepath)
{
	pugi::xml_document docx;

	auto result(docx.load_file(filepath.c_str()));
	if (!result)
	{
		std::cerr << "Unable to open `" << filepath << "`\n";
		return false;
	}


	auto mapNode(docx.child("map"));
	if (mapNode.empty())
	{
		std::cerr << "File does not appear to contain a map\n";
		return false;
	}

	return Parse(mapNode, filepath);
}




bool Map::Parse(const pugi::xml_node& mapNode, const std::string& filepath)
{
	const auto mapSize(ParseMapSize(mapNode));
	if (!mapSize.has_value())
	{
		return false;
	}

	const auto tileSize(ParseTileSize(mapNode));
	if (!tileSize.has_value())
	{
		return false;
	}

	const auto orientation(ParseOrientation(mapNode));
	if(!orientation.has_value())
	{
		return false;
	}

	const auto renderOrder(ParseRenderOrder(mapNode));
	if(!renderOrder.has_value())
	{
		return false;
	}

	std::optional<Stagger> stagger;
	if (*orientation == Orientation::Staggered || *orientation == Orientation::Hexagonal)
	{
		stagger = ParseStagger(mapNode);
		if (!stagger.has_value())
		{
			return false;
		}
	}

	layer_container_type layers;
	PropertyBag propertyBag;
	tileset_container_type tilesetReferences;

	if (!ParseChildren(mapNode, layers, tilesetReferences, propertyBag))
	{
		return false;
	}

	auto absoluteFilePath(EnsureAbsolutePath(filepath));
	auto absoluteDirectory(GetDirectoryFromFilePath(absoluteFilePath));
	auto filename(GetFilenameFromPath(filepath, true));

	//	Done so we set everything and return success!
	m_Filepath = move(absoluteFilePath);
	m_Filename = move(filename);
	m_Directory = move(absoluteDirectory);
	m_MapSize = *mapSize;
	m_TileSize = *tileSize;
	m_Orientation = *orientation;
	m_RenderOrder = *renderOrder;
	m_StaggerConfig = std::move(*stagger);
	m_PropertyBag = std::move(propertyBag);
	m_Layers = move(layers);
	m_Tilesets = move(tilesetReferences);

	return true;
}




bool Map::ParseChildren(
	const pugi::xml_node& mapNode,
	layer_container_type& layersOut,
	tileset_container_type& tilesetRefsOut,
	PropertyBag& propertyBagOut) const
{
	layer_container_type layers;
	PropertyBag propertyBag;
	tileset_container_type tilesetReferences;

	for (const auto& child : mapNode.children())
	{
		const std::string childName(child.name());

		if (childName == "properties")
		{
			if (!propertyBag.Parse(child))
			{
				return false;
			}
		}
		else if (childName == "tileset")
		{
			TilesetDescriptor tilesetRef;
			if (!tilesetRef.Parse(child))
			{
				return false;
			}

			tilesetReferences.emplace_back(tilesetRef);
		}
		else if (childName == "imagelayer")
		{
			std::cerr << "WARNING: Image layers not supported. Layer ignored.\n";
		}
		else if (childName == "layer")
		{
			auto layer(ParseTilesetLayerNode(child));
			if (!layer)
			{
				return false;
			}

			layers.emplace_back(std::move(layer));
		}
		else if (childName == "group")
		{
			std::cerr << "Groups not supported.\n";
			return false;
		}
		else if (childName == "objectgroup")
		{
			auto objectGroup(ParseObjectGroupNode(child));
			if (!objectGroup)
			{
				return false;
			}

			layers.emplace_back(move(objectGroup));
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


	layersOut = move(layers);
	propertyBagOut = std::move(propertyBag);
	tilesetRefsOut = move(tilesetReferences);


	return true;
}




std::optional<Stagger> Map::ParseStagger(const pugi::xml_node& mapNode) const
{
	Stagger stagger;

	if (!stagger.Parse(mapNode))
	{
		return std::optional<Stagger>();
	}

	return std::move(stagger);
}




std::optional<Size> Map::ParseMapSize(const pugi::xml_node& mapNode) const
{
	Size size;
	if (!size.Parse(mapNode))
	{
		return std::optional<Size>();
	}

	return size;
}


std::optional<Size> Map::ParseTileSize(const pugi::xml_node& mapNode) const
{
	Size size;
	if (!size.Parse(mapNode, "tilewidth", "tileheight"))
	{
		return std::optional<Size>();
	}

	return size;
}




std::optional<Map::Orientation> Map::ParseOrientation(const pugi::xml_node& mapNode) const
{
	const auto orintationAttr(mapNode.attribute("orientation"));
	if (orintationAttr.empty())
	{
		std::cerr << "Missing map orientation attribute.";
		return std::optional<Map::Orientation>();
	}

	std::string orientationStr(orintationAttr.as_string());
	if (orientationStr == "orthogonal")
	{
		return Orientation::Orthogonal;
	}
	else if (orientationStr == "isometric")
	{
		return Orientation::Isometric;
	}
	else if (orientationStr == "staggered")
	{
		return Orientation::Staggered;
	}
	else if (orientationStr == "hexagonal")
	{
		return Orientation::Hexagonal;
	}

	std::cerr << orientationStr << " format maps aren't supported.";
	
	return std::optional<Map::Orientation>();
}




std::optional<Map::RenderOrder> Map::ParseRenderOrder(const pugi::xml_node& mapNode) const
{
	//	Note:	This attribute is optional for older version of map files
	RenderOrder renderOrder(RenderOrder::RightDown);

	const auto renderOrderAttr(mapNode.attribute("renderorder"));
	if (renderOrderAttr.empty())
	{
		std::cerr << "WARNING: Render order attribute missing from map. Defaulting to Right-Down\n";
	}
	else
	{
		const std::string renderOrderStr(renderOrderAttr.as_string());
		if (renderOrderStr == "right-down")
		{
			renderOrder = RenderOrder::RightDown;
		}
		else if (renderOrderStr == "right-up")
		{
			renderOrder = RenderOrder::RightUp;
		}
		else if (renderOrderStr == "left-down")
		{
			renderOrder = RenderOrder::LeftDown;
		}
		else if (renderOrderStr == "left-up")
		{
			renderOrder = RenderOrder::LeftUp;
		}
		else
		{
			std::cerr << renderOrderStr + ": invalid render order. Map not loaded.\n";
			return std::optional<Map::RenderOrder>();
		}
	}

	return renderOrder;
}




std::unique_ptr<TilesetLayer> Map::ParseTilesetLayerNode(const pugi::xml_node& layerNode) const
{
	auto layer(std::make_unique<TilesetLayer>());
	if (!layer->Parse(layerNode))
	{
		return nullptr;
	}

	return move(layer);
}




std::unique_ptr<ObjectGroupLayer> Map::ParseObjectGroupNode(const pugi::xml_node& objectGroupNode) const
{
	auto objectGroup(std::make_unique<ObjectGroupLayer>());
	if (!objectGroup->Parse(objectGroupNode))
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
