#include <Packets/Clientbound/Packet.hpp>

namespace conv
{
	Packet::Packet(const State state, const int id)
		:
		m_state(state),
		m_id(id)
	{}

	void Packet::WriteInt(int value)
	{
		const uint32_t intValue = *reinterpret_cast<uint32_t*>(&value);
		m_data.push_back(static_cast<uint8_t>((intValue >> (8 * 3)) & 0xFF));
		m_data.push_back(static_cast<uint8_t>((intValue >> (8 * 2)) & 0xFF));
		m_data.push_back(static_cast<uint8_t>((intValue >> (8 * 1)) & 0xFF));
		m_data.push_back(static_cast<uint8_t>(intValue & 0xFF));
	}

	void Packet::WriteVarInt(const int value)
	{
		uint32_t uValue = static_cast<uint32_t>(value);

		do
		{
			uint8_t currentByte = static_cast<uint8_t>(uValue & 0b01111111);

			uValue >>= 7;
			if (uValue != 0)
				currentByte |= 0b10000000;

			m_data.push_back(currentByte);
		}
		while (uValue != 0);
	}

	void Packet::WriteLong(const int64_t value)
	{
		m_data.push_back(static_cast<uint8_t>((value >> (8 * 7)) & 0xFF));
		m_data.push_back(static_cast<uint8_t>((value >> (8 * 6)) & 0xFF));
		m_data.push_back(static_cast<uint8_t>((value >> (8 * 5)) & 0xFF));
		m_data.push_back(static_cast<uint8_t>((value >> (8 * 4)) & 0xFF));
		m_data.push_back(static_cast<uint8_t>((value >> (8 * 3)) & 0xFF));
		m_data.push_back(static_cast<uint8_t>((value >> (8 * 2)) & 0xFF));
		m_data.push_back(static_cast<uint8_t>((value >> (8 * 1)) & 0xFF));
		m_data.push_back(static_cast<uint8_t>(value & 0xFF));
	}

	void Packet::WriteFloat(float value)
	{
		const auto intValue = *reinterpret_cast<uint32_t*>(&value);
		m_data.push_back(static_cast<uint8_t>((intValue >> (8 * 3)) & 0xFF));
		m_data.push_back(static_cast<uint8_t>((intValue >> (8 * 2)) & 0xFF));
		m_data.push_back(static_cast<uint8_t>((intValue >> (8 * 1)) & 0xFF));
		m_data.push_back(static_cast<uint8_t>(intValue & 0xFF));
	}

	void Packet::WriteDouble(double value)
	{
		const auto intValue = *reinterpret_cast<uint64_t*>(&value);
		m_data.push_back(static_cast<uint8_t>((intValue >> (8 * 7)) & 0xFF));
		m_data.push_back(static_cast<uint8_t>((intValue >> (8 * 6)) & 0xFF));
		m_data.push_back(static_cast<uint8_t>((intValue >> (8 * 5)) & 0xFF));
		m_data.push_back(static_cast<uint8_t>((intValue >> (8 * 4)) & 0xFF));
		m_data.push_back(static_cast<uint8_t>((intValue >> (8 * 3)) & 0xFF));
		m_data.push_back(static_cast<uint8_t>((intValue >> (8 * 2)) & 0xFF));
		m_data.push_back(static_cast<uint8_t>((intValue >> (8 * 1)) & 0xFF));
		m_data.push_back(static_cast<uint8_t>(intValue & 0xFF));
	}

	void Packet::WriteString(const std::string_view str)
	{
		WriteVarInt(str.size());

		for (int i = 0; i < str.size(); i++)
			m_data.push_back(static_cast<uint8_t>(str[i]));
	}

	size_t Packet::VarIntSize(const int value)
	{
		if (value < 0)
			return 5;

		size_t size = 1;
		size += value > 0x7F;
		size += value > 0x3FFF;
		size += value > 0x1FFFFF;
		size += value > 0xFFFFFFF;

		return size;
	}

	size_t Packet::StringSize(const std::string_view str)
	{
		return str.size() + VarIntSize(str.size());
	}

	void Packet::WritePosition(const int x, const int y, const int z)
	{
		WriteLong(
			(static_cast<int64_t>(x) & 0x3FFFFFF) << 38 |
			(static_cast<int64_t>(y) & 0xFFF) << 26 |
			static_cast<int64_t>(z) & 0x3FFFFFF);
	}

	void Packet::WriteUInt16(const uint16_t value)
	{
		m_data.push_back(static_cast<uint8_t>((value >> (8 * 1)) & 0xFF));
		m_data.push_back(static_cast<uint8_t>(value & 0xFF));
	}

	void Packet::WriteByteArray(std::span<const uint8_t> bytes)
	{
		m_data.append_range(bytes);
	}

	uint8_t Packet::ReverseByte(const uint8_t byte)
	{
		uint8_t reversedByte = 0;
		for (uint8_t i = 0; i < 8; i++)
			reversedByte |= ((byte && (1 << i)) >> i) << (7 - i);

		return reversedByte;
	}

	void Packet::Reserve(const size_t amount)
	{
		m_data.reserve(amount);
	}

	uint8_t* Packet::GetData()
	{
		return m_data.data();
	}

	const uint8_t* Packet::GetData() const
	{
		return m_data.data();
	}

	size_t Packet::GetSize() const
	{
		return m_data.size();
	}

	State Packet::GetState() const
	{
		return m_state;
	}

	int Packet::GetID() const
	{
		return m_id;
	}
}
