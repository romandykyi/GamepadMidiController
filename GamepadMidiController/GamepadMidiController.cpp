// GamepadMidiController.cpp : Defines the entry point for the application.
//

#include "GamepadMidiController.h"

using namespace std;

int main()
{
	// Create the midi object
	libremidi::midi_out midi;

	std::cout << "Available MIDI input ports:\n";
	libremidi::observer obs;
	libremidi::output_port out_port;
	for (const libremidi::output_port& port : obs.get_output_ports()) 
	{
		if (port.port_name.find("loop") != std::string::npos)
		{
			out_port = port;
		}
	}

	std::cout << "Port: " << out_port.port_name << std::endl;

	midi.open_port(out_port);

	bool wasADown = false;
	bool wasSDown = false;
	while (true) 
	{
		bool isADown = (GetAsyncKeyState('A') & 0x8000) != 0;
		bool isSDown = (GetAsyncKeyState('K') & 0x8000) != 0;

		if (isADown && !wasADown)
		{
			std::cout << "Pressed C (channel 1)" << std::endl;
			midi.send_message(libremidi::channel_events::note_on(1, 60, 100));
		}
		else if (!isADown && wasADown)
		{
			std::cout << "Released C (channel 1)" << std::endl;
			midi.send_message(libremidi::channel_events::note_off(1, 60, 100));
		}

		if (isSDown && !wasSDown)
		{
			midi.send_message(libremidi::channel_events::control_change(1, 74, 80));
			std::cout << "Control change 74 (channel 1) to 80" << std::endl;
		}
		else if (!isSDown && wasSDown)
		{
			midi.send_message(libremidi::channel_events::control_change(1, 74, 0));
			std::cout << "Control change 74 (channel 1) to 0" << std::endl;
		}

		wasADown = isADown;
		wasSDown = isSDown;
		Sleep(10); // sleep to avoid CPU overload
	}

	return 0;
}
