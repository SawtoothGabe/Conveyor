#pragma once

#include <cstdint>
#include <span>

namespace conv
{
    class PacketDataStream
    {
    public:
        explicit PacketDataStream(std::span<uint8_t> data);

        int ReadVarInt();
        uint16_t ReadUInt16();
        double ReadDouble();
        int64_t ReadLong();
        std::string ReadString();
        std::string ReadStringWithSize(size_t size);

        void Seek(size_t position);
        size_t GetCurrentIndex() const;
        std::span<uint8_t> GetData() const;
    private:
        std::span<uint8_t> m_data;
        size_t m_currentIndex = 0;
    };
}
