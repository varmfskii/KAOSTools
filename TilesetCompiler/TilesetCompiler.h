//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#pragma once
#include "CodeGenerator.h"
#include <KAOS/Imaging/Image.h>
#include <KAOS/Imaging/Palette.h>
#include <memory>

class TilemapCompiler
{
public:


	bool Compile(
		const KAOS::Imaging::Image& image,
		const KAOS::Imaging::Palette& palette,
		size_t count,
		const std::string& tileDirectory,
		const std::string& tilesetFilename,
		const std::string& paletteFilename,
		bool optimize,
		size_t displayPitch) const;


protected:

	struct TextureInfo
	{
		size_t						m_TextureId;
		std::optional<size_t>		m_AliasTextureId;
		std::shared_ptr<KAOS::Imaging::Image>	m_Image;

		TextureInfo(std::shared_ptr<KAOS::Imaging::Image> image, size_t textureId)
			:
			m_Image(image),
			m_TextureId(textureId)
		{}

		TextureInfo(size_t textureId)
			:
			m_TextureId(0),
			m_AliasTextureId(textureId)
		{}

		size_t GetId() const
		{
			return m_TextureId;
		}

		size_t GetAliasOrId() const
		{
			return m_AliasTextureId.has_value() ? *m_AliasTextureId : m_TextureId;
		}

		bool HasIdAlias() const
		{
			return m_AliasTextureId.has_value();
		}

		void SetAliasId(size_t newId)
		{
			m_AliasTextureId = newId;
			m_Image = nullptr;
		}

		std::shared_ptr<const KAOS::Imaging::Image> GetImage() const
		{
			return m_Image;
		}

	};

	using TileContainerType = std::vector<TextureInfo>;


	TileContainerType ExtractTextures(const KAOS::Imaging::Image& image, size_t count) const;
	void ConslidateDuplicates(TileContainerType& tileData) const;
	void SaveTiles(const TileContainerType& tileData, const std::string& directory, bool optimize, size_t displayPitch) const;
	void SaveTilemap(const TileContainerType& tileData, const std::string& filename, const std::string& tileDirectory) const;
	bool SavePalette(const std::string& filename, const KAOS::Imaging::Palette& palette) const;

protected:

	CodeGenerator					m_CodeGen;

};




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
