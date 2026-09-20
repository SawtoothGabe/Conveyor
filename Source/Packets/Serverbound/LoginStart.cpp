#include <Packets/Serverbound/PacketHandlerRegistry.hpp>
#include <IO/Logger.hpp>

namespace conv
{
    DEFINE_SIMPLE_PACKET_HANDLER(State::LOGIN, 0x00, LoginStart);

    void LoginStart::HandlePacket(Connection& connection, PacketDataStream stream)
    {
        std::string username = stream.ReadString();
        const UUID uuid = stream.ReadUUID();

        LOG_INFO("Player \"{}\" connecting with uuid {}", username,
            uuid.ToString());


    }
}
