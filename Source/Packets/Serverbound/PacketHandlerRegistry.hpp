#pragma once

#include <memory>
#include <unordered_map>
#include <vector>
#include <Common/Types.hpp>
#include <Packets/Serverbound/PacketHandler.hpp>

#define REGISTER_PACKET_HANDLER(state, id, name, ...) \
    static const uint8_t _##name##_registrar = \
        (PacketHandlerRegistry::Get().Register(state, id, \
            std::make_unique<name>(__VA_ARGS__)), 0)

#define DEFINE_SIMPLE_PACKET_HANDLER(name) \
    class name : public PacketHandler \
    { \
    public: \
        virtual void HandlePacket(Connection& connection, \
            PacketDataStream stream) override; \
    };

namespace conv
{
    class PacketHandlerRegistry
    {
    public:
        void Register(State state, int packetID,
            std::unique_ptr<PacketHandler> handler);

        void Dispatch(State state, int packetID, Connection& connection,
            const PacketDataStream& data) const;

        static PacketHandlerRegistry& Get();
    private:
        std::unordered_map<State,
            std::unordered_map<int,
                std::vector<std::unique_ptr<PacketHandler>>>>
                    m_handlers;
    };
}
