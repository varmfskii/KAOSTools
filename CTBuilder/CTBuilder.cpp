#include "BinaryParser.h"
#include "Instrument.h"
#include "Pattern.h"
#include "Channel.h"
#include "Module.h"
#include "Effect.h"
#include <zlib.h>
#include <fstream>
#include <iostream>
#include <string>
#include <iterator>
#include <vector>
#include <memory>


bool LoadFile(const std::string& filename, std::vector<unsigned char>& outputBuffer)
{
    std::ifstream input(filename, std::ifstream::binary);
    if (!input.is_open())
    {
        std::cerr << "Unable to open file `" << filename << "`\n";
        return false;
    }

    input.seekg(0, std::ifstream::ios_base::end);
    const size_t fileLength(input.tellg());
    input.seekg(0, std::ifstream::ios_base::beg);

    std::vector<unsigned char> fileData(fileLength);
    input.read(reinterpret_cast<char*>(&fileData[0]), fileData.size());

    if (fileLength != fileData.size())
    {
        std::cerr << "Unable to load file `" << filename << "`\n";
        return false;
    }

    outputBuffer = move(fileData);
    
    return true;
}

std::vector<std::vector<unsigned int>> LoadPatternMatrix(BinaryParser& parser, unsigned int numberOfChannels, unsigned int patternCount)
{
	std::vector<std::vector<unsigned int>> matrix(numberOfChannels);
	for (auto& channel : matrix)
	{
		// skip matrix data
		for (auto i(0U); i < patternCount; ++i)
		{
			channel.push_back(parser.ReadByte());
		}
	}

	return matrix;
}


void LoadWaveTable(BinaryParser& parser)
{
	const auto waveTableCount(parser.ReadByte());
	for (int i(0); i < waveTableCount; ++i)
	{
		parser.ReadQuadArrayEx();
	}
}

std::optional<std::vector<std::unique_ptr<Instrument>>> LoadInstruments(BinaryParser& parser)
{
	const auto instrumentCount(parser.ReadByte());
	std::vector<std::unique_ptr<Instrument>> instruments;
	instruments.reserve(instrumentCount);
	for (auto i(0U); i < instrumentCount; ++i)
	{
		const auto name(parser.ReadString());
		const auto type(parser.ReadByte());
		if (type != 0)
		{
			std::cerr << "Unsupported instrument type for `" << name << "` in file `" << parser.GetFilename() << "`\n";
			return std::optional<std::vector<std::unique_ptr<Instrument>>>();
		}

		//	Load volume envelope/macro
		const auto volumeMacroData(parser.ReadQuadArray());
		const auto volumeMacroLoopPos(volumeMacroData.empty() ? 0xff : parser.ReadByte());
		auto volumeMacro(
			volumeMacroLoopPos == 0xff
			? VolumeEnvelope(move(volumeMacroData))
			: VolumeEnvelope(move(volumeMacroData), volumeMacroLoopPos));
			
		//	Load arpeggio envelope/macro
		const auto arpeggioMacroData(parser.ReadSignedQuadArray());
		const auto arpeggioMacroLoopPos(arpeggioMacroData.empty() ? 0xff : parser.ReadByte());
		const auto arpeggioMacroMode(parser.ReadByte() == 0 ? ArpeggioEnvelope::Mode::Normal : ArpeggioEnvelope::Mode::Fixed);
		auto arpeggioMacro(
			arpeggioMacroLoopPos == 0xff
			? ArpeggioEnvelope(move(arpeggioMacroData), arpeggioMacroMode)
			: ArpeggioEnvelope(move(arpeggioMacroData), arpeggioMacroLoopPos, arpeggioMacroMode));

		//	Load noise envelope/macro
		const auto noiseMacroData(parser.ReadQuadArray());
		const auto noiseMacroLoopPos(noiseMacroData.empty() ? 0xff : parser.ReadByte());
		auto noiseMacro(
			noiseMacroLoopPos == 0xff
			? NoiseEnvelope(noiseMacroData)
			: NoiseEnvelope(noiseMacroData, noiseMacroLoopPos));
	
		//	Load wave envelope/macro
		const auto waveformMacroData(parser.ReadQuadArray());
		const auto waveformMacroLoopPos(waveformMacroData.empty() ? 0xff : parser.ReadByte());


		auto instrument(std::make_unique<GenericInstrument>(
			name,
			std::move(volumeMacro),
			std::move(arpeggioMacro),
			std::move(noiseMacro)));
		instruments.emplace_back(move(instrument));
	}

	return move(instruments);
}

std::vector<std::unique_ptr<Channel>> LoadChannels(
	BinaryParser& parser,
	unsigned int numberOfChannels,
	unsigned int patternCount,
	unsigned int patternRowCount,
	const std::vector<std::vector<unsigned int>>& patternMatrix)
{
	std::vector<std::unique_ptr<Channel>> channels;
	for (auto channelIndex(0U); channelIndex < numberOfChannels; ++channelIndex)
	{
		const auto effectsCount(parser.ReadByte());
		auto channel(std::make_unique<Channel>(effectsCount));
		for (auto patternIndex(0U); patternIndex < patternCount; ++patternIndex)
		{
			auto pattern(std::make_unique<Pattern>(patternRowCount, effectsCount));
			for (auto i(0U); i < patternRowCount; ++i)
			{
				const auto note(parser.ReadWord());
				const auto octave(parser.ReadWord());
				const auto volume(parser.ReadWord());

				std::vector<Effect> effects;
				for (auto i(0U); i < effectsCount; ++i)
				{
					const auto effectCode(parser.ReadWord());
					const auto effectArgument(parser.ReadWord());

					effects.emplace_back(effectCode, effectArgument);
				}
				const auto instrument(parser.ReadWord());

				pattern->SetNote(i, note, octave, volume, instrument, move(effects));
			}

			channel->AddPattern(move(pattern));
		}

		for (auto patternIndex(0U); patternIndex < patternCount; ++patternIndex)
		{
			channel->SetSequencePattern(patternIndex, patternMatrix[channelIndex][patternIndex]);
		}

		channels.push_back(move(channel));
	}

	return channels;
}

int main()
{
	//	Load the test file
	const std::string filename("../TestData/DMF/ryu_ending.dmf");

	std::vector<unsigned char> rawBuffer;
	if(!LoadFile(filename, rawBuffer))
	{
		return EXIT_FAILURE;
	}

	uLong rawBufferLength(static_cast<uLong>(rawBuffer.size()));
	std::vector<unsigned char> dmfBuffer(1024*1024*16);
	uLong dmfBufferLength(static_cast<uLong>(dmfBuffer.size()));
	if (uncompress(dmfBuffer.data(), &dmfBufferLength, rawBuffer.data(), rawBufferLength) != Z_OK)
	{
		std::cerr << "Error encountered while decompressing file `" << filename << "`\n";
		return EXIT_FAILURE;
	}
	dmfBuffer.resize(dmfBufferLength);


	BinaryParser parser(filename, dmfBuffer.begin(), dmfBuffer.end());

	if (parser.ReadString(16) != ".DelekDefleMask.")
	{
		std::cerr << "Incorrect signature encountered processing file `" << filename << "`\n";
		return EXIT_FAILURE;
	}


	//	Start with version 17 of the file format
	if (parser.ReadByte() != 0x11)
	{
		std::cerr << "Unsupported version for file `" << filename << "`\n";
		return EXIT_FAILURE;
	}
	
	static const unsigned char SYSTEM_SMS = 3;

	if(parser.ReadByte() != SYSTEM_SMS)
	{
		std::cerr << "Unsupported system type file `" << filename << "`\n";
		return EXIT_FAILURE;
	}

	const auto numberOfChannels(4U);


	const auto title(parser.ReadString());
	const auto author(parser.ReadString());
	parser.ReadByte();	//	Skip highlightA
	parser.ReadByte();	//	Skip highlightB

	const auto timeBase(parser.ReadByte());
	const auto tickTime1(parser.ReadByte());
	const auto tickTime2(parser.ReadByte());
	const auto clockRate(parser.ReadByte());	//	50hz/60hz
	const auto usingCustomClockRate(parser.ReadByte());
	const auto customClockRate1(parser.ReadByte());
	const auto customClockRate2(parser.ReadByte());
	const auto customClockRate3(parser.ReadByte());
	const auto patternRowCount(parser.ReadByte());
	const auto patternCount(parser.ReadByte());
	const auto arpeggioTickSpeed(parser.ReadByte());


	// skip matrix data
	auto patternMatrix(LoadPatternMatrix(parser, numberOfChannels, patternCount));

	auto instruments(LoadInstruments(parser));
	if (!instruments.has_value())
	{
		return EXIT_FAILURE;
	}

	LoadWaveTable(parser);


	auto channels(LoadChannels(parser, numberOfChannels, patternCount, patternRowCount, patternMatrix));



	//const auto xxxx(parser.ReadByte());
	//const auto xxxx(parser.ReadByte());
	//const auto xxxx(parser.ReadByte());
	//const auto xxxx(parser.ReadByte());


	Module module;

	module.SetTitle(title);
	module.SetAuthor(author);
	module.SetTickMultiplier(timeBase);
	module.SetEvenRowTickRate(tickTime1);
	module.SetOddRowTickRate(tickTime2);
	if (!usingCustomClockRate)
	{
		module.SetDriverClock(clockRate ? 60 : 50);
	}
	else
	{
		//	The value is stored in ASCII. Dunno dafuq why.
		module.SetDriverClock(
			((customClockRate1 - 0x30) * 100)
			+ ((customClockRate2 - 0x30) * 10)
			+ (customClockRate3 - 0x30));
	}
	//	FIXME: Add patternRowCount
	//	FIXME: Add patternCount
	module.SetDefaultArpeggioTickRate(arpeggioTickSpeed);

	for (auto& instrument : *instruments)
	{
		module.AddInstrument(move(instrument));
	}

	for (auto& channel : channels)
	{
		module.AddChannel(move(channel));
	}


	return EXIT_SUCCESS;

}

