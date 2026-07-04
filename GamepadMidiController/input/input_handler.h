#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <concepts>
#include <cstdint>
#include <libgamepad.hpp>
#include "gamepad_scheme.h"

namespace gamepad_midi
{
    /**
     * @brief Requires support for receiving button presses and releases.
     * 
     * @param button The gamepad button that was pressed/released.
     * @param state The state of the gamepad button (pressed/released).
     */
    template<typename T>
    concept has_on_button_event = requires(T r, gamepad_button button, button_state state) {
        { r.on_button_event(button, state) } -> std::same_as<void>;
    };

    /**
     * @brief Requires support for receiving analog axis movements.
     * 
     * @param axis The gamepad axis that is being moved.
     * @param value The updated axis value. Never negative for triggers.
     */
    template<typename T>
    concept has_on_axis_event = requires(T r, gamepad_axis axis, int32_t value) {
        { r.on_axis_event(axis, value) } -> std::same_as<void>;
    };

    /**
     * @brief Requires a notification callback for the end of the input polling cycle.
     */
    template<typename T>
    concept has_on_poll_end = requires(T r) {
        { r.on_poll_end() } -> std::same_as<void>;
    };

    /**
     * @brief A concept for a component that receives input from a gamepad.
     */
    template<typename t_receiver>
    concept input_receiver = 
        has_on_button_event<t_receiver> && 
        has_on_axis_event<t_receiver> && 
        has_on_poll_end<t_receiver>;

    template <input_receiver t_receiver>
    class input_handler
    {
    private:
        t_receiver& _eventReceiver;
        std::shared_ptr<gamepad::hook> _hook;

        static int32_t quantize_axis_value(float value);
        
        void process_axis(uint16_t axis, gamepad_axis out_axis);

        void button_handler(std::shared_ptr<gamepad::device> dev);

    public:
        input_handler(t_receiver& eventReceiver);
        
        void start_listening(void);
        void poll(void);
    };
}

#include "input_handler.tpp"

#endif // INPUT_HANDLER_H