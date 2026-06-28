#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <concepts>
#include <cstdint>
#include "gamepad_scheme.h"

namespace gamepad_midi
{    
    template<typename t_receiver>
    concept input_receiver = requires(
        t_receiver r, 
        gamepad_button button_code, 
        button_state button_state, 
        gamepad_axis axis_code,
        int16_t axis_value
    ) {
        { r.on_button_event(button_code, button_state) } -> std::same_as<void>;
        { r.on_axis_event(axis_code, axis_value)} -> std::same_as<void>;
    };

    template <input_receiver t_receiver>
    class input_handler
    {
    private:
        t_receiver _eventReceiver;

    public:
        input_handler(t_receiver eventReceiver);
        
        void start_listening(void);
        void poll(void);
    };

}

#include "input_handler.tpp"

#endif // INPUT_HANDLER_H