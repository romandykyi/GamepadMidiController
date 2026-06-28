#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <concepts>
#include <cstdint>
#include <libremidi/libremidi.hpp>
#include "gamepad_scheme.h"

namespace gamepad_midi
{    
    template<typename t_receiver>
    concept input_receiver = requires(
        t_receiver r, 
        gamepad_button button_code, 
        button_state button_state, 
        gamepad_axis axis_code,
        int32_t axis_value
    ) {
        { r.on_button_event(button_code, button_state) } -> std::same_as<void>;
        { r.on_axis_event(axis_code, axis_value) } -> std::same_as<void>;
        { r.on_poll_end() } -> std::same_as<void>;
    };

    template <input_receiver t_receiver>
    class input_handler
    {
    private:
        t_receiver _eventReceiver;
        std::shared_ptr<gamepad::hook> _hook;

        int32_t quantize_axis_value(float value) const;
        
        void process_axis(uint16_t axis);

        void axis_handler(std::shared_ptr<gamepad::device> dev);
        void button_handler(std::shared_ptr<gamepad::device> dev);

    public:
        input_handler(t_receiver eventReceiver);
        
        void start_listening(void);
        void poll(void);
    };

}

#include "input_handler.tpp"

#endif // INPUT_HANDLER_H