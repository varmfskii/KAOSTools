// Aseprite FLIC Library
// Copyright (c) 2015 David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#include <FlicLib/flic.h>
#include <istream>


namespace flic
{

	StreamInputInterface::StreamInputInterface(std::istream& input)
		: m_Input(input)
		, m_Ok(true)
	{
	}

	bool StreamInputInterface::ok() const
	{
		return m_Ok;
	}

	size_t StreamInputInterface::tell()
	{
		return m_Input.tellg();
	}

	void StreamInputInterface::seek(size_t absPos)
	{
		m_Input.seekg(absPos);
	}

	uint8_t StreamInputInterface::read8()
	{
		uint8_t value(0);
		if (!m_Input.read(reinterpret_cast<char*>(&value), 1))
		{
			m_Ok = false;
		}

		return value;
	}

}
