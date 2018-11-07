//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#pragma once
#include <vector>
#include <string>
#include <optional>


namespace KAOS { namespace Common
{

	std::vector<std::string> SplitString(const std::string& text, const std::string& delim);
	std::vector<unsigned int> ConvertToInteger(const std::vector<std::string>& values);
	std::string TrimString(std::string str);
	std::string MakePath(const std::string& path, const std::string& filename);
	std::string ConvertToForwardSlashes(std::string filePath);
	std::string GetFilenameFromPath(std::string path, bool includeExtension);
	bool IsAbsolutePath(const std::string& path);
	std::string EnsureAbsolutePath(const std::string& relativePath);
	std::string GetDirectoryFromFilePath(std::string path);
	std::string GetAbsolutePathFromFilePath(const std::string& filePath);
	std::optional<std::string> GetRelativePathFromFilePath(
		const std::string& fromPath,
		bool fromIsDirectory,
		const std::string& toPath,
		bool toIsDirectory);
	bool CreateDirectory(const std::string& path);
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
