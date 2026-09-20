#include <Connection.hpp>
#include <Packets/Serverbound/PacketHandlerRegistry.hpp>

namespace conv
{
    DEFINE_SIMPLE_PACKET_HANDLER(State::STATUS, 0x1, PingRequest);

    void PingRequest::HandlePacket(Connection& connection, PacketDataStream stream)
    {
        connection.SendPacket(PacketBuilder::MakePong(stream.ReadLong()));
    }
}
