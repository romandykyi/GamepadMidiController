#include <utility>
namespace gamepad_midi
{
    template <midi_out t_midi_out>
    g2m_processor<t_midi_out>::g2m_processor(t_midi_out midi_out) : _midi_out(std::move(midi_out)) {}

    template <midi_out t_midi_out>
    void g2m_processor<t_midi_out>::on_button_event(gamepad_button button_code, button_state button_state)
    {

    }
    
    template <midi_out t_midi_out>
    void g2m_processor<t_midi_out>::on_axis_event(gamepad_axis axis_code, int16_t axis_value)
    {
    }
}