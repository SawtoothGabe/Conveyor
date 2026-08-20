#pragma once

#include <cstdint>
#include <span>
#include <string_view>
#include <vector>
#include <Common/Types.hpp>

namespace conv
{
    class Packet
    {
    public:
        Packet(State state, int id);

        void WriteInt(int value);
        void WriteVarInt(int value);
        void WriteLong(int64_t value);
        void WriteFloat(float value);
        void WriteDouble(double value);
        void WriteString(std::string_view str);
        void WritePosition(int x, int y, int z);
        void WriteUInt16(uint16_t value);
        void WriteByteArray(std::span<const uint8_t> bytes);

        static size_t VarIntSize(int value);
        static size_t StringSize(std::string_view str);
        static uint8_t ReverseByte(uint8_t byte);

        void Reserve(size_t amount);
        uint8_t* GetData();
        const uint8_t* GetData() const;
        size_t GetSize() const;
        State GetState() const;
        int GetID() const;
    private:
        State m_state = State::HANDSHAKING;
        int m_id = 0;

        std::vector<uint8_t> m_data;
    };
}
