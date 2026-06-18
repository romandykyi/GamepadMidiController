#ifndef BUTTON_STATE_H
#define BUTTON_STATE_H

#include <cstdint>

namespace gamepad_midi
{
    enum class button_state : std::uint8_t {
        PRESSED,
        RELEASED
    };
}

#endif // BUTTON_STATE_H