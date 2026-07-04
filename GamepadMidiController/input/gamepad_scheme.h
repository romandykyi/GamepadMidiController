#ifndef BUTTON_STATE_H
#define BUTTON_STATE_H

#include <cstdint>

namespace gamepad_midi
{
    /**
     * @brief An enum that represents the gamepad button state.
     */
    enum class button_state : std::uint8_t 
    {
        /**
         * @brief The pressed button state.
         */
        PRESSED,
        /**
         * @brief The released button state.
         */
        RELEASED
    };

    /**
     * @brief An enum that represents the gamepad buttons.
     */
    enum class gamepad_button : std::uint8_t
    {
        /**
         * @brief The north button on a gamepad (e.g. Y on Xbox).
         */
        NORTH,
        /**
         * @brief The east button on a gamepad (e.g. B on Xbox).
         */
        EAST,
        /**
         * @brief The south button on a gamepad (e.g. A on Xbox).
         */
        SOUTH,
        /**
         * @brief The west button on a gamepad (e.g. X on Xbox).
         */
        WEST,
        /**
         * @brief The left trigger on a gamepad.
         */
        LT,
        /**
         * @brief The left bumper on a gamepad.
         */
        LB,
        /**
         * @brief The right trigger on a gamepad.
         */
        RT,
        /**
         * @brief The right bumper on a gamepad.
         */
        RB,
        /**
         * @brief The DPAD up button on a gamepad.
         */
        DPAD_UP,
        /**
         * @brief The DPAD right button on a gamepad.
         */
        DPAD_RIGHT,
        /**
         * @brief The DPAD down button on a gamepad.
         */
        DPAD_DOWN,
        /**
         * @brief The DPAD left button on a gamepad.
         */
        DPAD_LEFT,
        /**
         * @brief The start button on a gamepad (or sometimes menu).
         */
        START,
        /**
         * @brief The select button on a gamepad (or sometimes share).
         */
        SELECT,
        /**
         * @brief The left stick button on a gamepad.
         */
        LSB,
        /**
         * @brief The right stick button on a gamepad.
         */
        RSB,
        /**
         * @brief The number of gamepad buttons.
         */
        COUNT // Must be the last element
    };
    /**
     * @brief The number of gamepad buttons.
     */
    inline constexpr size_t GAMEPAD_BUTTONS_NUM = static_cast<size_t>(gamepad_button::COUNT);

    /**
     * @brief An enum that represents the gamepad sticks.
     */
    enum class gamepad_stick : std::uint8_t
    {
        /**
         * @brief The left stick.
         */
        LEFT,
        /**
         * @brief The right stick.
         */
        RIGHT,
        /**
         * @brief The number of gamepad sticks.
         */
        COUNT // Must be the last element
    };
    /**
     * @brief The number of gamepad sticks.
     */
    inline constexpr size_t GAMEPAD_STICKS_NUM = static_cast<size_t>(gamepad_stick::COUNT);

    /**
     * @brief An enum that represents the gamepad axes.
     */
    enum class gamepad_axis : std::uint8_t
    {
        /**
         * @brief The left stick X axis.
         */
        LS_X,
        /**
         * @brief The left stick Y axis.
         */
        LS_Y,
        /**
         * @brief The right stick X axis.
         */
        RS_X,
        /**
         * @brief The right stick Y axis.
         */
        RS_Y,
        /**
         * @brief The right trigger axis.
         */
        RT,
        /**
         * @brief The left trigger Y axis.
         */
        LT,
        /**
         * @brief The number of gamepad axes.
         */
        COUNT // Must be the last element
    };
    /**
     * @brief The number of gamepad axes.
     */
    inline constexpr size_t GAMEPAD_AXES_NUM = static_cast<size_t>(gamepad_axis::COUNT);
}

#endif // BUTTON_STATE_H