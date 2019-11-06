//	VGMTool for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2019, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include <PSG/CommandData.h>



namespace PSG
{

	CommandData::CommandData(container_type data)
		: data_(move(data))
	{}




	size_t CommandData::size() const
	{
		return data_.size();
	}


	const std::vector<uint8_t>& CommandData::GetData() const
	{
		return data_;
	}



	CommandByteData::CommandByteData(uint8_t value)
		: CommandData(std::vector<uint8_t>(1, value))
	{}


	std::unique_ptr<CommandData> CommandByteData::Clone() const
	{
		return std::make_unique<CommandByteData>(*this);
	}




	CommandBytePairData::CommandBytePairData(uint8_t first, uint8_t second)
		: CommandData({ first, second })
	{}


	std::unique_ptr<CommandData> CommandBytePairData::Clone() const
	{
		return std::make_unique<CommandBytePairData>(*this);
	}

}




//	Copyright (c) 2019 Chet Simpson
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
