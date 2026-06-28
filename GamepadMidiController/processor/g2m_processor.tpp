#ifndef G2M_PROCESSOR_H // Hint for IDEs
#include "g2m_processor.h"
#endif

#include <cmath>
#include <limits>
#include <utility>

namespace gamepad_midi
{
    template <midi_out t_midi_out>
    g2m_processor<t_midi_out>::g2m_processor(t_midi_out midi_out) : _midi_out(std::move(midi_out)) 
    {
        _ls_index = _rs_index = -1;
    }

    template <midi_out t_midi_out>
    uint8_t g2m_processor<t_midi_out>::button_to_note(gamepad_button button) const
    {
        switch (button)
        {
            case gamepad_button::NORTH:
                return 46;
            case gamepad_button::EAST:
                return 42;
            case gamepad_button::SOUTH:
                return 36;
            case gamepad_button::WEST:
                return 38;
        }
        // Invalid button - skip
        return SKIP_NOTE;
    }

    template <midi_out t_midi_out>
    uint8_t g2m_processor<t_midi_out>::axis_to_control(gamepad_axis axis) const
    {
        switch (axis)
        {
            case gamepad_axis::RT:
                return 69;
            case gamepad_axis::LT:
                return 70;
        }
        return SKIP_CONTROL;
    }

    template <midi_out t_midi_out>
    uint32_t g2m_processor<t_midi_out>::normalize_axis_value(int32_t value)
    {
        int32_t clamped = value & ~(value >> 31);
        return static_cast<uint32_t>(clamped) << 1;
    }

    template <midi_out t_midi_out>
    float g2m_processor<t_midi_out>::axis_to_float(int32_t value)
    {
        return value / 2147483648.0f;
    }

    template <midi_out t_midi_out>
    int g2m_processor<t_midi_out>::direction_to_index(float x, float y, int size)
    {
        x = -x;
        y = -y;

        float magnitude = std::sqrt(x * x + y * y);
        if (magnitude < JOYSTICK_DEADZONE)
        {
            return -1; // Outside of the deadzone
        }

        float angleRadians = std::atan2(y, x);
        float angleDegrees = angleRadians * (180.0f / static_cast<float>(M_PI));

        if (angleDegrees < 0)
        {
            angleDegrees += 360.0f;
        }

        // Normalize angle to 0 - 1
        float value01 = angleDegrees / 360.0f;
        value01 -= 0.25f; // Shift so 0 is at the top
        value01 = value01 - floor(value01); // Ensure it's in the range [0, 1)

        return (int)(value01 * size);
    }

    template <midi_out t_midi_out>
    void g2m_processor<t_midi_out>::play_chord(
        bool press, const std::vector<uint8_t>& chord_notes, uint8_t channel, uint32_t velocity)
    {
        for (uint8_t note : chord_notes)
        {
            if (press)
            {
                _midi_out.note_on(channel, note, velocity);
            }
            else
            {
                _midi_out.note_off(channel, note, velocity);
            }
        }
    }

    template <midi_out t_midi_out>
    void g2m_processor<t_midi_out>::play_joystick_chord(
        const std::vector<std::vector<uint8_t>>& chord_notes, int& prev_index, int32_t axis_x, int32_t axis_y)
    {
        float x = axis_to_float(axis_x);
        float y = axis_to_float(axis_y);
        int index = direction_to_index(x, y, static_cast<int>(chord_notes.size()));

        if (index == prev_index) return;

        if (prev_index != -1)
        {
            play_chord(false, chord_notes[prev_index], 2, DEFAULT_VELOCITY);
        }

        if (index != -1)
        {
            play_chord(true, chord_notes[index], 2, DEFAULT_VELOCITY);
        }

        prev_index = index;
    }

    template <midi_out t_midi_out>
    void g2m_processor<t_midi_out>::on_button_event(gamepad_button button_code, button_state button_state)
    {
        uint8_t note = button_to_note(button_code);
        if (note == SKIP_NOTE) return;

        if (button_state == button_state::PRESSED)
        {
            _midi_out.note_on(1, note, DEFAULT_VELOCITY);
        }
        else
        {
            _midi_out.note_off(1, note, DEFAULT_VELOCITY);
        }
    }
    
    template <midi_out t_midi_out>
    void g2m_processor<t_midi_out>::on_axis_event(gamepad_axis axis_code, int32_t axis_value)
    {
        size_t axis_index = static_cast<size_t>(axis_code);
        int32_t prev_axis_value = _axis_value[axis_index];

        if (std::abs(axis_value) < AXIS_DEADZONE)
        {
            axis_value = 0;
        }
        if (std::abs(axis_value - prev_axis_value) < AXIS_CHANGE_SENSITIVITY)
        {
            return;
        }

        _axis_value[axis_index] = axis_value;

        uint8_t control = axis_to_control(axis_code);
        if (control == SKIP_CONTROL) return;
        
        _midi_out.cc(2, control, normalize_axis_value(axis_value));
    }

    template <midi_out t_midi_out>
    void g2m_processor<t_midi_out>::on_poll_end(void)
    {
        // TODO: replace with config
        static std::vector<std::vector<uint8_t>> left_notes = 
        {
            { 61, },
            { 62, },
            { 64, },
            { 65, },
            { 67, },
            { 69, },
        };
        static std::vector<std::vector<uint8_t>> right_notes =
        {
            { 61 + 12, },
            { 62 + 12, },
            { 64 + 12, },
            { 65 + 12, },
            { 67 + 12, },
            { 69 + 12, },
        };

		play_joystick_chord(
            left_notes, 
            _ls_index, 
            _axis_value[static_cast<size_t>(gamepad_axis::LS_X)], 
            _axis_value[static_cast<size_t>(gamepad_axis::LS_Y)]
        );

		play_joystick_chord(
            right_notes, 
            _rs_index, 
            _axis_value[static_cast<size_t>(gamepad_axis::RS_X)], 
            _axis_value[static_cast<size_t>(gamepad_axis::RS_Y)]
        );
    }
}