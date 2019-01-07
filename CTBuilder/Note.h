#pragma once
#include <vector>

class Note
{
public:

	enum class Id
	{
		Empty,		//	Does not change the notes (effects are still processed)
		Off,		//	Turns off all notes and effects for this channel until the next non-empty non-off note is encountered.
		C_Sharp,
		D,
		D_Sharp,
		E,
		F,
		F_Sharp,
		G,
		G_Sharp,
		A,
		A_Sharp,
		B,
		C,
	};

	Note(Id id, unsigned int octave, unsigned int volume);


protected:

	Id				m_Id;
	unsigned int	m_Octave;
	unsigned int	m_Volume;
};
