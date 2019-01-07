#pragma once
#include "Envelope.h"


/*

	Size		Desc
	1			Length of instrument name
	0..N		Instrument name
	1			Type (Generic, Waveform, Sample, SID, OPx)


	1			Length of volume envelope data
	0..N		Volume envelope data

	1			Length of arpeggio envelope data
	0..N		Arpeggio envelope data

	1			Length of noise envelope data
	0..N		Noise envelope data

	1			Length of wave macro
	0..N		Wave macro data


*/
class Instrument
{
public:

	Instrument(std::string name);

	virtual ~Instrument() = default;


private:

	std::string					name;			//!< The name of the instrument
};




//	See http://shiru.untergrund.net/software.shtml
//	See http://battleofthebits.org/lyceum/View/Vgm+Music+Maker/



class GenericInstrument : public Instrument
{
public:

	GenericInstrument(
		std::string name,
		VolumeEnvelope volumeEnvelope,
		ArpeggioEnvelope arpeggioEnvelope,
		NoiseEnvelope noisePitchEnvelope);


private:

	VolumeEnvelope		m_VolumeEnvelope;		//!< Volume envelope
	ArpeggioEnvelope	m_ArpeggioEnvelope;		//!< Arpeggio envelope
	NoiseEnvelope		m_NoisePitchEnvelope;	//!< Duty/noise envelope
};




class WaveformInstrument : public GenericInstrument
{
	//Envelope<std::uint32_t>		m_Waveform;				//!< Actual wavetable envelope (SID, PCENgine, GAmeboy)
};




class SIDInstrument : public WaveformInstrument
{
	unsigned char triangle_wave_enabled;
	unsigned char saw_wave_enabled;
	unsigned char pulse_wave_enabled;
	unsigned char noise_wave_enabled;
	unsigned char attack;
	unsigned char decay;
	unsigned char sustain;
	unsigned char release;
	unsigned char pulse_width;
	unsigned char ring_modulation_enabled;
	unsigned char sync_modulation_enabled;
	unsigned char to_filter;
	unsigned char volume_macro_to_filter_cutoff_enabled;
	unsigned char use_filter_values_from_instrument;
	//filter_globals
	unsigned char filter_resonance;
	unsigned char filter_cutoff;
	unsigned char filter_high_pass;
	unsigned char filter_band_pass;
	unsigned char filter_low_pass;
	unsigned char filter_ch2_off;
};











struct FMOperator
{
	unsigned char m_Amplitude;		//!< Amplitude Modulation enable, whether or not this operator will allow itself to be modified by the LFO.
	unsigned char m_AttackRate;		//!< Attack rate, the angle of initial amplitude increase.
	unsigned char m_DR;				//!< The angle of initial amplitude decrease.
	unsigned char m_MULT;			//!< A multiplier to the frequency.
	unsigned char m_RR;				//!< The final angle of amplitude decrease, after ‘key off’.
	unsigned char m_SL;				//!< The amplitude at which the slower amplitude decrease starts.
	unsigned char m_TL;				//!< Total level, the highest amplitude of the waveform.
	unsigned char m_DT2;			//!< Detune 2 used only on the YM2151 see #DT
	unsigned char m_RS;				//!< Rate scaling, the degree to which envelopes become shorter as frequencies become higher.
	unsigned char m_DT;				//!< Detune, a number between -3 and 3. Positive is a sharper detune, whilst negative is lower.
	unsigned char m_D2R;			//!< The angle of secondary amplitude decrease. This will continue indefinitely unless ‘key off’ occurs.
	unsigned char m_SSGMODE;		//!< The SSG mode (BIT 4 = 0 Disabled, 1 Enabled, BITS 0,1,2 SSG_MODE)
};


class OPInstrument : public Instrument
{
	// FM Info
	unsigned char				m_ALG;			//!< The FM Algorithm
	unsigned char				m_FB;			//!< The feedback for operator 1
	unsigned char				m_LFO1;			//!< Low Frequency Oscillator 1 (FMS on YM2612, PMS on YM2151)
	unsigned char				m_LFO2;			//!< Low Frequency Oscillator 2 (AMS on YM2612, AMS on YM2151)
	std::vector<FMOperator>		m_Operators;	//!< The operator data
};
