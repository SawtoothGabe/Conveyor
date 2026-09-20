#include <Connection.hpp>
#include <Packets/Serverbound/PacketHandlerRegistry.hpp>

namespace conv
{
    DEFINE_SIMPLE_PACKET_HANDLER(State::STATUS, 0x0, StatusRequest);

    void StatusRequest::HandlePacket(Connection& connection, PacketDataStream stream)
    {
        connection.SendPacket(PacketBuilder::MakeStatusResponse());
    }
}
