//	VGMTool for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2019, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include "CostInfo.h"
#include <PSG/Packet.h>



double CostInfo::GetNativeWriteRanking(const size_t writeCount) const
{
	if (!writeCount)
	{
		return 0.0f;
	}

	return interruptEnterCycleCost + firstWriteCycleCost + ((writeCount - 1) * extraWriteCycleCost);
}


double CostInfo::GetNativeWriteRanking(const PSG::Packet& packet) const
{
	return GetNativeWriteRanking(packet.GetWriteCount());
}




double CostInfo::GetNativeWriteCostInCycles(uint64_t sequenceId) const
{
	if (!sequenceId)
	{
		return 0;
	}
	else if(sequenceId == 1)
	{
		return interruptEnterCycleCost + firstWriteCycleCost;
	}

	return extraWriteCycleCost;
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
