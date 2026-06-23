#ifndef MIDI1_OUTPUT_H
#define MIDI1_OUTPUT_H

#include <cstdint>
#include <libremidi/libremidi.hpp>
#include <libremidi/message.hpp>

namespace gamepad_midi
{
    class midi1_output
    {
    private:
        libremidi::midi_out& _midi;    

    public:
        midi1_output(libremidi::midi_out& midi);

        void note_on(uint8_t channel, uint8_t note, uint32_t velocity);
        void note_off(uint8_t channel, uint8_t note, uint32_t velocity);
        void cc(uint8_t channel, uint8_t control, uint32_t value);
    };
}

#endif // MIDI1_OUTPUT_H