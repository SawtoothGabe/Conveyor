#include <Packets/Clientbound/PacketBuilder.hpp>

namespace conv::PacketBuilder
{
    Packet MakeStatusResponse()
    {
        Packet packet(State::STATUS, 0x0);
        packet.WriteString(R"({
    "version": {
        "name": "Conveyor Server",
        "protocol": 767
    },
    "players": {
        "max": 0,
        "online": -1,
        "sample": [
            {
                "name": "thinkofdeath",
                "id": "4566e69f-c907-48ee-8d71-d7ba5aa00d21"
            }
        ]
    },
    "description": {
        "text": "yo"
    },
    "favicon": "data:image/png;base64,<data>",
    "enforcesSecureChat": false
}
        )");

        return packet;
    }

    Packet MakePong(const int64_t timestamp)
    {
        Packet packet(State::STATUS, 0x1);
        packet.WriteLong(timestamp);

        return packet;
    }
}
