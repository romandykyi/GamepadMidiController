#include "GamepadMidiController.h"

std::optional<gamepad_midi::midi1_output> midi_out;

static std::optional<libremidi::output_port> select_midi_out_port()
{
	std::cout << "Available MIDI output ports:\n";

	libremidi::observer obs;

	while (true)
	{
		auto ports = obs.get_output_ports();
		int i = 1;
		for (const libremidi::output_port& port : ports)
		{
			std::cout << i++ << " - " << port.port_name << '\n';
		}
		std::cout << "r - Refresh\n";
		std::cout << "0 - Exit\n";

		std::cout << "Select MIDI output port: ";

		std::string input;
		std::getline(std::cin, input);

		size_t option = 0;
		try 
		{
			option = std::stoll(input);
		}
		catch (const std::invalid_argument&)
		{
			continue;
		}
		catch (const std::out_of_range&)
		{
			continue;
		}

		if (option == 0) return std::nullopt;

		if (option > ports.size()) continue;

		return ports[option - 1];
	}
}

int main()
{
	using namespace gamepad_midi;

	std::optional<libremidi::output_port> out_port = select_midi_out_port();
	if (!out_port.has_value())
	{
		std::cout << "No MIDI output port selected. Exiting.\n";
		return 0;
	}

	std::cout << "\n";
	std::cout << "Selected port: " << out_port->port_name << '\n';
	
	libremidi::midi_out midi;
	midi.open_port(out_port.value());

	midi1_output midi_output(midi);
	g2m_processor<midi1_output> processor(midi_output);
	input_handler<g2m_processor<midi1_output>> gamepad_input(processor);

	gamepad_input.start_listening();
	std::cout << "Listening for input..." << std::endl;
	while (true)
	{
		gamepad_input.poll();
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	return 0;
}
