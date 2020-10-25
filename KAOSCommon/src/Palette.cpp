//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include <KAOS/Imaging/Palette.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>


namespace KAOS { namespace Imaging
{

	Palette::Palette(size_t colorCount, Color initColor)
		: m_ColorData(colorCount, initColor)
	{}


	Palette::Palette(container_type colors)
		: m_ColorData(move(colors))
	{}




	std::optional<Palette> LoadRawRGBPalette(const std::string& filename, size_t minColors)
	{
		std::ifstream input(filename, std::ifstream::ate | std::ifstream::binary);
		if (!input.is_open())
		{
			std::cerr << "Unable to open palette file `" << filename << "`\n";
			return std::optional<Palette>();
		}

		const auto fileSize(input.tellg());
		input.seekg(0, std::ios_base::beg);
		const auto colorCount(fileSize / 3);
		const auto expectedBytes(colorCount * 3);
		if (expectedBytes != fileSize)
		{
			std::cerr << "WARNING: Ignoring " << (fileSize - expectedBytes) << " extra bytes in palette file `" << filename << "`\n";
		}


		Palette::container_type colorData(colorCount);
		for (auto& color : colorData)
		{
			uint8_t red, green, blue;

			if (!input.read(reinterpret_cast<char*>(&red), sizeof(red)))
			{
				std::cerr << "Error loading red color field\n";
				return std::optional<Palette>();
			}

			if(!input.read(reinterpret_cast<char*>(&green), sizeof(green)))
			{
				std::cerr << "Error loading green color field\n";
				return std::optional<Palette>();
			}

			if(!input.read(reinterpret_cast<char*>(&blue), sizeof(blue)))
			{
				std::cerr << "Error loading blue color field\n";
				return std::optional<Palette>();
			}

			color = Color(red, green, blue);
		}

		if (minColors > colorData.size())
		{
			colorData.resize(minColors);
		}


		return Palette(move(colorData));
	}




	std::optional<Palette> LoadGimpPalette(const std::string& filename, size_t minColors)
	{
		std::ifstream input(filename);
		if (!input.is_open())
		{
			std::cerr << "Unable to open palette file `" << filename << "`\n";
			return std::optional<Palette>();
		}

		std::string line;
		getline(input, line);
		getline(input, line);
		if (!getline(input, line))
		{
			return std::optional<Palette>();
		}
		std::istringstream parser(line);
		std::string scratch;
		size_t count;
		if (!(parser >> scratch >> count))
		{
			return std::optional<Palette>();
		}

		getline(input, line);
		if (line.empty() || line[0] != '#')
		{
			return std::optional<Palette>();
		}

		Palette::container_type colorData(std::max(minColors, count));
		for (auto i(0U); i < count; ++i)
		{

			getline(input, line);

			int red, green, blue;
			char ch;
			int index;
			std::istringstream colorParser(line);
			if(!(colorParser >> red >> green >> blue >> ch >> index))
			{
				return std::optional<Palette>();
			}

			colorData[index] = Color(red, green, blue);;
		}

		return Palette(move(colorData));
	}


}}

/*
bool Palette::SaveAsCode(const std::string& filename)
{
	std::ofstream output(filename);

	if (!output.is_open())
	{
		std::cerr << "Unable to open or create `" << filename << "` for writing\n";
		return false;
	}

	output << "TilesetPalette\tFCB\t";

	for (const auto& color : m_ColorData)
	{
		auto red = GetColorBits(color.red) << 2;
		auto green = GetColorBits(color.green) << 1;
		auto blue = GetColorBits(color.blue);

		auto cocoColor = red | green | blue;


		output << (&color != &m_ColorData.front() ? "," : "") << cocoColor;
	}

	return true;
}
*/




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
