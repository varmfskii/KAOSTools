//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include <KAOS/Imaging/ImageUtils.h>
#include <KAOS/Common/Utilities.h>
#include <fstream>
#include <iostream>
#include <sstream>


namespace KAOS { namespace Imaging
{

	std::optional<Color> ColorFromString(std::string str)
	{
		str = Common::TrimString(str);
		while (str.front() == '#')
		{
			str.erase(0, 1);
		}

		if (str.size() == 6 || str.size() == 8)
		{
			uint32_t value;
			std::stringstream(str) >> std::hex >> value;

			const auto r((value >> 16) & 0xff);
			const auto g((value >> 8) & 0xff);
			const auto b(value & 0xff);
			const auto a(str.size() == 8 ? (value >> 24) & 0xff : 0);

			return Color(std::uint8_t(r), std::uint8_t(g), std::uint8_t(b), std::uint8_t(a));
		}

		std::cerr << "Unable to convert `" << str << "` to color\n";

		return std::optional<Color>();
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
