#pragma once

#include <Packets/Clientbound/Packet.hpp>

namespace conv::PacketBuilder
{
    Packet MakeStatusResponse();
    Packet MakePong(int64_t timestamp);
}
