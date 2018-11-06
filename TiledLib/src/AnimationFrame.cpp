//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include <Tiled/AnimationFrame.h>
#include <iostream>


namespace KAOS { namespace Tiled
{

	bool AnimationFrame::Parse(const pugi::xml_node& node)
	{
		const auto& idAttr(node.attribute("tileid"));
		if (idAttr.empty())
		{
			std::cerr << "Tile animation does not have a tileid attribute\n";
			return false;
		}

		const auto& durationAttr(node.attribute("duration"));
		if (durationAttr.empty())
		{
			std::cerr << "Tile animation does not have a duration attribute\n";
			return false;
		}


		m_TileId = idAttr.as_uint();
		m_Duration = durationAttr.as_uint();

		return true;
	}




	AnimationFrame::id_type AnimationFrame::GetTileId() const
	{
		return m_TileId;
	}


	AnimationFrame::duration_type AnimationFrame::GetDuration() const
	{
		return m_Duration;
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
