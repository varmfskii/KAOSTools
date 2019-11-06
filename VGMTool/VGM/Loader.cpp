//	VGMTool for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2019, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include <VGM/Loader.h>
#include <PSG/DelayCommand.h>
#include <PSG/WriteCommand.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>



namespace VGM
{

	bool Loader::Load(std::istream& input, PSG::CommandSequence& vgmStreamOut)
	{
		const auto handlers(CreateHandlers());

		PSG::CommandSequence output;

		uint32_t signature;
		input.read(reinterpret_cast<char*>(&signature), sizeof(signature));

		uint32_t endOfStreamPos;
		input.read(reinterpret_cast<char*>(&endOfStreamPos), sizeof(endOfStreamPos));

		uint32_t version;
		input.read(reinterpret_cast<char*>(&version), sizeof(version));

		uint32_t sn76489clock;
		input.read(reinterpret_cast<char*>(&sn76489clock), sizeof(sn76489clock));

		uint32_t ym2413clock;
		input.read(reinterpret_cast<char*>(&ym2413clock), sizeof(ym2413clock));

		uint32_t gd3offset;
		input.read(reinterpret_cast<char*>(&gd3offset), sizeof(gd3offset));

		uint32_t totalSampleCount;
		input.read(reinterpret_cast<char*>(&totalSampleCount), sizeof(totalSampleCount));

		uint32_t loopOffset;
		input.read(reinterpret_cast<char*>(&loopOffset), sizeof(loopOffset));

		uint32_t loopSampleCount;
		input.read(reinterpret_cast<char*>(&loopSampleCount), sizeof(loopSampleCount));

		uint32_t sampleRate;
		input.read(reinterpret_cast<char*>(&sampleRate), sizeof(sampleRate));

		uint32_t ym2612clock;
		input.read(reinterpret_cast<char*>(&ym2612clock), sizeof(ym2612clock));

		uint32_t ym2151clock;
		input.read(reinterpret_cast<char*>(&ym2151clock), sizeof(ym2151clock));

		uint32_t dataOffset;
		input.read(reinterpret_cast<char*>(&dataOffset), sizeof(dataOffset));



		auto headerSize = 0U;
		if (version <= 0x150)
		{
			headerSize = 64;
		}
		else if (version == 0x151)
		{
			headerSize = 128;
		}
		else
		{
			std::cerr << "Unknown version " << std::hex << version << "\n";
			return false;
		}

		input.seekg(headerSize);	//	FIXME

		endOfStreamEncountered_ = false;

		while(!endOfStreamEncountered_)
		{
			unsigned char command;
			input.read(reinterpret_cast<char*>(&command), sizeof(command));
			auto handler = handlers.find(command);
			if (handler == handlers.end())
			{
					std::cerr << "Unknown command " << std::hex << static_cast<unsigned int>(command) << "\n";
			}

			if (!handler->second(input, output))
			{
				return false;
			}
		}

		vgmStreamOut = std::move(output);

		return true;
	}


	Loader::handlermap_type Loader::CreateHandlers()
	{
		using namespace std::placeholders;

		handlermap_type handlers = {
			{ 0x61, std::bind(&Loader::OnVariableDelay, this, _1, _2) },
			{ 0x62, std::bind(&Loader::OnFixedDelay, this, _1, _2, 735) },
			{ 0x63, std::bind(&Loader::OnFixedDelay, this, _1, _2, 882) },
			{ 0x66, std::bind(&Loader::OnEndOfStream, this, _1, _2) },

			{ 0x70, std::bind(&Loader::OnFixedDelay, this, _1, _2, 1) },
			{ 0x71, std::bind(&Loader::OnFixedDelay, this, _1, _2, 2) },
			{ 0x72, std::bind(&Loader::OnFixedDelay, this, _1, _2, 3) },
			{ 0x73, std::bind(&Loader::OnFixedDelay, this, _1, _2, 4) },
			{ 0x74, std::bind(&Loader::OnFixedDelay, this, _1, _2, 5) },
			{ 0x75, std::bind(&Loader::OnFixedDelay, this, _1, _2, 6) },
			{ 0x76, std::bind(&Loader::OnFixedDelay, this, _1, _2, 7) },
			{ 0x77, std::bind(&Loader::OnFixedDelay, this, _1, _2, 8) },
			{ 0x78, std::bind(&Loader::OnFixedDelay, this, _1, _2, 9) },
			{ 0x79, std::bind(&Loader::OnFixedDelay, this, _1, _2, 10) },
			{ 0x7a, std::bind(&Loader::OnFixedDelay, this, _1, _2, 11) },
			{ 0x7b, std::bind(&Loader::OnFixedDelay, this, _1, _2, 12) },
			{ 0x7c, std::bind(&Loader::OnFixedDelay, this, _1, _2, 13) },
			{ 0x7d, std::bind(&Loader::OnFixedDelay, this, _1, _2, 14) },
			{ 0x7e, std::bind(&Loader::OnFixedDelay, this, _1, _2, 15) },
			{ 0x7f, std::bind(&Loader::OnFixedDelay, this, _1, _2, 16) },

			{ 0x4f, std::bind(&Loader::OnGameGearWrite, this, _1, _2, 0) },
			{ 0x50, std::bind(&Loader::OnSN76489Write, this, _1, _2, 0) },
			{ 0x5e, std::bind(&Loader::OnYM262WritePort2, this, _1, _2, 0) },
			{ 0x5f, std::bind(&Loader::OnYM262WritePort1, this, _1, _2, 0) },
			{ 0xa0, std::bind(&Loader::OnAY8910Write, this, _1, _2, 0) },	//	0x55 = second chip

		};

		return handlers;
	}




	bool Loader::OnFixedDelay(std::istream&, PSG::CommandSequence& output, double delay)
	{
		output.Write(std::make_unique<PSG::DelayCommand>(delay));

		return true;
	}


	bool Loader::OnVariableDelay(std::istream& input, PSG::CommandSequence& output)
	{
		uint16_t sampleCount(0);

		//	Delay sampleCount samples
		input.read(reinterpret_cast<char*>(&sampleCount), sizeof(sampleCount));
	
		return OnFixedDelay(input, output, sampleCount);
	}


	bool Loader::OnSN76489Write(std::istream& input, PSG::CommandSequence& output, uint64_t /*chipId*/)
	{
		unsigned char dataValue = 0;

		input.read(reinterpret_cast<char*>(&dataValue), sizeof(dataValue));

		output.Write(std::make_unique<PSG::WriteCommand>(dataValue));

		return true;
	}


	bool Loader::OnAY8910Write(std::istream& input, PSG::CommandSequence& output, uint64_t /*chipId*/)
	{
		unsigned char regValue = 0;
		unsigned char dataValue = 0;

		input.read(reinterpret_cast<char*>(&regValue), sizeof(regValue));
		input.read(reinterpret_cast<char*>(&dataValue), sizeof(dataValue));

		output.Write(std::make_unique<PSG::WriteCommand>(regValue, dataValue));

		return true;
	}


	bool Loader::OnYM262WritePort1(std::istream& input, PSG::CommandSequence& output, uint64_t /*chipId*/)
	{
		unsigned char regValue = 0;
		unsigned char dataValue = 0;

		input.read(reinterpret_cast<char*>(&regValue), sizeof(regValue));
		input.read(reinterpret_cast<char*>(&dataValue), sizeof(dataValue));

		output.Write(std::make_unique<PSG::WriteCommand>(regValue, dataValue));

		return true;
	}


	bool Loader::OnYM262WritePort2(std::istream& input, PSG::CommandSequence& output, uint64_t /*chipId*/)
	{
		unsigned char regValue = 0;
		unsigned char dataValue = 0;

		input.read(reinterpret_cast<char*>(&regValue), sizeof(regValue));
		input.read(reinterpret_cast<char*>(&dataValue), sizeof(dataValue));

		output.Write(std::make_unique<PSG::WriteCommand>(regValue, dataValue));

		return true;
	}


	bool Loader::OnEndOfStream(std::istream& /*input*/, PSG::CommandSequence& /*output*/)
	{
		endOfStreamEncountered_ = true;

		return true;
	}

	//	NOP - We do nothing with the data
	bool Loader::OnGameGearWrite(std::istream& input, PSG::CommandSequence& output, uint64_t /*chipId*/)
	{
		unsigned char dataValue = 0;

		input.read(reinterpret_cast<char*>(&dataValue), sizeof(dataValue));

		return true;
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
