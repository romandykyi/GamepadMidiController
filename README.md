# GamepadToMidi

GamepadToMidi lets you turn a standard game controller into a playable MIDI instrument. 

It maps gamepad buttons, triggers, and joysticks to MIDI note and control change (CC) messages in real time, allowing you to control your favorite DAW or virtual instrument using a gamepad.

## Features

- Real-time translation of gamepad input to MIDI messages
- Support for both note and control change events
- Interactive selection of available MIDI output ports
- Customizable mappings for:
  - Buttons → MIDI notes
  - Triggers → MIDI control changes
  - Analog sticks → Chord triggering
- Cross-platform architecture (Windows, macOS, Linux)

Uses modern C++20 and lightweight libraries (libremidi, libgamepad).

## How it works

When you launch the program, it lists all available MIDI output ports.
You select one (for example, a loopMIDI virtual port on Windows), and from then on:

| Gamepad Input | MIDI Action | Default Channel | Notes/CC |
|----------------|--------------|------------------|-----------|
| A | Note On/Off | 1 | 36 (C1) |
| B | Note On/Off | 1 | 42 (F#1) |
| X | Note On/Off | 1 | 38 (D1) |
| Y | Note On/Off | 1 | 46 (A#1) |
| Left Trigger | Control Change | 2 | CC 70 |
| Right Trigger | Control Change | 2 | CC 69 |
| Left Stick | Chord selection (low octave) | 2 | 61–69 |
| Right Stick | Chord selection (high octave) | 2 | 73–81 |

## Requirements

- C++20 compatible compiler
- CMake ≥ 3.14
- A virtual MIDI loopback tool such as:
  - [loopMIDI](https://www.tobias-erichsen.de/software/loopmidi.html) (Windows only)

## Instruction (Windows)

1. Create a virtual MIDI port using loopMIDI
2. Run the program:
3. Select your MIDI output port when prompted.
4. Open your DAW or synth and set its MIDI input to the created virtual port.
5. Start playing! Press gamepad buttons to trigger notes. Move triggers or sticks for expressive control.

## Customization 

To be implemented. . .

## Used libraries
- https://github.com/celtera/libremidi
- https://github.com/univrsal/libgamepad
