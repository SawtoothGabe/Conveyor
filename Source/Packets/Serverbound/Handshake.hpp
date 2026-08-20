#pragma once

#include <Packets/PacketHandlerRegistry.hpp>
#include <Packets/ServerboundPacketHandler.hpp>

namespace conv
{
    class Handshake : public ServerboundPacketHandler
    {
    public:
        void HandlePacket(Connection& connection,
            PacketDataStream stream) override;
    };

    REGISTER_PACKET_HANDLER(State::HANDSHAKING, 0x0, Handshake);
}
