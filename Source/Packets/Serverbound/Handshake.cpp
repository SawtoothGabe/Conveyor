#include <Connection.hpp>
#include <IO/Logger.hpp>

#include <Packets/Serverbound/PacketDefinitions.hpp>

namespace conv
{
    void Handshake::HandlePacket(Connection& connection,
        PacketDataStream stream)
    {
        int protocolVersion = stream.ReadVarInt();
        std::string serverAddress = stream.ReadString();
        uint16_t port = stream.ReadUInt16();
        int intent = stream.ReadVarInt();

        LOG_INFO("Handshake received:");
        LOG_INFO("\tprotocolVersion = {}", protocolVersion);
        LOG_INFO("\tserverAddress = \"{}\"", serverAddress);
        LOG_INFO("\tport = {}", port);
        LOG_INFO("\tintent = {}", intent);

        switch (intent)
        {
            case 1: connection.SetState(State::STATUS); break;
            default: connection.SetState(State::LOGIN);
        }
    }
}
