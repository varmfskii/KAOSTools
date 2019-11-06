//	VGMTool for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2019, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#pragma once
#include "HardwareConfig.h"
#include "CostInfo.h"
#include  <iosfwd>


class DumpVisitor : public PSG::CommandVisitor
{
public:

	DumpVisitor(const HardwareConfig& hardwareConfig, const CostInfo& costInfo, std::ostream& output, bool verbose);

	void OnPSGDelay(double delay) override;
	void OnPSGWrite(const PSG::CommandData& datum, uint64_t sequenceId) override;


protected:

	static std::string PadString(std::string str, size_t width);

	static const uint64_t maxGimeTimerValue_ = 0x1000;


	const HardwareConfig&	hardwareConfig_;
	const CostInfo&			costInfo_;
	const bool				verbose_ = false;
	std::ostream&			output_;
	uint64_t				currentSequenceId_ = 0;
	uint64_t				currentCycleCount_ = 0;
	uint64_t				currentSampleCount_ = 0;
	uint64_t				currentPacketCount_ = 0;

	uint64_t				currentWriteCount_ = 0;
	uint64_t				currentDelayCount_ = 0;
};




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
