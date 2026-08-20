#pragma once

#include <Packets/PacketDataStream.hpp>

namespace conv
{
    class Connection;

    class ServerboundPacketHandler
    {
    public:
        virtual ~ServerboundPacketHandler() = default;

        virtual void HandlePacket(Connection& connection,
            PacketDataStream stream) = 0;
    };
}
