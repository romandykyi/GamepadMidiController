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
        RSB,
        COUNT // Must be the last element
    };
    inline constexpr size_t GAMEPAD_BUTTONS_NUM = static_cast<size_t>(gamepad_button::COUNT);

    enum class gamepad_stick : std::uint8_t
    {
        LEFT,
        RIGHT,
        COUNT // Must be the last element
    };
    inline constexpr size_t GAMEPAD_STICKS_NUM = static_cast<size_t>(gamepad_stick::COUNT);

    enum class gamepad_axis : std::uint8_t
    {
        LS_X,
        LS_Y,
        RS_X,
        RS_Y,
        RT,
        LT,
        COUNT // Must be the last element
    };
    inline constexpr size_t GAMEPAD_AXES_NUM = static_cast<size_t>(gamepad_axis::COUNT);
}

#endif // BUTTON_STATE_H