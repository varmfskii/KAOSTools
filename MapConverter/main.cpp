//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
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


bool ParseObjectList(const std::string& filename, std::map<std::string, unsigned int>& objectList);
bool GenerateCode(
	std::ostream& output,
	const KAOS::Tiled::Map& map,
	const std::map<std::string, unsigned int>& objectList,
	const unsigned int emptyCellId);


int main(int argc, const char **argv)
{
	std::deque<std::string> args(argv + 1, argv + argc);
	std::string outputDirectory;
	std::optional<unsigned int> emptyId;
	std::optional<std::string> filename;
	std::optional<std::string> objectListFilename;
	std::optional<std::string> explicitOutputFilename;
	bool hasError(false);


	while (!args.empty())
	{
		const auto originalArg(args.front());
		args.pop_front();
		if (originalArg.empty())
		{
			continue;
		}

		if (originalArg[0] == '-')
		{
			auto arg(originalArg);

			arg.erase(arg.begin());
			if (arg.empty() || arg[0] != '-')
			{
				std::cerr << "Unknown option `" << arg << "`\n";
				return EXIT_FAILURE;
			}
			arg.erase(arg.begin());


			const auto argOffset(arg.find('='));
			std::string value;

			if (argOffset != arg.npos)
			{
				value = arg.substr(argOffset + 1);
				arg.resize(argOffset);
			}


			if (arg == "output")
			{
				if (!outputDirectory.empty())
				{
					std::cerr << "Warning: Output directory file already set to `" << outputDirectory << "`\n";
				}
				else
				{
					outputDirectory = value;
				}
			}
			else if (arg == "outputname")
			{
				if (explicitOutputFilename.has_value())
				{
					std::cerr << "Warning: Output filename file already set to `" << *explicitOutputFilename << "`\n";
				}
				else
				{
					explicitOutputFilename = value;
				}
			}
			else if (arg == "objectlist")
			{
				if (objectListFilename.has_value())
				{
					std::cerr << "Warning: Object list file already set to `" << *objectListFilename << "`\n";
				}
				else
				{
					objectListFilename = value;
				}
			}
			else if (arg == "emptyid")
			{
				if (emptyId.has_value())
				{
					std::cerr << "Warning: Empty tile ID already set to `" << emptyId.value() << "`\n";
				}
				else
				{
					emptyId = std::stoul(value);
				}
			}
			else
			{
				std::cerr << "Unknown argument `" << originalArg << "`\n";
				return EXIT_FAILURE;
			}
		}

		else
		{
			if (filename.has_value())
			{
				std::cerr << "Multiple files not supported. File `" << originalArg << " ignored.`\n";
				hasError = true;
			}

			filename = originalArg;
		}
	}


	//	Errors
	if (!filename.has_value())
	{
		std::cerr << "Input filename not set\n";
		hasError = true;
	}

	if (outputDirectory.empty())
	{
		std::cerr << "WARNING: Output directory is not set. Using current working directory.\n";
		outputDirectory = ".";
	}

	if (hasError)
	{
		return EXIT_FAILURE;
	}


	//	Warnings
	if (!emptyId.has_value())
	{
		std::cerr << "Empty tile ID not set. Defaulting to 0\n";
		emptyId = 0;
	}


	std::map<std::string, unsigned int> objectList;
	if (objectListFilename.has_value())
	{
		ParseObjectList(*objectListFilename, objectList);
	}

	KAOS::Tiled::Map map;
	if(!map.Load(*filename))
	{
		return EXIT_FAILURE;
	}

	KAOS::Tiled::TilesetCache tilesetCache;

	for (const auto& tilesetDescriptor : map.GetTilesets())
	{
		if (!tilesetCache.Load(tilesetDescriptor.GetSource()).has_value())
		{

		}
	}

	std::ostringstream outputString;
	if (!GenerateCode(outputString, map, objectList, emptyId.value()))
	{
		return EXIT_FAILURE;
	}

	std::string outputFilename;
	if (explicitOutputFilename.has_value())
	{
		outputFilename = *explicitOutputFilename;
	}
	else
	{
		explicitOutputFilename = KAOS::Common::MakePath(
			outputDirectory,
			KAOS::Common::GetFilenameFromPath(map.GetFilename(), false) + ".asm");
	}

	std::ofstream output(outputFilename);
	if (!output.is_open())
	{
		std::cerr << "Unable to create `" << outputFilename << "`\n";
	}

	output << outputString.str();
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
