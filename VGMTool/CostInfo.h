//	VGMTool for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2019, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#pragma once
#include <PSG/Packet.h>



class CostInfo
{
public:

	CostInfo() = default;
	CostInfo(const CostInfo& hardwareConfig) = delete;
	virtual ~CostInfo() = default;

	virtual double GetNativeWriteRanking(size_t datumCount) const;
	virtual double GetNativeWriteRanking(const PSG::Packet& packet) const;

	virtual double GetNativeWriteCostInCycles(uint64_t sequenceId) const;


public:

	//	FIXME: We need another member here for the number of cycles eaten up
	//	by the last few instructions returning from the interrupt after resetting
	//	the timer
	//	Player CPU cycle costs
	double minimumNativeTicks = 3;				//	Minimum number of native clock ticks allowed for a delay
	double interruptEnterCycleCost = 23.0;		//	Cost of entering the processing function
	double delayBiasCycleCost = 22.0;			//	Cost of processing the remaining irq ops after resetting the irq
	double singleDelayCycleCost = 52.0;			//	Cost of processing a single delay and exit
	double firstWriteCycleCost = 74.0;			//	Additional cost of processing 1 write, resetting the timer, and existing the irq
	double extraWriteCycleCost = 21.0;			//	Additional cost of processing each extra write in the packet
	double timerCountdownCycleCost = 27.0;		//	Additional cost of processing each extra delay due to limitations of GIME!
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
