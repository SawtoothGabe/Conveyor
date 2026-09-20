#pragma once
#include <cstdint>
#include <string>

namespace conv
{
    struct UUID
    {
        UUID();
        UUID(uint64_t high, uint64_t low);

        std::string ToString() const;

        uint64_t high;
        uint64_t low;
    };
}
