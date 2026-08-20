#include <string>
#include <Packets/PacketDataStream.hpp>

namespace conv
{
    PacketDataStream::PacketDataStream(const std::span<uint8_t> data)
        :
        m_data(data)
    {}

    int PacketDataStream::ReadVarInt()
    {
        int decodedInt = 0;
        int bitOffset = 0;
        uint8_t currentByte;
        int bytesRead = 0;

        do
        {
            if (m_currentIndex + bytesRead >= m_data.size())
                return decodedInt;

            currentByte = m_data[m_currentIndex + bytesRead];
            decodedInt |= (currentByte & 0b01111111) << bitOffset;

            if (bitOffset == 35)
                return -1;

            bitOffset += 7;

            bytesRead++;
        }
        while ((currentByte & 0b10000000) != 0);

        m_currentIndex += bytesRead;
        return decodedInt;
    }

    uint16_t PacketDataStream::ReadUInt16()
    {
        if (m_currentIndex + sizeof(uint16_t) >= m_data.size())
            return 0.0;

        uint16_t value;
        const auto doubleIter = reinterpret_cast<uint8_t*>(&value);

        doubleIter[1] = m_data[m_currentIndex];
        doubleIter[0] = m_data[m_currentIndex + 1];

        m_currentIndex += sizeof(uint16_t);
        return value;
    }

    double PacketDataStream::ReadDouble()
    {
        if (m_currentIndex + sizeof(double) >= m_data.size())
            return 0.0;

        double value;
        const auto doubleIter = reinterpret_cast<uint8_t*>(&value);

        doubleIter[7] = m_data[m_currentIndex];
        doubleIter[6] = m_data[m_currentIndex + 1];
        doubleIter[5] = m_data[m_currentIndex + 2];
        doubleIter[4] = m_data[m_currentIndex + 3];
        doubleIter[3] = m_data[m_currentIndex + 4];
        doubleIter[2] = m_data[m_currentIndex + 5];
        doubleIter[1] = m_data[m_currentIndex + 6];
        doubleIter[0] = m_data[m_currentIndex + 7];

        m_currentIndex += sizeof(double);
        return value;
    }

    int64_t PacketDataStream::ReadLong()
    {
        if (m_currentIndex + sizeof(int64_t) >= m_data.size())
            return 0.0;

        int64_t value;
        const auto longIter = reinterpret_cast<uint8_t*>(&value);

        longIter[7] = m_data[m_currentIndex];
        longIter[6] = m_data[m_currentIndex + 1];
        longIter[5] = m_data[m_currentIndex + 2];
        longIter[4] = m_data[m_currentIndex + 3];
        longIter[3] = m_data[m_currentIndex + 4];
        longIter[2] = m_data[m_currentIndex + 5];
        longIter[1] = m_data[m_currentIndex + 6];
        longIter[0] = m_data[m_currentIndex + 7];

        m_currentIndex += sizeof(int64_t);
        return value;
    }

    std::string PacketDataStream::ReadStringWithSize(const size_t size)
    {
        std::string str;
        str.insert(str.end(), m_data.begin() + m_currentIndex,
            m_data.begin() + std::min(m_currentIndex + size, m_data.size()));

        m_currentIndex += size;
        return str;
    }

    std::string PacketDataStream::ReadString()
    {
        const int length = ReadVarInt();
        return ReadStringWithSize(length);
    }

    void PacketDataStream::Seek(size_t position)
    {
        m_currentIndex = position;
    }

    size_t PacketDataStream::GetCurrentIndex() const
    {
        return m_currentIndex;
    }

    std::span<uint8_t> PacketDataStream::GetData() const
    {
        return m_data;
    }
}
