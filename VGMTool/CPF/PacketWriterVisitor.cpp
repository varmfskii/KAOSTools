//	VGMTool for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2019, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include <CPF/PacketWriterVisitor.h>
#include <ostream>
#include <iostream>



namespace CFP
{

	PacketWriterVisitor::PacketWriterVisitor(std::ostream& output, const HardwareConfig& hardwareConfig)
		:
		hardwareConfig_(hardwareConfig),
		output_(output)
	{}




	void PacketWriterVisitor::OnPSGPacket(PSG::Packet& packet)
	{
		uint64_t nativeDelay(hardwareConfig_.GetNativeDelayInTimerTicks(packet.GetDelay()));
		if (packet.GetWriteCount() > 12)
		{
			std::cerr << "Excessive writes = " << std::dec << packet.GetWriteCount() << "\n";
		}

		if (nativeDelay >= 1024)
		{
			std::cerr << "Delay value " << std::dec << nativeDelay << " exceeds limit\n";
		}


		uint8_t byteValue;

		//
		//	Word	Delay					(delays are in GIME timer values)
		//	Byte	Number of data elements	(0xff = end of stream)
		//	....	Data elements			(Data elements are bytes or byte pairs depending on the chip)
		//

		//	Write out delay
		auto nativeDelayTotal(nativeDelay);
		if (nativeDelay > 1023)
		{
			nativeDelay = 1023;
		}
		byteValue = (nativeDelay >> 8) & 0xff;
		output_.write(reinterpret_cast<char*>(&byteValue), sizeof(byteValue));

		byteValue = nativeDelay & 0xff;
		output_.write(reinterpret_cast<char*>(&byteValue), sizeof(byteValue));


		//	Erite out data count and data
		byteValue = packet.GetWriteCount();
		output_.write(reinterpret_cast<char*>(&byteValue), sizeof(byteValue));

		for (const auto& datum : packet)
		{
			for (const auto& value : datum->GetData())
			{
				output_.write(reinterpret_cast<const char*>(&value), sizeof(value));
			}
		}


		//	Write out additional delays
		nativeDelayTotal -= nativeDelay;
		while (nativeDelayTotal > 3)
		{
			nativeDelay = nativeDelayTotal > 1023 ? 1023 : nativeDelayTotal;

			//	Write out the delay
			byteValue = (nativeDelay >> 8) & 0xff;
			output_.write(reinterpret_cast<char*>(&byteValue), sizeof(byteValue));

			byteValue = nativeDelay & 0xff;
			output_.write(reinterpret_cast<char*>(&byteValue), sizeof(byteValue));

			nativeDelayTotal -= nativeDelay;

			//	Write our chip write element count (it's zero!)
			byteValue = 0;
			output_.write(reinterpret_cast<char*>(&byteValue), sizeof(byteValue));
		}

		output_.flush();
	}



	void PacketWriterVisitor::Finalize()
	{
		uint8_t byteValue = 0x00;
		output_.write(reinterpret_cast<char*>(&byteValue), sizeof(byteValue));
		output_.write(reinterpret_cast<char*>(&byteValue), sizeof(byteValue));

		byteValue = 0xff;
		output_.write(reinterpret_cast<char*>(&byteValue), sizeof(byteValue));
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
