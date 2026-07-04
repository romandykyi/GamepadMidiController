#ifndef G2M_PROCESSOR_H
#define G2M_PROCESSOR_H

#include <array>
#include <cstdint>
#include <vector>
#include "gamepad_scheme.h"

namespace gamepad_midi
{
    /**
     * @brief A note code that makes the processor ignore the button.
     */
    inline constexpr uint8_t SKIP_NOTE = std::numeric_limits<uint8_t>::max();
    /**
     * @brief A control code that makes the processor ignore the axis.
     */
    inline constexpr uint8_t SKIP_CONTROL = std::numeric_limits<uint8_t>::max();
    /**
     * @brief The default velocity (equals 100 in MIDI 1).
     */
    inline constexpr uint32_t DEFAULT_VELOCITY = 3381864012;
    /**
     * @brief The joystick deadzone where notes are not registered.
     * 
     * Will be replaced with a config.
     */
    inline constexpr float JOYSTICK_DEADZONE = 0.3f; // TODO: replace with config
    /**
     * @brief The axis deadzone for CC events.
     * 
     * Will be replaced with a config.
     */
    inline constexpr int32_t AXIS_DEADZONE = 100; // TODO: replace with config (?)
    /**
     * @brief The axis change sensitivity threshold. Changes under this threshold are not registered.
     * 
     * Will be replaced with a config.
     */
    inline constexpr int32_t AXIS_CHANGE_SENSITIVITY = 100; // TODO: replace with config (?)

    /**
     * @brief Requires support for sending MIDI note on event.
     * 
     * @param note A note to be sent.
     * @param channel A MIDI channel to use.
     * @param velocity The note's velocity.
     */
    template<typename T>
    concept has_note_on = requires(T r, uint8_t note, uint8_t channel, uint32_t velocity) {
        { r.note_on(note, channel, velocity) } -> std::same_as<void>;
    };

    /**
     * @brief Requires support for sending MIDI note off event.
     * 
     * @param note A note to be sent.
     * @param channel A MIDI channel to use.
     * @param velocity The note's velocity.
     */
    template<typename T>
    concept has_note_off = requires(T r, uint8_t note, uint8_t channel, uint32_t velocity) {
        { r.note_off(note, channel, velocity) } -> std::same_as<void>;
    };

    /**
     * @brief Requires support for sending MIDI CC event.
     * 
     * @param channel A MIDI channel to use.
     * @param control A MIDI control to be sent.
     * @param value The control's value.
     */
    template<typename T>
    concept has_cc = requires(T r, uint8_t channel, uint8_t control, uint32_t value) {
        { r.cc(channel, control, value) } -> std::same_as<void>;
    };

    /**
     * @brief A concept for a component that sends MIDI events.
     */
    template<typename t_midi_out>
    concept midi_out = 
        has_note_on<t_midi_out> &&
        has_note_off<t_midi_out> &&
        has_cc<t_midi_out>;

    /**
     * @brief A component responsible for mapping gamepad input into MIDI output.
     * 
     * @tparam midi_out A template for a component that sends MIDI events.
     */
    template <midi_out t_midi_out>
    class g2m_processor
    {    
    private:
        t_midi_out _midi_out;
        std::array<int32_t, GAMEPAD_AXES_NUM> _axis_value = {};
        int _ls_index, _rs_index;

        uint8_t button_to_note(gamepad_button button) const;
        uint8_t axis_to_control(gamepad_axis axis) const;
        
        static uint32_t normalize_axis_value(int32_t value);
        static float axis_to_float(int32_t value);
        static int direction_to_index(float x, float y, int size);

        void play_chord(bool press, const std::vector<uint8_t>& chord_notes, uint8_t channel, uint32_t velocity);
        void play_joystick_chord(const std::vector<std::vector<uint8_t>>& chord_notes, int& prev_index, int32_t x, int32_t y);

    public:
        /**
         * @brief Creates an instance based on the MIDI sender.
         * 
         * @param midi_out The MIDI event sender. Must outlive this class.
         */
        g2m_processor(t_midi_out midi_out);

        /**
         * @brief Processes the gamepad button events (pressed/released).
         * 
         * @param button_code The gamepad button that was pressed/released.
         * @param button_state The state of the gamepad button (pressed/released).
         */
        void on_button_event(gamepad_button button_code, button_state button_state);
        /**
         * @brief Processes the gamepad axis event.
         * 
         * @param axis_code The gamepad axis that is being moved.
         * @param axis_value The updated axis value. Assumed to never be negative for triggers.
         */
        void on_axis_event(gamepad_axis axis_code, int32_t axis_value);
        /**
         * Processes the end of the input polling cycle.
         */
        void on_poll_end(void);
    };
}

#include "g2m_processor.tpp"

#endif // G2M_PROCESSOR_H