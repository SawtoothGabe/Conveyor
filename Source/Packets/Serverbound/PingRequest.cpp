#include <Connection.hpp>
#include <Packets/Serverbound/PacketDefinitions.hpp>

namespace conv
{
    void PingRequest::HandlePacket(Connection& connection, PacketDataStream stream)
    {
        connection.SendPacket(PacketBuilder::MakePong(stream.ReadLong()));
    }
}
