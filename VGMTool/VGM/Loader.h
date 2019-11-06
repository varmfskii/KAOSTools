//	VGMTool for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2019, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#pragma once
#include <PSG/CommandSequence.h>
#include <functional>
#include <map>


namespace VGM
{

	class Loader
	{
	public:

		bool Load(std::istream& input, PSG::CommandSequence& vgmStreamOut);


	protected:

		using handler_type = std::function<bool(std::istream&, PSG::CommandSequence&)>;
		using handlermap_type = std::map<unsigned int, handler_type>;

		handlermap_type CreateHandlers();


		bool OnEndOfStream(std::istream& input, PSG::CommandSequence& output);

		//	Delay handlers
		bool OnVariableDelay(std::istream& input, PSG::CommandSequence& output);
		bool OnFixedDelay(std::istream&, PSG::CommandSequence& output, double delay);

		//	PSG chip handlers
		bool OnAY8910Write(std::istream& input, PSG::CommandSequence& output, uint64_t chipId);
		bool OnSN76489Write(std::istream& input, PSG::CommandSequence& output, uint64_t chipId);
		bool OnYM262WritePort1(std::istream& input, PSG::CommandSequence& output, uint64_t chipId);
		bool OnYM262WritePort2(std::istream& input, PSG::CommandSequence& output, uint64_t chipId);

		//	Specialized hardware
		bool OnGameGearWrite(std::istream& input, PSG::CommandSequence& output, uint64_t chipId);



	protected:

		handlermap_type handlers_;
		bool endOfStreamEncountered_ = false;
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
