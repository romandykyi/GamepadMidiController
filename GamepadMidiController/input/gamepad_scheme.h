#ifndef BUTTON_STATE_H
#define BUTTON_STATE_H

#include <cstdint>

namespace gamepad_midi
{
    enum class button_state : std::uint8_t 
    {
        PRESSED,
        RELEASED
    };

    enum class gamepad_button : std::uint8_t
    {
        NORTH,
        EAST,
        SOUTH,
        WEST,
        LT,
        LB,
        RT,
        RB,
        DPAD_UP,
        DPAD_RIGHT,
        DPAD_DOWN,
        DPAD_LEFT,
        START,
        BACK,
        LSB,
        RSB
    };

    enum class gamepad_stick : std::uint8_t
    {
        LEFT,
        RIGHT
    };

    enum class gamepad_axis : std::uint8_t
    {
        LS_X,
        LS_Y,
        RS_X,
        RS_Y,
        RT,
        LT
    };
}

#endif // BUTTON_STATE_H