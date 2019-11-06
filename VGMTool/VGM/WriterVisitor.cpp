//	VGMTool for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2019, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include <VGM/WriterVisitor.h>
#include <ostream>



namespace VGM
{

	WriterVisitor::WriterVisitor(const CostInfo& costInfo, std::ostream& output)
		:
		costInfo_(costInfo),
		output_(output)
	{}




	void WriterVisitor::OnPSGDelay(double sampleCount)
	{
		uint8_t byteValue = 0x61;
		output_.write(reinterpret_cast<char*>(&byteValue), sizeof(byteValue));

		//	FIXME: We should decrease the sample count by 1
		byteValue = static_cast<uint16_t>(sampleCount) & 0xff;
		output_.write(reinterpret_cast<char*>(&byteValue), sizeof(byteValue));

		byteValue = (static_cast<uint16_t>(sampleCount) >> 8) & 0xff;
		output_.write(reinterpret_cast<char*>(&byteValue), sizeof(byteValue));
	}





	void WriterVisitor::OnPSGWrite(const PSG::CommandData& datum, uint64_t /*sequenceId*/)
	{
		uint8_t byteValue = 0x50;
		output_.write(reinterpret_cast<char*>(&byteValue), sizeof(byteValue));

		for (const auto& value : datum.GetData())
		{
			output_.write(reinterpret_cast<const char*>(&value), sizeof(value));
			output_.flush();
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
