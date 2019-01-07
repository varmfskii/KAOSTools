#pragma once
#include "Pattern.h"
#include <memory>


class Channel
{
public:

	using size_type = std::size_t;

	struct PatternInfo
	{
		PatternInfo(std::unique_ptr<Pattern> pattern, size_type remapTo)
			:
			m_Pattern(move(pattern)),
			m_RemapTo(remapTo)
		{}

		std::unique_ptr<Pattern>	m_Pattern;
		size_type					m_RemapTo;
	};


	using container_type = std::vector<PatternInfo>;


public:

	Channel(size_type effectsCount);

	void AddPattern(std::unique_ptr<Pattern> pattern);
	void SetSequencePattern(size_type setFor, size_type setTo);


protected:

	size_type					m_EffectsCount;
	std::vector<PatternInfo>	m_Patterns;
};