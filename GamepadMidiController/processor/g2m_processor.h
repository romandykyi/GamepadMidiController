#ifndef G2M_PROCESSOR_H
#define G2M_PROCESSOR_H

#include <cstdint>
#include "button_state.h"

namespace gamepad_midi
{
    template<typename t_midi_out>
    concept midi_out = requires(
        t_midi_out o, 
        uint8_t note, 
        uint8_t channel, 
        uint8_t control,
        uint32_t velocity,
        uint32_t value
    ) {
        { o.note_on(note, channel, velocity) } -> std::same_as<void>;
        { o.note_off(note, channel, velocity)} -> std::same_as<void>;
        { o.cc(channel, control, value)} -> std::same_as<void>;
    };

    template <midi_out t_midi_out>
    class g2m_processor
    {
    private:
        t_midi_out _midi_out;

    public:
        g2m_processor(t_midi_out midi_out);

        void on_button_event(uint16_t button_code, button_state button_state);
        void on_axis_event(uint16_t axis_code, int16_t axis_value);
    };
}

#include "g2m_processor.tpp"

#endif // G2M_PROCESSOR_H