#pragma once
#include <bits/types.h>
namespace comm::codes
{
    constexpr __uint16_t terminate = 0;
    constexpr __uint16_t handshake = 1;
    constexpr __uint16_t welcome = 2;
    constexpr __uint16_t tile = 3;
    constexpr __uint16_t chunk = 4;
}
