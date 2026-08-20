#include <Packets/Serverbound/PacketDefinitions.hpp>

namespace conv
{
    REGISTER_PACKET_HANDLER(State::HANDSHAKING, 0x0, Handshake);
    REGISTER_PACKET_HANDLER(State::STATUS, 0x0, StatusRequest);
}
