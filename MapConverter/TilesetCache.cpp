//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include "TilesetCache.h"
#include "Utils.h"
#include <iostream>


std::optional<TilesetCache::value_type> TilesetCache::Load(const std::string& filepath)
{
	//	FIXME: We should "clean" the path
	if (!IsAbsolutePath(filepath))
	{
		std::cerr << "Retrieving a cached tileset requires an absolute path\n";
		return nullptr;
	}


	auto cachedTileset(m_Cache.find(filepath));
	if (cachedTileset != m_Cache.end())
	{
		return cachedTileset->second;
	}

	auto tileset(std::make_shared<Tileset>());
	if (!tileset->Load(filepath))
	{
		tileset.reset();
	}

	m_Cache.emplace(filepath, tileset);

	return tileset;
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
