#include <Connection.hpp>
#include <Packets/PacketDataStream.hpp>
#include <Packets/PacketHandlerRegistry.hpp>

namespace conv
{
    Connection::Connection(asio::io_context& context)
        :
        m_context(context),
        m_socket(context)
    {
        m_currentPacketData.reserve(READ_BLOCK_SIZE);
    }

    tcp::socket& Connection::GetSocket()
    {
        return m_socket;
    }

    void Connection::Start()
    {
        MakeRead();
    }

    void Connection::MakeRead()
    {
        m_socket.async_read_some(asio::buffer(m_data, READ_BLOCK_SIZE),
        [this](const std::error_code ec, const size_t length)
        {
           if (!ec)
               if (OnRead(length))
                   MakeRead();
        });
    }

    bool Connection::OnRead(const size_t bytesRead)
    {
        for (size_t i = 0; i < bytesRead; i++)
        {
            const uint8_t byte = m_data[i];

            if (!m_lengthRead)
            {
                m_decodedLength |= (byte & 0b01111111) << m_lengthBitOffset;

                if (m_lengthBitOffset == 35)
                    return false;

                m_lengthBitOffset += 7;
                m_lengthRead = (byte & 0b10000000) == 0;
            }
            else
            {
                m_currentPacketData.push_back(byte);

                if (m_currentPacketData.size() >= m_decodedLength)
                    FinishReading();
            }
        }

        return true;
    }

    void Connection::FinishReading()
    {
        m_decodedLength = 0;
        m_lengthRead = false;
        m_lengthBitOffset = 0;

        PacketDataStream stream(m_currentPacketData);
        int id = stream.ReadVarInt();

        PacketHandlerRegistry::Get().Dispatch(m_state, id,
            *this, PacketDataStream(std::span(
                m_currentPacketData.begin() + stream.GetCurrentIndex(),
                m_currentPacketData.end())));

        m_currentPacketData.clear();
    }

    void Connection::Disconnect()
    {
        try
        {
            m_socket.close();
        }
        catch (const std::exception& e)
        {
            LOG_ERROR("{}", e.what());
        }
    }

    void Connection::SetState(const State state)
    {
        m_state = state;
    }
}
