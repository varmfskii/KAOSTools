#include "Module.h"


void Module::SetAuthor(std::string author)
{
	m_Author = author;
}

void Module::SetTitle(std::string title)
{
	m_Title = title;
}


void Module::SetTickMultiplier(unsigned int multiplier)
{
	m_Multiplier = multiplier;
}

void Module::SetEvenRowTickRate(unsigned int tickRate)
{
	m_EvenRowTickRate = tickRate;
}

void Module::SetOddRowTickRate(unsigned int tickRate)
{
	m_OddRowTickRate = tickRate;
}

void Module::SetDriverClock(unsigned int clockRate)
{
	m_DriverClockRate = clockRate;
}

void Module::SetDefaultArpeggioTickRate(unsigned int tickRate)
{
	m_ArpeggioTickRate = tickRate;
}
	

void Module::AddInstrument(std::unique_ptr<Instrument> instrument)
{
	m_Instruments.emplace_back(move(instrument));
}
	
void Module::AddChannel(std::unique_ptr<Channel> channel)
{
	m_Channels.emplace_back(move(channel));
}
