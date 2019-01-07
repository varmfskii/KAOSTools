#pragma once
#include <vector>
#include <optional>


template<class Type_, class Tag_>
class Envelope
{
public:

	using value_type = Type_;

	Envelope() = default;

	Envelope(std::vector<value_type> envelopeData)
		: envelopeData(move(envelopeData))
	{}

	Envelope(std::vector<value_type> envelopeData, size_t loopPosition)
		:
		envelopeData(move(envelopeData)),
		loopPosition(loopPosition)
	{}

private:

	std::vector<value_type>	envelopeData;		//!< Volume envelope data. MAx size = 127
	std::optional<size_t>	loopPosition;		//!< The loop position of the volume envelope.
};




class VolumeEnvelope : public Envelope<unsigned int, VolumeEnvelope>
{
public:

	using Envelope::Envelope;
};




class NoiseEnvelope : public Envelope<unsigned int, NoiseEnvelope>
{
public:

	using Envelope::Envelope;
};




class ArpeggioEnvelope : public Envelope<signed int, ArpeggioEnvelope>
{
public:

	enum Mode
	{
		Normal,
		Fixed
	};

	ArpeggioEnvelope(std::vector<value_type> envelopeData, Mode mode);
	ArpeggioEnvelope(std::vector<value_type> envelopeData, size_t loopPosition, Mode mode);


private:

	Mode				mode;		//!< The arpeggio macro mode (0 = Normal, 1 = Fixed)
};


