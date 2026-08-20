#include <Connection.hpp>
#include <Packets/Serverbound/PacketDefinitions.hpp>

namespace conv
{
    void StatusRequest::HandlePacket(Connection& connection, PacketDataStream stream)
    {
        connection.SendPacket(PacketBuilder::MakeStatusResponse());
    }
}
