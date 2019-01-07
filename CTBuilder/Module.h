#pragma once
#include "Instrument.h"
#include "Channel.h"
#include <string>
#include <memory>


class Module
{
public:

	Module() = default;


	void SetAuthor(std::string author);
	void SetTitle(std::string title);

	void SetTickMultiplier(unsigned int multiplier);
	void SetEvenRowTickRate(unsigned int tickRate);
	void SetOddRowTickRate(unsigned int tickRate);
	void SetDriverClock(unsigned int clockRate);
	void SetDefaultArpeggioTickRate(unsigned int tickRate);

	void AddInstrument(std::unique_ptr<Instrument> instrument);
	void AddChannel(std::unique_ptr<Channel> channel);


protected:

	std::string		m_Author;
	std::string		m_Title;
	unsigned int	m_Multiplier = 6;
	unsigned int	m_EvenRowTickRate = 1;
	unsigned int	m_OddRowTickRate = 1;
	unsigned int	m_DriverClockRate = 60;
	unsigned int	m_ArpeggioTickRate = 1;
	std::vector<std::unique_ptr<Instrument>>	m_Instruments;
	std::vector<std::unique_ptr<Channel>>		m_Channels;
};

