#pragma once
#include "Note.h"
#include "Effect.h"


class Pattern
{
public:

	Pattern(unsigned int rowCount, unsigned int effectsCount);

	void SetNote(unsigned int row,
		unsigned int note,
		unsigned int octave,
		unsigned int volume,
		unsigned int instrument,
		std::vector<Effect> effects);
};