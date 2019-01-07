#include "Instrument.h"


Instrument::Instrument(std::string name)
	: name(move(name))
{}


GenericInstrument::GenericInstrument(
	std::string name,
	VolumeEnvelope volumeEnvelope,
	ArpeggioEnvelope arpeggioEnvelope,
	NoiseEnvelope noisePitchEnvelope)
	:
	Instrument(move(name)),
	m_VolumeEnvelope(std::move(volumeEnvelope)),
	m_ArpeggioEnvelope(std::move(arpeggioEnvelope)),
	m_NoisePitchEnvelope(std::move(noisePitchEnvelope))
{}


