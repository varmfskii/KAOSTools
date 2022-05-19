//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#pragma once
#include <KAOS/Imaging/Color.h>
#include <vector>
#include <string>
#include <optional>
#include <algorithm>

namespace KAOS { namespace Imaging
{

	class Palette
	{
	public:

		using container_type = std::vector<Color>;
		using value_type = container_type::value_type;
		using iterator = container_type::iterator;
		using const_iterator = container_type::const_iterator;
		using size_type = container_type::size_type;

		Palette() = default;
		explicit Palette(size_t colorCount, Color initColor = Color());
		explicit Palette(container_type colors);
		Palette(const Palette&) = default;
		Palette(Palette&&) = default;

		Palette& operator=(const Palette&) = default;


		size_type add(const Color& color, bool force = false)
		{
			if (!force)
			{
				const auto currentColor(find(m_ColorData.begin(), m_ColorData.end(), color));
				if (currentColor != m_ColorData.end())
				{
					return distance(m_ColorData.begin(), currentColor);
				}
			}

			auto index(m_ColorData.size());
			m_ColorData.emplace_back(color);
			return index;
		}

		std::optional<size_type> getIndex(const Color& color) const
		{
			const auto currentColor(find(m_ColorData.begin(), m_ColorData.end(), color));
			if (currentColor != m_ColorData.end())
			{
				return distance(m_ColorData.begin(), currentColor);
			}

			return std::optional<size_type>();
		}

		container_type::value_type& operator[](size_type index)
		{
			return m_ColorData[index];
		}
		const container_type::value_type& operator[](size_type index) const
		{
			return m_ColorData[index];
		}


		size_type size() const
		{
			return m_ColorData.size();
		}

		iterator begin()
		{
			return m_ColorData.begin();
		}

		const_iterator begin() const
		{
			return m_ColorData.begin();
		}

		iterator end()
		{
			return m_ColorData.end();
		}

		const_iterator end() const
		{
			return m_ColorData.end();
		}


		value_type& front()
		{
			return m_ColorData.front();
		}

		const value_type& front() const
		{
			return m_ColorData.front();
		}


	protected:

		container_type	m_ColorData;
	};


	std::optional<Palette> LoadRawRGBPalette(const std::string& filename, size_t minColors);
	std::optional<Palette> LoadGimpPalette(const std::string& filename, size_t minColors);

	inline Palette&& move(Palette& palette)
	{
		return static_cast<Palette&&>(palette);
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
