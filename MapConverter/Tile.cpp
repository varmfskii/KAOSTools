//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include "Tileset.h"
#include <iostream>


bool Tile::Parse(const pugi::xml_node& node)
{
	const auto& idAttr(node.attribute("id"));
	if (idAttr.empty())
	{
		std::cerr << "Tile does not have an id attribute\n";
		return false;
	}

	const auto& typeAttr(node.attribute("type"));
	const auto& probabilityAttr(node.attribute("probability"));

	frame_container animationFrames;
	PropertyBag propertyBag;
	if (!ParseChildren(node, animationFrames, propertyBag))
	{
		return false;
	}


	std::string type(typeAttr.as_string());

	m_Id = idAttr.as_uint();
	m_Type = move(type);
	m_Probability = probabilityAttr.as_float(1.0f);
	m_AnimationFrames = move(animationFrames);
	m_Properties = std::move(propertyBag);

	return true;
}


bool Tile::ParseChildren(
	const pugi::xml_node& rootNode,
	frame_container& animationFramesOut,
	PropertyBag& propertyBagOut) const
{
	PropertyBag propertyBag;
	frame_container animationFrames;

	for (const auto& child : rootNode.children())
	{
		const std::string childName(child.name());
		
		if (childName == "properties")
		{
			if (!propertyBag.Parse(child))
			{
				return false;
			}
		}
		else if (childName == "animation")
		{
			if (!ParseAnimation(child, animationFrames))
			{
				return false;
			}
		}
		else
		{
			std::cerr << "WARNING: Unsupported element `" << childName << "` encountered while parsing Tile.\n";
		}
	}


	propertyBagOut = std::move(propertyBag);
	animationFramesOut = move(animationFrames);

	return true;
}

bool Tile::ParseAnimation(const pugi::xml_node& rootNode, frame_container& animationFramesOut) const
{
	frame_container animationFrames;

	for (const auto& child : rootNode.children())
	{
		const std::string childName(child.name());

		if (childName == "frame")
		{
			AnimationFrame frame;
			if (!frame.Parse(child))
			{
				return false;
			}

			animationFrames.emplace_back(frame);
		}
		else
		{
			std::cerr << "WARNING: Unsupported element `" << childName << "` encountered while parsing Tile Animation.\n";
		}
	}


	animationFramesOut = move(animationFrames);

	return true;
}


Tile::id_type Tile::GetId() const
{
	return m_Id;
}


std::string Tile::GetType() const
{
	return m_Type;
}


float_t Tile::GetProbability() const
{
	return m_Probability;
}


Tile::frame_container Tile::GetAnimationFrames() const
{
	return m_AnimationFrames;
}


std::optional<PropertyBag::value_type> Tile::QueryProperty(const std::string& name) const
{
	return m_Properties.find(name);
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
