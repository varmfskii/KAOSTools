#pragma once
#include "Envelope.h"


ArpeggioEnvelope::ArpeggioEnvelope(std::vector<value_type> envelopeData, Mode mode)
	:
	Envelope(move(envelopeData)),
	mode(mode)
{}

ArpeggioEnvelope::ArpeggioEnvelope(std::vector<value_type> envelopeData, size_t loopPosition, Mode mode)
	:
	Envelope(move(envelopeData), loopPosition),
	mode(mode)
{}
