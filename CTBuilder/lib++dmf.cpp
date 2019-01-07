#include "Instrument.h"
#include <string>



enum System
{
	SegaGenesis,
	SegaGenesisEx,
	SegaMasterSystem,
	GameBoy,
	PCEngine,
	NES,
	SID8580,
	SID6581,
	YM2151
};

enum Region
{
	PAL,
	NTSC
};


/*
														OPL Channels	Tone Channels	Noise Channels		Channel Envelopes		OPL		Volume		Arpeggio	Duty	Inst. Waveform		Samples		Wave table		Noise
	YMF-262M		Mega Mini MPI						18				-				-					-						Yes
	YM-2149			CoCoPSG												3				1					1						No
	AY3-8912		Symphony 12 / Philharmonic 12						3				1					1						No
	AY3-8913		SSC													3				1					1						No
	SN74689			Game Master Cartridge								3				1					0						No
	SN74689			SirSound											3				1					0						No

	Sega Genesis																													Yes										No					Yes			No
	Sega Genesis (Ext 3)																											Yes										No					Yes			No
	PC Engine																														No										Yes					YEs			Yes				No
	YM2151																															Yes										No					Yes			No
	Sega MAster System																												No										No					No			No				Yes
	NES																																No										No					Yes			No				Yes
	Game Boy																														No										Yes					No			Yes				Yes
	SID 6581																														No										Yes					No			No
	SID 8580																														No										Yes					No			No


*/
