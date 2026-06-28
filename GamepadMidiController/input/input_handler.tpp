#ifndef INPUT_HANDLER_H // Hint for IDEs
#include "input_handler.h"
#endif

#include <algorithm>
#include <utility>

namespace gamepad_midi
{
    template <input_receiver t_receiver>
    input_handler<t_receiver>::input_handler(t_receiver& eventReceiver) : _eventReceiver(eventReceiver) {}

    template <input_receiver t_receiver>
    int32_t input_handler<t_receiver>::quantize_axis_value(float value)
    {
        return value > 0
            ? static_cast<int32_t>(static_cast<double>(std::numeric_limits<int32_t>::max()) * value)
            : static_cast<int32_t>(static_cast<double>(std::numeric_limits<int32_t>::min()) * -value);
    }

    template <input_receiver t_receiver>
    void input_handler<t_receiver>::process_axis(uint16_t axis, gamepad_axis out_axis)
    {
        float value = std::clamp(_hook->get_devices()[0]->get_axis(axis), -1.0f, 1.0f);
        int32_t quant_value = quantize_axis_value(value);
        
        _eventReceiver.on_axis_event(
            out_axis,
            quant_value
        );
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
        _hook->set_button_event_handler([this] (std::shared_ptr<gamepad::device> dev) { this->button_handler(dev); });

        // TODO: add better error handling
        if (!_hook->start()) 
        {
            std::cerr << "Failed to start hook" << std::endl;
            return;
        }
    }

    template <input_receiver t_receiver>
    void input_handler<t_receiver>::poll(void) 
    {
        process_axis(gamepad::axis::LEFT_TRIGGER, gamepad_axis::LT);
        process_axis(gamepad::axis::RIGHT_TRIGGER, gamepad_axis::RT);
        process_axis(gamepad::axis::LEFT_STICK_X, gamepad_axis::LS_X);
        process_axis(gamepad::axis::LEFT_STICK_Y, gamepad_axis::LS_Y);
        process_axis(gamepad::axis::RIGHT_STICK_X, gamepad_axis::RS_X);
        process_axis(gamepad::axis::RIGHT_STICK_Y, gamepad_axis::RS_Y);

        _eventReceiver.on_poll_end();
    }
}