//	VGMTool for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2019, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#pragma once
#include <PSG/Packet.h>



class HardwareConfig
{
public:

	HardwareConfig() = default;
	HardwareConfig(const HardwareConfig&) = delete;
	virtual ~HardwareConfig() = default;

	virtual double GetNativeDelayRanking(double vgmSampleCount) const;
	virtual double GetNativeDelayRanking(const PSG::Packet& packet) const;

	virtual double GetNativeDelayInTimerTicks(double vgmSampleCount) const;
	virtual double GetNativeDelayInTimerTicks(const PSG::Packet& packet) const;

	virtual double GetNativeDelayInCycles(double vgmSampleCount) const;
	virtual double GetNativeDelayInCycles(const PSG::Packet& packet) const;


public:

	static const double pitInterval279nsec;
	static const double pitInterval63usec;

	//	
	//	GIME Timer 279.365nsec	0.000000279365
	//	GIME Timer 63.695usec	0.000063695
	//	YML262 Timer1			80usec
	//	YML262 Timer2			320usec
	double pitInterval = 0.000063695;								//	In seconds


	//	Sample frequencies
	const double vgmSampleFrequency = 44100;							//	In Hz (host)
	const double cpuSampleFrequenry = 1780000;							//	In Hz (native)
	const double pitSampleFrequency = 1 / pitInterval;					//	In hz (native)

	//const double pitCPUCycleInterval = cpuSampleFrequenry/pitSampleFrequency;	//	In cpu cycles
	//const double vgmCPUCycleInterval = cpuSampleFrequenry/vgmSampleFrequency;	//	Value of each sample in  CPU cycles
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
