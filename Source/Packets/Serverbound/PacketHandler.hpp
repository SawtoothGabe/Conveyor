#pragma once

#include <Packets/Serverbound/PacketDataStream.hpp>

namespace conv
{
    class Connection;

    class PacketHandler
    {
    public:
        virtual ~PacketHandler() = default;

        virtual void HandlePacket(Connection& connection,
            PacketDataStream stream) = 0;
    };
}
