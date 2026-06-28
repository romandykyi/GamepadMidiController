#ifndef INPUT_HANDLER_H // Hint for IDEs
#include "input_handler.h"
#endif

#include <algorithm>
#include <utility>

namespace gamepad_midi
{
    template <input_receiver t_receiver>
    input_handler<t_receiver>::input_handler(t_receiver eventReceiver) : _eventReceiver(std::move(eventReceiver)) {}

    template <input_receiver t_receiver>
    int32_t input_handler<t_receiver>::quantize_axis_value(float value) const
    {
        return value > 0
            ? static_cast<int32_t>(static_cast<double>(std::numeric_limits<int32_t>::max()) * value)
            : static_cast<int32_t>(static_cast<double>(std::numeric_limits<int32_t>::min()) * -value);
    }

    template <input_receiver t_receiver>
    void input_handler<t_receiver>::process_axis(uint16_t axis)
    {
        float value = std::clamp(_hook->get_devices()[0]->get_axis(axis), -1, 1);
        int32_t quant_value = quantize_axis_value(value);
        
        _eventReceiver.on_axis_event(axis, quant_value);
    }

    template <input_receiver t_receiver>
    void input_handler<t_receiver>::axis_handler(std::shared_ptr<gamepad::device> dev)
    {
        auto event = dev->last_axis_event();

        gamepad_axis axis;
        switch (event->vc)
        {
            case gamepad::axis::RIGHT_TRIGGER:
                axis = gamepad_axis::RT;
                break;
            case gamepad::axis::LEFT_TRIGGER:
                axis = gamepad_axis::LT;
                break;
            default:
                return;
        }
        float value = std::clamp(event->virtual_value, -1.0, 1.0);
        int32_t quant_value = quantize_axis_value(value);

        _eventReceiver.on_axis_event(axis, quant_value);
    }

    template <input_receiver t_receiver>
    void input_handler<t_receiver>::button_handler(std::shared_ptr<gamepad::device> dev)
    {
        auto event = dev->last_button_event();

        gamepad_button button;
        switch (event->vc)
        {
            case gamepad::button::Y:
                button = gamepad_button::NORTH;
                break;
            case gamepad::button::B:
                button = gamepad_button::EAST;
                break;
            case gamepad::button::A:
                button = gamepad_button::SOUTH;
                break;
            case gamepad::button::X:
                button = gamepad_button::WEST;
                break;
            default:
                return;
        }
        
        _eventReceiver.on_button_event(
            button, 
            event->virtual_value > 0.5 ? button_state::PRESSED : button_state::RELEASED
        );
    }

    template <input_receiver t_receiver>
    void input_handler<t_receiver>::start_listening(void) 
    {
        _hook = gamepad::hook::make();
	    _hook->set_plug_and_play(true, gamepad::ms(1000));
        _hook->set_axis_event_handler(axis_handler);
        _hook->set_button_event_handler(button_handler);
    }

    template <input_receiver t_receiver>
    void input_handler<t_receiver>::poll(void) 
    {
        if (_hook == nullptr) return;

        process_axis(gamepad::axis::LEFT_STICK_X);
        process_axis(gamepad::axis::LEFT_STICK_Y);
        process_axis(gamepad::axis::RIGHT_STICK_X);
        process_axis(gamepad::axis::RIGHT_STICK_Y);

        _eventReceiver.on_poll_end();
    }
}