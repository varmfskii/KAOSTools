//	VGMTool for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2019, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include <Visitors/BuildPacketVisitor.h>



BuildPacketVisitor::BuildPacketVisitor(const HardwareConfig& hardwareConfig, const CostInfo& costInfo)
	:
	hardwareConfig_(hardwareConfig),
	costInfo_(costInfo)
{}




void BuildPacketVisitor::CheckFlush()
{
	if (!currentPacket_)
	{
		//	No delay or write, no value to offset the cost. Going into debt!
		return;
	}

	const auto costOfWrite(costInfo_.GetNativeWriteRanking(*currentPacket_));
	const auto valueOfDelay(hardwareConfig_.GetNativeDelayRanking(*currentPacket_));
	const auto nativeDelayInTicks(hardwareConfig_.GetNativeDelayInTimerTicks(*currentPacket_));

	//	If the cost and value are the same, dump both
	if (std::fabs(costOfWrite - valueOfDelay) < std::numeric_limits<double>::epsilon())
	{
		currentPacket_ = nullptr;
	}
	else if (nativeDelayInTicks >= costInfo_.minimumNativeTicks && costOfWrite < valueOfDelay)
	{
		auto savings(valueOfDelay - costOfWrite);
		auto singleSampleRanking(hardwareConfig_.GetNativeDelayRanking(1));	//	Get native ranking for 1 host sample

		if (savings >= singleSampleRanking && savings >= costInfo_.delayBiasCycleCost)
		{
			currentPacket_ = nullptr;
		}
	}
}


void BuildPacketVisitor::OnPSGDelay(double sampleCount)
{
	if (!currentPacket_)
	{
		auto delayCommand(std::make_unique<PSG::Packet>());
		currentPacket_ = delayCommand.get();
		commands_.Write(move(delayCommand));
	}

	currentPacket_->AdjustDelay(sampleCount);

	//CheckFlush();	//	FIXME: We probably shouldn't do this here so we can let delays accumulate
}


void BuildPacketVisitor::OnPSGWrite(const PSG::CommandData& datum, uint64_t /*sequenceId*/)
{
	//	If the current command does not have any data assume it has a delay and flush it
	if (currentPacket_
		&& hardwareConfig_.GetNativeDelayInTimerTicks(*currentPacket_) >= 3
		&& !currentPacket_->HasData()
		&& currentPacket_->HasDelay())
	{
		currentPacket_ = nullptr;
	}

	if (currentPacket_)
	{
		CheckFlush();
	}
	
	if (!currentPacket_)
	{
		auto writeCommand(std::make_unique<PSG::Packet>());
		currentPacket_ = writeCommand.get();
		commands_.Write(move(writeCommand));
	}

	currentPacket_->AppendData(datum.Clone());
}




void BuildPacketVisitor::Visit(CommandVisitor& visitor)
{
	commands_.Visit(visitor);
}


void BuildPacketVisitor::Visit(PSG::PacketVisitor& visitor)
{
	commands_.Visit(visitor);
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
