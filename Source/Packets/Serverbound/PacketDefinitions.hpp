#pragma once

#include <Packets/Serverbound/PacketHandlerRegistry.hpp>

namespace conv
{
    DEFINE_SIMPLE_PACKET_HANDLER(Handshake);
    DEFINE_SIMPLE_PACKET_HANDLER(StatusRequest);
}