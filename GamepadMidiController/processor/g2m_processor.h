#ifndef G2M_PROCESSOR_H
#define G2M_PROCESSOR_H

#include <array>
#include <cstdint>
#include <vector>
#include "gamepad_scheme.h"

namespace gamepad_midi
{
    constexpr uint8_t SKIP_NOTE = std::numeric_limits<uint8_t>::max();
    constexpr uint8_t SKIP_CONTROL = std::numeric_limits<uint8_t>::max();
    constexpr uint32_t DEFAULT_VELOCITY = 3381864012;
    constexpr float JOYSTICK_DEADZONE = 0.3f; // TODO: replace with config
    constexpr int32_t AXIS_DEADZONE = 100; // TODO: replace with config (?)
    constexpr int32_t AXIS_CHANGE_SENSITIVITY = 100; // TODO: replace with config (?)

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
        std::array<int32_t, static_cast<size_t>(gamepad_axis::COUNT)> _axis_value = {};
        int _ls_index, _rs_index;

        uint8_t button_to_note(gamepad_button button) const;
        uint8_t axis_to_control(gamepad_axis axis) const;
        
        static uint32_t normalize_axis_value(int32_t value);
        static float axis_to_float(int32_t value);
        static int direction_to_index(float x, float y, int size);

        void play_chord(bool press, const std::vector<uint8_t>& chord_notes, uint8_t channel, uint32_t velocity);
        void play_joystick_chord(const std::vector<std::vector<uint8_t>>& chord_notes, int& prev_index, int32_t x, int32_t y);

    public:
        g2m_processor(t_midi_out midi_out);

        void on_button_event(gamepad_button button_code, button_state button_state);
        void on_axis_event(gamepad_axis axis_code, int32_t axis_value);
        void on_poll_end(void);
    };
}

#include "g2m_processor.tpp"

#endif // G2M_PROCESSOR_H