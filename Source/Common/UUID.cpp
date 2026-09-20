#include <format>
#include <random>
#include <Common/UUID.hpp>

namespace conv
{
    UUID::UUID()
    {
        std::random_device rd;
        std::mt19937_64 gen(rd());
        std::uniform_int_distribution<uint64_t> dist;

        high = dist(gen);
        low = dist(gen);
    }

    UUID::UUID(const uint64_t high, const uint64_t low)
        :
        high(high),
        low(low)
    {}

    std::string UUID::ToString() const
    {
        const auto highBytes = reinterpret_cast<const uint8_t*>(&high);
        const auto lowBytes = reinterpret_cast<const uint8_t*>(&low);

        return std::format("{:02x}{:02x}{:02x}{:02x}-{:02x}{:02x}-{:02x}{:02x}-{:02x}{:02x}-{:02x}{:02x}{:02x}{:02x}{:02x}{:02x}",
            highBytes[0],
            highBytes[1],
            highBytes[2],
            highBytes[3],
            highBytes[4],
            highBytes[5],
            highBytes[6],
            highBytes[7],
            lowBytes[0],
            lowBytes[1],
            lowBytes[2],
            lowBytes[3],
            lowBytes[4],
            lowBytes[5],
            lowBytes[6],
            lowBytes[7]
        );
    }
}
