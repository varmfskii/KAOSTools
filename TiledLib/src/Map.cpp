//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include <Tiled/Map.h>
#include <KAOS/Common/Utilities.h>
#include <KAOS/Imaging/ImageUtils.h>
#include <iostream>
#include <algorithm>


namespace KAOS { namespace Tiled
{

	std::string Map::GetFilePath() const
	{
		return m_Filepath;
	}


	std::string Map::GetFilename() const
	{
		return m_Filename;
	}


	std::string Map::GetDirectory() const
	{
		return m_Directory;
	}


	std::string Map::GetName() const
	{
		auto nameProperty(m_Properties.find("Name"));
		std::string name;
		if (!nameProperty.has_value())
		{
			name = "<UNNAMED>";
		}
		else
		{
			if (!nameProperty->QueryValue(name))
			{
				std::cerr << "Unable to query value for name property.\n";
				return nullptr;
			}
		}

		if (name.empty())
		{
			name = "<EMPTY>";
		}

		return name;
	}


	Size Map::GetDimensions() const
	{
		return m_MapDimensions;
	}


	Size Map::GetTileDimensions() const
	{
		return m_TileDimensions;
	}


	Map::Orientation Map::GetOrientation() const
	{
		return m_Orientation;
	}


	Map::RenderOrder Map::GetRenderOrder() const
	{
		return m_RenderOrder;
	}


	Stagger Map::GetStagger() const
	{
		return m_StaggerConfig;
	}


	std::optional<NamedProperty::int_type> Map::GetRightEdge() const
	{
		auto value(QueryProperty("RightEdge"));
		NamedProperty::int_type rightEdge(0);
		if (!value.has_value() || !value->QueryValue<decltype(rightEdge)>(rightEdge))
		{
			return GetDimensions().GetWidth();
		}

		return rightEdge;
	}


	std::optional<PropertyBag::value_type> Map::QueryProperty(const std::string& name) const
	{
		return m_Properties.find(name);
	}


	std::shared_ptr<ObjectGroupLayer> Map::QueryObjectLayer(const std::string& name) const
	{
		auto layer(std::find_if(
			m_Layers.begin(),
			m_Layers.end(),
			[&name](std::shared_ptr<Layer> layer) -> bool
			{
				return layer->GetName() == name;
			}));
		
		return layer != m_Layers.end() ? std::dynamic_pointer_cast<ObjectGroupLayer>(*layer) : nullptr;
	}


	std::shared_ptr<TilesetLayer> Map::QueryTilesetLayer(const std::string& name) const
	{
		auto layer(std::find_if(
			m_Layers.begin(),
			m_Layers.end(),
			[&name](std::shared_ptr<Layer> layer) -> bool
			{
				return layer->GetName() == name;
			}));
		
		return layer != m_Layers.end() ? std::dynamic_pointer_cast<TilesetLayer>(*layer) : nullptr;
	}


	std::shared_ptr<TilesetLayer> Map::QueryTilesetLayer(uint64_t index) const
	{
		auto count(index);
		auto layer(std::find_if(
			m_Layers.begin(),
			m_Layers.end(),
			[&count](std::shared_ptr<Layer> layer) -> bool
			{
				if (std::dynamic_pointer_cast<TilesetLayer>(layer))
				{
					if (!count)
					{
						return true;
					}

					--count;
				}

				return false;
			}));
		
		return layer != m_Layers.end() ? std::dynamic_pointer_cast<TilesetLayer>(*layer) : nullptr;
	}


	std::vector<std::shared_ptr<const Layer>> Map::GetLayers() const
	{
		return std::vector<std::shared_ptr<const Layer>>(m_Layers.begin(), m_Layers.end());
	}


	std::vector<TilesetDescriptor> Map::GetTilesets() const
	{
		return m_Tilesets;
	}




	bool Map::Load(const std::string& filepath)
	{
		pugi::xml_document doc;

		auto result(doc.load_file(filepath.c_str()));
		if (!result)
		{
			std::cerr << "Unable to open `" << filepath << "`\n";
			return false;
		}


		auto mapNode(doc.child("map"));
		if (mapNode.empty())
		{
			std::cerr << "File does not appear to contain a map\n";
			return false;
		}

		return Parse(mapNode, filepath);
	}




	bool Map::Parse(const pugi::xml_node& mapNode, const std::string& filepath)
	{
		const auto mapDimensions(ParseMapDimensions(mapNode));
		if (!mapDimensions.has_value())
		{
			return false;
		}

		const auto tileDimensions(ParseTileDimensions(mapNode));
		if (!tileDimensions.has_value())
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

		const auto backgroundColor(ParseBackgroundColor(mapNode));


		auto absoluteFilePath(Common::EnsureAbsolutePath(filepath));
		auto absoluteDirectory(Common::GetDirectoryFromFilePath(absoluteFilePath));
		auto filename(Common::GetFilenameFromPath(filepath, true));

		layer_container_type layers;
		PropertyBag propertyBag;
		tileset_container_type tilesetReferences;

		if (!ParseChildren(mapNode, absoluteDirectory, layers, tilesetReferences, propertyBag))
		{
			return false;
		}

		//	Done so we set everything and return success!
		m_Filepath = move(absoluteFilePath);
		m_Filename = move(filename);
		m_Directory = move(absoluteDirectory);
		m_MapDimensions = *mapDimensions;
		m_TileDimensions = *tileDimensions;
		m_Orientation = *orientation;
		m_RenderOrder = *renderOrder;
		m_StaggerConfig = std::move(*stagger);
		m_BackgroundColor = backgroundColor;
		m_Properties = std::move(propertyBag);
		m_Layers = move(layers);
		m_Tilesets = move(tilesetReferences);

		return true;
	}




	bool Map::ParseChildren(
		const pugi::xml_node& mapNode,
		const std::string& mapDirectory,
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
				if (!tilesetRef.Parse(child, mapDirectory))
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
				std::cerr << "WARNING: Unsupported element `" << childName << "` encountered while parsing Map.\n";
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




	std::optional<Size> Map::ParseMapDimensions(const pugi::xml_node& mapNode) const
	{
		Size dimensions;
		if (!dimensions.Parse(mapNode))
		{
			return std::optional<Size>();
		}

		return dimensions;
	}


	std::optional<Size> Map::ParseTileDimensions(const pugi::xml_node& mapNode) const
	{
		Size dimensions;
		if (!dimensions.Parse(mapNode, "tilewidth", "tileheight"))
		{
			return std::optional<Size>();
		}

		return dimensions;
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




	std::optional<KAOS::Imaging::Color> Map::ParseBackgroundColor(const pugi::xml_node& mapNode) const
	{
		std::optional<KAOS::Imaging::Color> color;
		const auto backgroundColorAttr(mapNode.attribute("backgroundcolor"));
		if (!backgroundColorAttr.empty())
		{
			color = KAOS::Imaging::ColorFromString(backgroundColorAttr.as_string());
		}

		return color;
	}




	std::shared_ptr<TilesetLayer> Map::ParseTilesetLayerNode(const pugi::xml_node& layerNode) const
	{
		auto layer(std::make_shared<TilesetLayer>());
		if (!layer->Parse(layerNode))
		{
			return nullptr;
		}

		return move(layer);
	}




	std::shared_ptr<ObjectGroupLayer> Map::ParseObjectGroupNode(const pugi::xml_node& objectGroupNode) const
	{
		auto objectGroup(std::make_shared<ObjectGroupLayer>());
		if (!objectGroup->Parse(objectGroupNode))
		{
			return nullptr;
		}

		return move(objectGroup);
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
