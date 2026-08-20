#pragma once

#include <memory>
#include <unordered_map>
#include <vector>
#include <Common/Types.hpp>
#include <Packets/ServerboundPacketHandler.hpp>

#define REGISTER_PACKET_HANDLER(state, id, name, ...) \
    static const uint8_t _name##_registrar = \
        (PacketHandlerRegistry::Get().Register(state, id, \
            std::make_unique<name>(__VA_ARGS__)), 0)

namespace conv
{
    class PacketHandlerRegistry
    {
    public:
        void Register(State state, int packetID,
            std::unique_ptr<ServerboundPacketHandler> handler);

        void Dispatch(State state, int packetID, Connection& connection,
            const PacketDataStream& data) const;

        static PacketHandlerRegistry& Get();
    private:
        std::unordered_map<State,
            std::unordered_map<int,
                std::vector<std::unique_ptr<ServerboundPacketHandler>>>>
                    m_handlers;
    };
}
