#pragma once
#include <vector>


class BinaryParser
{
public:
	using container_type = std::vector<unsigned char>;
	using iterator = container_type::iterator;
	using const_iterator = container_type::const_iterator;

	BinaryParser(std::string filename, iterator begin, iterator end)
		:
		m_Filename(filename),
		m_Begin(begin),
		m_End(end),
		m_Current(begin)
	{}

	unsigned char ReadByte()
	{
		unsigned char value = 0;

		if (m_Current != m_End)
		{
			value = *(m_Current++);
		}

		return value;
	}

	template<class Type_>
	Type_ ReadValue()
	{
		Type_ value = 0;

		for(auto i(0); m_Current != m_End && i < sizeof(value); ++i)
		{
			value |= *(m_Current++) << (i * 8);
		}

		return value;
	}



	std::string ReadString(unsigned int count)
	{
		std::string str;
		str.reserve(count);

		while (count--)
		{
			str.push_back(ReadByte());
		}

		str.erase(std::find(str.begin(), str.end(), 0), str.end());

		return str;
	}

	std::string ReadString()
	{
		return ReadString(ReadByte());
	}

	template<class Type_>
	std::vector<Type_> ReadArray(unsigned int count)
	{
		std::vector<Type_> buffer;
		buffer.reserve(count);

		while (count--)
		{
			buffer.push_back(ReadValue<Type_>());
		}

		return buffer;
	}

	using signed_quad_type = std::int32_t;
	using quad_type = std::uint32_t;
	using word_type = std::uint16_t;

	std::string GetFilename() const
	{
		return m_Filename;
	}

	quad_type ReadQuad()
	{
		return ReadValue<quad_type>();
	}

	word_type ReadWord()
	{
		return ReadValue<word_type>();
	}

	std::vector<quad_type> ReadQuadArray()
	{
		return ReadArray<quad_type>(ReadByte());
	}

	std::vector<signed_quad_type> ReadSignedQuadArray()
	{
		return ReadArray<signed_quad_type>(ReadByte());
	}

	std::vector<quad_type> ReadQuadArrayEx()
	{
		return ReadArray<quad_type>(ReadQuad());
	}


private:

	std::string m_Filename;
	iterator	m_Begin;
	iterator	m_End;
	iterator	m_Current;

};
