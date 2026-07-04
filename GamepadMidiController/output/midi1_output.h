#ifndef MIDI1_OUTPUT_H
#define MIDI1_OUTPUT_H

#include <cstdint>
#include <libremidi/libremidi.hpp>
#include <libremidi/message.hpp>

namespace gamepad_midi
{
    /**
     * @brief A component that is responsible for sending MIDI 1 events.
     */
    class midi1_output
    {
    private:
        libremidi::midi_out& _midi;    

    public:
        /**
         * @brief Creates an instance based on the libremidi out port.
         * 
         * @param midi A libremidi out port. Must be initialized and opened before. Must outlive this class.
         */
        midi1_output(libremidi::midi_out& midi);

        /**
         * @brief Sends the MIDI note on event.
         * 
         * @param channel A MIDI channel to use.
         * @param note A MIDI note to be sent.
         * @param velocity The note's velocity.
         */
        void note_on(uint8_t channel, uint8_t note, uint32_t velocity);
        /**
         * @brief Sends the MIDI note off event.
         * 
         * @param channel A MIDI channel to use.
         * @param note A MIDI note to be sent.
         * @param velocity The note's velocity.
         */
        void note_off(uint8_t channel, uint8_t note, uint32_t velocity);
        /**
         * @brief Sends the MIDI CC event.
         * 
         * @param channel A channel to use.
         * @param control A MIDI control to be sent.
         * @param value The control's value.
         */
        void cc(uint8_t channel, uint8_t control, uint32_t value);
    };
}

#endif // MIDI1_OUTPUT_H