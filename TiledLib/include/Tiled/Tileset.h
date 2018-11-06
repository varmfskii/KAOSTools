//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#pragma once
#include "Tile.h"
#include "TilesetImage.h"
#include "PropertyBag.h"
#include "Size.h"
#include <string>
#include <map>


namespace KAOS { namespace Tiled
{

	class Tileset
	{
	public:

		using tile_collection_type = std::map<size_t, Tile>;


	public:

		bool Load(std::string filepath);

		bool Parse(const pugi::xml_node& node, std::string filepath);

		std::string GetFilePath() const;
		std::string GetFilename() const;
		std::string GetDirectory() const;
		std::string GetName() const;
		Size GetTileDimensions() const;
		size_t GetTileCount() const;
		size_t GetColumns() const;
		size_t GetMargin() const;
		size_t GetSpacing() const;
		TilesetImage GetImage() const;
		tile_collection_type GetTilesetDefinitions() const;
		std::optional<PropertyBag::value_type> QueryProperty(const std::string& name) const;


	protected:

		bool ParseChildren(
			const pugi::xml_node& rootNode,
			tile_collection_type& tileDefinitionsOut,
			TilesetImage& tilesetImageOut,
			PropertyBag& propertyBagOut) const;


	private:

		std::string				m_Filepath;
		std::string				m_Filename;
		std::string				m_Directory;
		std::string				m_Name;
		Size					m_TileDimensions;
		size_t					m_TileCount = 0;
		size_t					m_Columns = 0;
		size_t					m_Margin = 0;
		size_t					m_Spacing = 0;
		TilesetImage			m_TilesetImage;
		tile_collection_type	m_TileDefinitions;
		PropertyBag				m_Properties;
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
