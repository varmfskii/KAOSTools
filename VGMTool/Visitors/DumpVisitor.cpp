//	VGMTool for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2019, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include <Visitors/DumpVisitor.h>
#include <ostream>
#include <iomanip>
#include <sstream>

#define DUMP_ALL


DumpVisitor::DumpVisitor(const HardwareConfig& hardwareConfig, const CostInfo& costInfo, std::ostream& output, bool verbose)
	:
	hardwareConfig_(hardwareConfig),
	costInfo_(costInfo),
	verbose_(verbose),
	output_(output)
{}


std::string DumpVisitor::PadString(std::string str, size_t width)
{
	if (str.size() < width)
	{
		str += std::string(width - str.size(), ' ');
	}

	return str;
}

void DumpVisitor::OnPSGDelay(double sampleCount)
{
	currentSequenceId_ = 0;
	const auto nativeDelayInTicks(hardwareConfig_.GetNativeDelayInTimerTicks(sampleCount));
	const auto nativeDelayInCycles(hardwareConfig_.GetNativeDelayInCycles(sampleCount));

	std::string formatted;
	std::ostringstream formatter;

	formatter << "-- Delay(" << std::dec << std::fixed << std::setprecision(2) << sampleCount << ")";
	formatted += PadString(formatter.str(), 25);

	const uint64_t totalDelaysNeeded((nativeDelayInTicks / maxGimeTimerValue_) + .9999999);
	if (totalDelaysNeeded > 1)
	{
		currentDelayCount_  += (totalDelaysNeeded - 1);
		const auto delayCostInCycles(totalDelaysNeeded * costInfo_.timerCountdownCycleCost);
		currentCycleCount_ += delayCostInCycles;

		if (verbose_)
		{
			formatter.str("");
			formatter << "CPUCycleCost(" << std::dec << std::fixed << std::setprecision(2) << delayCostInCycles << ")";
			formatted += PadString(formatter.str(), 25);
		}
	}

	if (verbose_)
	{
		formatter.str("");
		formatter << "CPUCycleDelay(" << std::dec << std::fixed << std::setprecision(2) << nativeDelayInCycles << ")";
		formatted += PadString(formatter.str(), 25);

		formatter.str("");
		formatter << "GIMETimerDelay(" << std::dec << std::fixed << std::setprecision(2) << nativeDelayInTicks << ")";
		formatted += PadString(formatter.str(), 25);
		//formatted += std::string(16 - (formatted.size() % 16), ' ');
	}

	output_ << formatted;

	currentSampleCount_ += sampleCount;
	++currentPacketCount_;
	if(currentSampleCount_ > hardwareConfig_.vgmSampleFrequency)
	{
		const auto percentUsed(currentCycleCount_ / hardwareConfig_.cpuSampleFrequenry);
		if (verbose_)
		{
			output_ << "\n";
		}

		output_
			<< "@@ CPU Cycles for 1 second = " << std::dec << currentCycleCount_
			<< "  %" << std::dec << std::fixed << std::setprecision(4) << percentUsed << " of CPU"
			<< "  (Write = " << currentWriteCount_
			<< ", WritePackets = " << currentPacketCount_ << ")"
			<< ", Extra Delays = " << currentDelayCount_ << ")"
			<< "\n";
		currentCycleCount_ = 0;
		currentWriteCount_ = 0;
		currentDelayCount_ = 0;
		currentPacketCount_ = 0;
		currentSampleCount_ -= hardwareConfig_.vgmSampleFrequency;
	}

	output_ << "\n";
}




void DumpVisitor::OnPSGWrite(const PSG::CommandData& datum, uint64_t sequenceId)
{
	if (!sequenceId)
	{
		sequenceId = ++currentSequenceId_;
	}
	const auto nativeCostInCycles(costInfo_.GetNativeWriteCostInCycles(sequenceId));
	currentCycleCount_ += nativeCostInCycles;
	currentWriteCount_++;


	std::ostringstream formatter;
	formatter << ">> Write(";

	bool doComma(false);
	for (const auto& value : datum.GetData())
	{
		if (doComma)
		{
			formatter << ", ";
		}

		formatter << "0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<unsigned int>(value);

		doComma = true;
	}

	formatter << ")";
	std::string formatted(PadString(formatter.str(), 25));

	if (verbose_)
	{
		formatter.str("");
		formatter << "CPUCycleCost(" << std::dec << std::fixed << std::setprecision(2) << nativeCostInCycles << ")";
		formatted += formatter.str();
		//formatted += std::string(16 - (formatted.size() % 16), ' ');
	}


	output_ << formatted << "\n";
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
