//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include "Utils.h"
#include <iostream>


std::string TrimString(std::string str)
{
	//	trim leading spaces
	auto endpos = str.find_first_not_of(" \t\n");
	if( std::string::npos != endpos )
	{
		str.erase(0, endpos);
	}

	// trim trailing spaces
	endpos = str.find_last_not_of(" \t\n");
	if( std::string::npos != endpos )
	{
		str = str.substr( 0, endpos+1 );
	}


	return str;
}


std::vector<unsigned int> ConvertToInteger(const std::vector<std::string>& values)
{
	std::vector<unsigned int> intValues;
	for (const auto& stringValue : values)
	{
		intValues.emplace_back(std::stoul(stringValue));
	}

	return intValues;
}


std::vector<std::string> SplitString(const std::string& text, const std::string& delim)
{
	std::vector<std::string> parts;

	size_t start = 0U;
	auto end = text.find(delim);
	while (end != std::string::npos)
	{
		parts.emplace_back(TrimString(text.substr(start, end - start)));
		if (parts.back().empty())
		{
			std::cerr << "Empty value in map!\n";
		}
		start = end + delim.length();
		end = text.find(delim, start);
	}

	parts.emplace_back(TrimString(text.substr(start, end)));

	return parts;
}




std::string MakePath(const std::string& path, const std::string& filename)
{
	std::string fullPath(path);

	if (!fullPath.empty())
	{
		if (fullPath.back() != '/' && fullPath.back() != '\\')
		{
			fullPath += '/';
		}
	}

	return fullPath + filename;
}


std::string GetFilenameFromPath(std::string path, bool includeExtension)
{
	const size_t last_slash_idx = path.find_last_of("\\/");
	if (std::string::npos != last_slash_idx)
	{
		path.erase(0, last_slash_idx + 1);
	}

	if (!includeExtension)
	{
		// Remove extension if present.
		const size_t period_idx = path.rfind('.');
		if (std::string::npos != period_idx)
		{
			path.erase(period_idx);
		}
	}

	return path;
}


std::string GetDirectoryFromFilePath(std::string path)
{
	const size_t last_slash_idx = path.find_last_of("\\/");
	if (std::string::npos != last_slash_idx)
	{
		path.erase(last_slash_idx + 1, path.size());
	}

	return path;
}


std::string GetAbsolutePathFromFilePath(const std::string& filePath)
{
	return GetDirectoryFromFilePath(EnsureAbsolutePath(filePath));
}


std::string EnsureAbsolutePath(const std::string& relativePath)
{
	char buffer[_MAX_PATH + 1];
	if (!_fullpath(buffer, relativePath.c_str(), sizeof(buffer)))
	{
		throw std::runtime_error("Unable to convert relative path to absolute path");
	}

	return buffer;
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
