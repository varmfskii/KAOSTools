//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include "Tileset.h"
#include "Utils.h"
#include <iostream>


bool Tileset::Load(std::string filepath)
{
	pugi::xml_document doc;

	auto result(doc.load_file(filepath.c_str()));
	if (!result)
	{
		std::cerr << "Unable to open `" << filepath << "`\n";
		return false;
	}


	auto mapNode(doc.child("tileset"));
	if (mapNode.empty())
	{
		std::cerr << "File does not appear to contain a tileset\n";
		return false;
	}

	return Parse(mapNode, move(filepath));
}



bool Tileset::Parse(const pugi::xml_node& node, std::string filepath)
{
	const auto& nameAttr(node.attribute("name"));
	if (nameAttr.empty())
	{
		std::cerr << "Tileset does not have a name attribute\n";
		return false;
	}

	const auto& tileWidthAttr(node.attribute("tilewidth"));
	if (tileWidthAttr.empty())
	{
		std::cerr << "Tileset does not have a tilewidth attribute\n";
		return false;
	}

	const auto& tileHeightAttr(node.attribute("tileheight"));
	if (tileHeightAttr.empty())
	{
		std::cerr << "Tileset does not have a tileheight attribute\n";
		return false;
	}

	const auto& tileCountAttr(node.attribute("tilecount"));
	if (tileCountAttr.empty())
	{
		std::cerr << "Tileset does not have a tilecount attribute\n";
		return false;
	}

	const auto& columnsAttr(node.attribute("columns"));
	if (columnsAttr.empty())
	{
		std::cerr << "Tileset does not have a columns attribute\n";
		return false;
	}

	const auto& marginAttr(node.attribute("margin"));
	const auto& spacingAttr(node.attribute("spacing"));

	PropertyBag	properties;
	TilesetImage tilesetImage;
	tile_collection_type tileDefinitions;
	if (!ParseChildren(node, tileDefinitions, tilesetImage, properties))
	{
		return false;
	}


	auto absoluteFilePath(EnsureAbsolutePath(filepath));
	auto absoluteDirectory(GetDirectoryFromFilePath(absoluteFilePath));
	auto filename(GetFilenameFromPath(filepath, true));

	m_Filepath = move(absoluteFilePath);
	m_Filename = move(filename);
	m_Directory = move(absoluteDirectory);
	m_Name = nameAttr.as_string();
	m_TileDimensions = Size(tileWidthAttr.as_uint(), tileHeightAttr.as_uint());
	m_TileCount = tileCountAttr.as_uint();
	m_Columns = columnsAttr.as_uint();
	m_Margin = marginAttr.as_uint(0);
	m_Spacing  = spacingAttr.as_uint(0);
	m_Properties = std::move(properties);
	m_TilesetImage = std::move(tilesetImage);


	return true;
}


bool Tileset::ParseChildren(
	const pugi::xml_node& rootNode,
	tile_collection_type& tileDefinitionsOut,
	TilesetImage& tilesetImageOut,
	PropertyBag& propertyBagOut) const
{
	PropertyBag propertyBag;
	TilesetImage tilesetImage;
	tile_collection_type tileDefinitions;

	for (const auto& child : rootNode.children())
	{
		const std::string childName(child.name());

		if (childName == "properties")
		{
			if (!propertyBag.Parse(child))
			{
				return false;
			}
		}
		else if (childName == "image")
		{
			if (!tilesetImage.Parse(child))
			{
				return false;
			}
		}
		else if (childName == "tile")
		{
			Tile tile;
			if (!tile.Parse(child))
			{
				return false;
			}

			tileDefinitions.emplace(tile.GetId(), tile);
		}
		else
		{
			std::cerr << "WARNING: Unsupported element `" << childName << "` encountered while parsing Tileset.\n";
		}
	}


	propertyBagOut = std::move(propertyBag);
	tilesetImageOut = std::move(tilesetImage);
	tileDefinitionsOut = std::move(tileDefinitions);

	return true;
}




std::string Tileset::GetFilePath() const
{
	return m_Filepath;
}


std::string Tileset::GetFilename() const
{
	return m_Filename;
}


std::string Tileset::GetDirectory() const
{
	return m_Directory;
}


std::string Tileset::GetName() const
{
	return m_Name;
}


Size Tileset::GetTileDimensions() const
{
	return m_TileDimensions;
}


size_t Tileset::GetTileCount() const
{
	return m_TileCount;
}


size_t Tileset::GetColumns() const
{
	return m_Columns;
}


size_t Tileset::GetMargin() const
{
	return m_Margin;
}


size_t Tileset::GetSpacing() const
{
	return m_Spacing;
}


TilesetImage Tileset::GetImage() const
{
	return m_TilesetImage;
}


Tileset::tile_collection_type Tileset::GetTilesetDefinitions() const
{
	return m_TileDefinitions;
}


std::optional<PropertyBag::value_type> Tileset::QueryProperty(const std::string& name) const
{
	return m_Properties.find(name);
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
