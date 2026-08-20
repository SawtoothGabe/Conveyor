#include <Packets/Serverbound/PacketHandlerRegistry.hpp>

namespace conv
{
    void PacketHandlerRegistry::Register(const State state, const int packetID,
        std::unique_ptr<PacketHandler> handler)
    {
        m_handlers[state][packetID].emplace_back(std::move(handler));
    }

    bool PacketHandlerRegistry::Dispatch(const State state, const int packetID,
        Connection& connection, const PacketDataStream& data) const
    {
        if (!m_handlers.contains(state))
            return false;

        const auto& packets = m_handlers.at(state);

        if (!packets.contains(packetID))
            return false;

        auto& handlers = packets.at(packetID);
        for (const auto& handler : handlers)
            handler->HandlePacket(connection, data);

        return !handlers.empty();
    }

    PacketHandlerRegistry& PacketHandlerRegistry::Get()
    {
        static PacketHandlerRegistry registry;
        return registry;
    }
}
