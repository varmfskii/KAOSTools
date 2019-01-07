#include "Channel.h"


Channel::Channel(size_type effectsCount)
{}


void Channel::AddPattern(std::unique_ptr<Pattern> pattern)
{
	//	FIXME: Check pattern for null
	//	FIXME: check pattern for correct number of effects
	m_Patterns.emplace_back(move(pattern), m_Patterns.size());
}


void Channel::SetSequencePattern(size_type setFor, size_type setTo)
{
	m_Patterns[setFor].m_RemapTo = setTo;
}
