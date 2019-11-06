//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include <KAOS/Common/Logging.h>
#include <KAOS/Common/Utilities.h>
#include <fstream>
#include <iostream>
#include <sstream>


namespace KAOS { namespace Logging
{
	bool enabled = true;

	void Warn(const std::string& message)
	{
		if (enabled)
		{
			std::cerr << "WARNING: " << message << "\n";
		}
	}

	void Error(const std::string& message)
	{
		std::cerr << "ERROR: " << message << "\n";
	}

	void NodeError(const pugi::xml_node& node, const std::string& message)
	{
		Error("Node `" + std::string(node.name()) + "` " + message);
	}


	void MissingAttributeError(const pugi::xml_node& node, const std::string& attributeName)
	{
		NodeError(node, "missing `" + attributeName + "` attribute.");
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
