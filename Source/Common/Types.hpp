#pragma once

namespace conv
{
    enum class State
    {
        HANDSHAKING,
        STATUS,
        LOGIN,
        CONFIGURATION,
        PLAY
    };
}