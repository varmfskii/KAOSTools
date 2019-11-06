//	VGMTool for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2019, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include "HardwareConfig.h"
#include <PSG/Packet.h>


const double HardwareConfig::pitInterval279nsec = 0.000000279365;
const double HardwareConfig::pitInterval63usec = 0.000063695;


double HardwareConfig::GetNativeDelayRanking(double vgmSampleCount) const
{
	return vgmSampleCount * cpuSampleFrequenry / vgmSampleFrequency;
}


double HardwareConfig::GetNativeDelayRanking(const PSG::Packet& packet) const
{
	return GetNativeDelayRanking(packet.GetDelay());
}


double HardwareConfig::GetNativeDelayInTimerTicks(double vgmSampleCount) const
{
	return vgmSampleCount * pitSampleFrequency / vgmSampleFrequency;
}


double HardwareConfig::GetNativeDelayInTimerTicks(const PSG::Packet& packet) const
{
	return GetNativeDelayInTimerTicks(packet.GetDelay());
}


double HardwareConfig::GetNativeDelayInCycles(double vgmSampleCount) const
{
	return vgmSampleCount * cpuSampleFrequenry / vgmSampleFrequency;
}


double HardwareConfig::GetNativeDelayInCycles(const PSG::Packet& packet) const
{
	return GetNativeDelayInCycles(packet.GetDelay());
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
