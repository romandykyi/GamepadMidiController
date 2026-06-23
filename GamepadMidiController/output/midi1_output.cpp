#include "midi1_output.h"
#include <utility>
#include <iostream>

namespace gamepad_midi
{
    midi1_output::midi1_output(libremidi::midi_out& midi) : _midi(midi) {}

    void midi1_output::note_on(uint8_t channel, uint8_t note, uint32_t velocity)
    {
        uint8_t velocity_7bit = velocity >> 25; // Quantize by taking the 7 top bits
		_midi.send_message(libremidi::channel_events::note_on(channel, note, velocity_7bit));
    }
    
    void midi1_output::note_off(uint8_t channel, uint8_t note, uint32_t velocity)
    {
        uint8_t velocity_7bit = velocity >> 25; // Quantize by taking the 7 top bits
		_midi.send_message(libremidi::channel_events::note_off(channel, note, velocity_7bit));
    }

    void midi1_output::cc(uint8_t channel, uint8_t control, uint32_t value)
    {
        uint8_t value_7bit = value >> 25; // Quantize by taking the 7 top bits
	    _midi.send_message(libremidi::channel_events::control_change(channel, control, value_7bit));
    }
}