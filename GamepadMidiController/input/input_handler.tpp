#include <utility>

namespace gamepad_midi
{
    template <input_receiver t_receiver>
    input_handler<t_receiver>::input_handler(t_receiver eventReceiver) : _eventReceiver(std::move(eventReceiver)) {}

    template <input_receiver t_receiver>
    void input_handler<t_receiver>::start_listening(void) 
    {

    }

    template <input_receiver t_receiver>
    void input_handler<t_receiver>::poll(void) 
    {
        _eventReceiver.on_button_event(67, button_state::RELEASED);
    }
}