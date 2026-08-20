#pragma once

#include <Common/Asio.hpp>
#include <Common/Types.hpp>

namespace conv
{
    class Connection
    {
    public:
        explicit Connection(asio::io_context& context);

        void Start();
        void Disconnect();

        void SetState(State state);

        tcp::socket& GetSocket();
    private:
        static constexpr size_t READ_BLOCK_SIZE = 512;

        void MakeRead();
        bool OnRead(size_t bytesRead);
        void FinishReading();

        asio::io_context& m_context;
        tcp::socket m_socket;

        std::array<uint8_t, READ_BLOCK_SIZE> m_data{};
        std::vector<uint8_t> m_currentPacketData;

        bool m_lengthRead = false;
        int m_decodedLength = 0;
        int m_lengthBitOffset = 0;

        State m_state = State::HANDSHAKING;
    };
}
