//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#pragma once
#include "TilesetLayer.h"
#include "ObjectGroupLayer.h"
#include "Stagger.h"
#include "PropertyBag.h"
#include "TilesetDescriptor.h"
#include "Size.h"
#include "pugixml.hpp"
#include <string>
#include <vector>
#include <memory>


class Map
{
public:

	enum class Orientation
	{
		None,
		Orthogonal,
		Isometric,
		Staggered,
		Hexagonal
	};

	enum class RenderOrder
	{
		None,
		RightDown,
		RightUp,
		LeftDown,
		LeftUp
	};


	using layer_container_type = std::vector<std::shared_ptr<Layer>>;
	using layer_const_iterator = layer_container_type::const_iterator;
	using tileset_container_type = std::vector<TilesetDescriptor>;


public:

	Map() = default;
	Map(const Map&) = delete;
	Map(Map&&) = default;


	bool Load(const std::string& filepath);


	std::string GetFilePath() const;
	std::string GetFilename() const;
	std::string GetDirectory() const;
	std::string GetName() const;
	Size GetDimensions() const;
	Size GetTileDimensions() const;
	Orientation GetOrientation() const;
	RenderOrder GetRenderOrder() const;
	Stagger GetStagger() const;

	std::vector<std::shared_ptr<const Layer>> GetLayers() const;
	std::vector<TilesetDescriptor> GetTilesets() const;
	std::optional<PropertyBag::value_type> QueryProperty(const std::string& name) const;


protected:

	bool Parse(const pugi::xml_node& mapNode, const std::string& filepath);

	bool ParseChildren(
		const pugi::xml_node& rootNode,
		const std::string& mapDirectory,
		layer_container_type& layers,
		tileset_container_type& tilesetRefsOut,
		PropertyBag& propertyBag) const;

	std::optional<Size> ParseMapDimensions(const pugi::xml_node& mapNode) const;
	std::optional<Size> ParseTileDimensions(const pugi::xml_node& mapNode) const;
	std::optional<Orientation> ParseOrientation(const pugi::xml_node& mapNode) const;
	std::optional<RenderOrder> ParseRenderOrder(const pugi::xml_node& mapNode) const;
	std::optional<Stagger> ParseStagger(const pugi::xml_node& node) const;
	std::shared_ptr<TilesetLayer> ParseTilesetLayerNode(const pugi::xml_node& layerNode) const;
	std::shared_ptr<ObjectGroupLayer> ParseObjectGroupNode(const pugi::xml_node& objectGroupNode) const;


private:

	std::string				m_Filepath;
	std::string				m_Filename;
	std::string				m_Directory;
	Size					m_MapDimensions;
	Size					m_TileDimensions;
	Orientation				m_Orientation = Orientation::None;
	RenderOrder				m_RenderOrder = RenderOrder::None;
	Stagger					m_StaggerConfig;
	PropertyBag				m_Properties;
	layer_container_type	m_Layers;
	tileset_container_type	m_Tilesets;
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
