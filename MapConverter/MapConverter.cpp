//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include "DescriptorNodes/Root.h"
#include <Tiled/Map.h>
#include <Tiled/Tileset.h>
#include <Tiled/TilesetCache.h>
#include <KAOS/Common/Utilities.h>
#include <pugixml/pugixml.hpp>
#include <iostream>
#include <deque>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <optional>
#include <functional>

namespace
{

	bool BuildTileDescriptors(
		std::ostream& output,
		Builder::DataStreamBuilder& dataBuilder,
		const KAOS::Tiled::Map& map,
		KAOS::Tiled::TilesetCache& tilesetCache,
		std::shared_ptr<DescriptorNodes::TileDescriptor> tileDescriptor)
	{
		for (const auto& tilesetDescriptor : map.GetTilesets())
		{
			auto tilesetPtr(tilesetCache.Load(tilesetDescriptor.GetSource()));
			if (!tilesetPtr.has_value() || !*tilesetPtr)
			{
				output << "Unable to load tileset from `" << tilesetDescriptor.GetSource() << "`\n";
				return false;
			}

			const auto& tileset(**tilesetPtr);
			std::vector<KAOS::Tiled::Tile> tiles(tileset.GetTileCount());
			for (const auto& tile : tileset.GetTiles())
			{
				tiles[tile.first] = tile.second;
			}

			for (const auto& tile : tiles)
			{
				tileDescriptor->CompileInstance(dataBuilder, Builder::TileDataSource(tile));
			}
		}

		return true;
	}


	bool BuildInstances(
		Builder::DataStreamBuilder& dataBuilder,
		const KAOS::Tiled::Map& map,
		std::shared_ptr<DescriptorNodes::MapFile> mapFileDescriptor,
		std::shared_ptr<DescriptorNodes::Objects> objectDescriptors)
	{
		Builder::MapDataSourceEx<DescriptorNodes::Objects> dataSource(map, *objectDescriptors);

		return mapFileDescriptor->CompileInstance(dataBuilder, dataSource);
	};

}




bool GenerateMapCode(
	std::ostream& output,
	const KAOS::Tiled::Map& map,
	DescriptorNodes::Root& descriptors,
	KAOS::Tiled::TilesetCache tilesetCache,
	const unsigned int /*emptyCellId*/)
{
	const auto tileDescriptor(descriptors.QueryTileDescriptor("TileDescriptor"));
	if (!tileDescriptor)
	{
		return false;
	}

	const auto objectDescriptors(descriptors.QueryObjectsDescriptor(""));
	if (!objectDescriptors)
	{
		return false;
	}

	const auto mapFileDescriptor(descriptors.QueryMapFileDescriptor(""));
	if (!mapFileDescriptor)
	{
		return false;
	}



	//	Generate object instances
	{
		output
			<< ";;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;\n"
			<< ";; Main map chunk (header, objects, etc)\n"
			<< ";;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;\n"
			;

		Builder::DataStreamBuilder objectInstanceDataBuilder;
		if (!BuildInstances(objectInstanceDataBuilder, map, mapFileDescriptor, objectDescriptors))
		{
			return false;
		}

		Builder::AsmFormatter instanceFormatter(output);
		if (!objectInstanceDataBuilder.Generate(instanceFormatter))
		{
			return false;
		}
	}

	{
		output
			<< "\n"
			<< ";;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;\n"
			<< ";; Tileset attributes\n"
			<< ";;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;\n"
			;

		Builder::DataStreamBuilder tileDescriptorDataBuilder;
		BuildTileDescriptors(output, tileDescriptorDataBuilder, map, tilesetCache, tileDescriptor);

		Builder::AsmFormatter tileDescriptorFormatter(output);
		tileDescriptorDataBuilder.Generate(tileDescriptorFormatter);
	}

	return true;
}



bool GenerateDefinitions(KAOS::Common::EventConsole& eventConsole, std::ostream& output, DescriptorNodes::Root& descriptors)
{
	const auto tileDescriptor(descriptors.QueryTileDescriptor("TileDescriptor"));
	if (!tileDescriptor)
	{
		return false;
	}

	const auto objectDescriptors(descriptors.QueryObjectsDescriptor(""));
	if (!objectDescriptors)
	{
		return false;
	}


	//	Generate descriptor definitions
	Builder::DefinitionBuilder tileDescriptorDefinitionBuilder(eventConsole);
	if (!tileDescriptor->CompileDefinition(tileDescriptorDefinitionBuilder))
	{
		return false;
	}

	Builder::DefinitionBuilder objectDescriptorDefinitionBuilder(eventConsole);
	if (!objectDescriptors->CompileDefinition(objectDescriptorDefinitionBuilder))
	{
		return false;
	}

	Builder::AsmFormatter descriptorFormatter(output);
	tileDescriptorDefinitionBuilder.Generate(descriptorFormatter);
	objectDescriptorDefinitionBuilder.Generate(descriptorFormatter);


	return true;
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
