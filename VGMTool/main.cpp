//	VGMTool for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2019, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include <Visitors/ProfilerVisitor.h>
#include <Visitors/BuildPacketVisitor.h>
#include <Visitors/DumpVisitor.h>
#include <PSG/CommandSequence.h>
#include <CPF/PacketWriterVisitor.h>
#include <VGM/Loader.h>
#include <VGM/WriterVisitor.h>
#include <iostream>
#include <fstream>
#include <string>
#include <optional>


using CommandHandler = std::function<void(const std::vector<std::string>&, const HardwareConfig&, const CostInfo&, PSG::CommandSequence&, const std::string&)>;


void DumpCommand(const std::vector<std::string>& /*arguments*/, const HardwareConfig& hardwareConfig, const CostInfo& costInfo, PSG::CommandSequence& vgmStreamOut, const std::string& /*inputFilename*/)
{
	DumpVisitor dumpVisitor(hardwareConfig, costInfo, std::cout, true);
	vgmStreamOut.Visit(dumpVisitor);
}


void ProfileCommand(const std::vector<std::string>& /*arguments*/, const HardwareConfig& hardwareConfig, const CostInfo& costInfo, PSG::CommandSequence& vgmStream, const std::string& /*inputFilename*/)
{
	ProfilerVisitor visitor(hardwareConfig, 20);
	vgmStream.Visit(visitor);

	DumpVisitor dumpVisitor(hardwareConfig, costInfo, std::cout, false);
	std::cout << "\n\n- Unprocessed -----------------------------\n\n";
	vgmStream.Visit(dumpVisitor);

	std::cout << "\n\n- Reordered and packetized ----------------\n\n";
	std::cout << "interruptEnterCycleCost = " << costInfo.interruptEnterCycleCost << "\n";
	std::cout << "delayBiasCycleCost      = " << costInfo.delayBiasCycleCost << "\n";
	std::cout << "singleDelayCycleCost    = " << costInfo.singleDelayCycleCost << "\n";
	std::cout << "firstWriteCycleCost     = " << costInfo.firstWriteCycleCost << "\n";
	std::cout << "extraWriteCycleCost     = " << costInfo.extraWriteCycleCost << "\n";
	std::cout << "timerCountdownCycleCost = " << costInfo.timerCountdownCycleCost << "\n";
	std::cout << "\n\n";
	BuildPacketVisitor consolidated(hardwareConfig, costInfo);
	vgmStream.Visit(consolidated);
	consolidated.Visit(dumpVisitor);
}


void BuildCommand(const std::vector<std::string>& /*arguments*/, const HardwareConfig& hardwareConfig, const CostInfo& costInfo, PSG::CommandSequence& vgmStream, const std::string& inputFilename)
{
	BuildPacketVisitor consolidated(hardwareConfig, costInfo);
	vgmStream.Visit(consolidated);

	//	Save it out
	std::ofstream output;
	output.open(inputFilename + ".cpf.bin", std::ofstream::binary);
	CFP::PacketWriterVisitor writer(output, hardwareConfig);
	consolidated.Visit(writer);
	writer.Finalize();
}


bool ProcessList(
	const std::vector<std::string>& inputFilenames,
	const std::vector<std::string>& arguments,
	CommandHandler handler,
	const HardwareConfig& hardwareConfig,
	const CostInfo& costInfo)
{
	for (const auto& inputFilename : inputFilenames)
	{
		std::ifstream input(inputFilename, std::ifstream::binary);
		if (!input.is_open())
		{
			std::cerr << "Unable to open `" << inputFilename << "`.\n";
			return false;
		}

		PSG::CommandSequence vgmStream;
		VGM::Loader loader;
		if (!loader.Load(input, vgmStream))
		{
			return false;
		}

		handler(arguments, hardwareConfig, costInfo, vgmStream, inputFilename);
	}


	return true;
}




int main(int argc, char** argv)
{
	if (argc < 3)
	{
		std::cerr << "Syntax: vmgtool >command> [args...]\n";
		return EXIT_FAILURE;
	}

	std::map<std::string, CommandHandler> commandHandlers = {
		{"dump", DumpCommand},
		{"profile", ProfileCommand},
		{"build", BuildCommand}
	};

	const std::string command(argv[1]);
	const auto handler(commandHandlers.find(command));
	if (handler == commandHandlers.end())
	{
		std::cerr << "Unknown command '" << command << "'\n";
		return EXIT_FAILURE;
	}



	std::vector<std::string> args(argv + 2, argv + argc);

	std::vector<std::string> inputFilenames;
	std::string outputFilename;
	std::optional<double> interruptEnterCycleCost;		//	 Cost in samples
	std::optional<double> firstWriteCycleCost;			//	 Cost in samples
	std::optional<double> extraWriteCycleCost;			//	 Cost in samples
	std::optional<double> pitInterval;

	bool hasError(false);
	for (auto arg : args)
	{
		if (arg.empty())
		{
			continue;
		}

		const auto originalArg(arg);

		if (arg[0] == '-')
		{
			bool handled(true);
			arg.erase(arg.begin());
			if (!arg.empty() && arg[0] == '-')
			{
				arg.erase(arg.begin());

				const auto separatorPos(arg.find('='));
				const auto argValue(separatorPos == std::string::npos ? "" : arg.substr(separatorPos + 1));
				if (separatorPos != std::string::npos)
				{
					arg.resize(separatorPos);
				}

				if (arg == "overhead-cost")
				{
					interruptEnterCycleCost = std::stod(argValue);
				}
				else if (arg == "chipwrite-cost")
				{
					firstWriteCycleCost = std::stod(argValue);
				}
				else if (arg == "chipwrite-cost-extra")
				{
					extraWriteCycleCost = std::stod(argValue);
				}
				else if (arg == "pit-interval-279nsec")
				{
					pitInterval = HardwareConfig::pitInterval279nsec;
				}
				else if (arg == "pit-interval-63usec")
				{
					pitInterval = HardwareConfig::pitInterval63usec;
				}
				else
				{
					handled = false;
				}
			}
			else
			{
				handled = false;
			}

			if (!handled)
			{
				std::cerr << "Unknown command line argument `" << originalArg << "`\n";
			}

		}
		else
		{
			inputFilenames.emplace_back(move(arg));
		}
	}

	if (hasError)
	{
		return EXIT_FAILURE;
	}

	if (inputFilenames.empty())
	{
		std::cerr << "No input files specified.\n";
		return EXIT_FAILURE;
	}

	auto failed = false;
	for (const auto& inputFilename : inputFilenames)
	{
		std::ifstream input(inputFilename);
		if (!input.is_open())
		{
			std::cerr << "Unable to open `" << inputFilename << "`.\n";
			failed = true;
		}
	}

	//	Return if failed
	if (failed)
	{
		return EXIT_FAILURE;
	}

	CostInfo costInfo;
	HardwareConfig hardwareConfig;
	if (interruptEnterCycleCost.has_value())
	{
		costInfo.interruptEnterCycleCost = interruptEnterCycleCost.value();
	}

	if (firstWriteCycleCost.has_value())
	{
		costInfo.firstWriteCycleCost = firstWriteCycleCost.value();
	}

	if (pitInterval.has_value())
	{
		hardwareConfig.pitInterval = pitInterval.value();
	}

	if (extraWriteCycleCost.has_value())
	{
		costInfo.extraWriteCycleCost = extraWriteCycleCost.value();
	}



	if (!ProcessList(inputFilenames, args, handler->second, hardwareConfig, costInfo))
	{
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
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
