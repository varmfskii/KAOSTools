//	VGMTool for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2019, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#pragma once
#include <PSG/Command.h>
#include <PSG/CommandData.h>
#include <memory>


namespace PSG
{

	class WriteCommand : public Command
	{
	public:

		WriteCommand(const WriteCommand&);
		WriteCommand(std::unique_ptr<CommandData> value);
		WriteCommand(uint8_t value);
		WriteCommand(uint8_t first, uint8_t second);


		std::unique_ptr<Command> Clone() const override;

		void Visit(CommandVisitor& visitor) override;


	protected:

		using container_type = std::vector<std::unique_ptr<CommandData>>;

		container_type	datums_;
	};

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
