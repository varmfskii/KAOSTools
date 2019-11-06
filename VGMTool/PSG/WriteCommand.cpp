//	VGMTool for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2019, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include <PSG/WriteCommand.h>


namespace PSG
{

	WriteCommand::WriteCommand(std::unique_ptr<CommandData> data)
	{
		datums_.emplace_back(move(data));
	}


	WriteCommand::WriteCommand(uint8_t value)
		: WriteCommand(std::make_unique<CommandByteData>(value))
	{}


	WriteCommand::WriteCommand(uint8_t first, uint8_t second)
		:
		WriteCommand(std::make_unique<CommandBytePairData>(first, second))
	{}


	WriteCommand::WriteCommand(const WriteCommand& other)
		: Command(other)
	{
		container_type datums;

		datums.reserve(other.datums_.size());
		for (auto& datum : other.datums_)
		{
			datums.emplace_back(datum->Clone());
		}

		datums_ = move(datums);
	}




	std::unique_ptr<Command> WriteCommand::Clone() const
	{
		return std::make_unique<WriteCommand>(*this);
	}



	void WriteCommand::Visit(CommandVisitor& visitor)
	{
		for (const auto& value : datums_)
		{
			visitor.OnPSGWrite(*value, 0);
		}
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
