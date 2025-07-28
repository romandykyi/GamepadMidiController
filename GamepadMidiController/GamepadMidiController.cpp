#include "GamepadMidiController.h"

libremidi::midi_out midi;

const double DEADZONE = 0.3;

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
		catch (const std::invalid_argument& e)
		{
			continue;
		}
		catch (const std::out_of_range& e)
		{
			continue;
		}

		if (option == 0) return std::nullopt;

		if (option > ports.size()) continue;

		return ports[option - 1];
	}
}

static void note_msg(float value, uint8_t channel, uint8_t note, uint8_t velocity)
{
	std::cout << "Note event: Channel: " << static_cast<int>(channel)
		<< ", Note: " << static_cast<int>(note)
		<< ", Velocity: " << static_cast<int>(velocity)
		<< ", Value: " << value << '\n';
	if (value < 0.5f)
	{
		midi.send_message(libremidi::channel_events::note_off(channel, note, velocity));
	}
	else
	{
		midi.send_message(libremidi::channel_events::note_on(channel, note, velocity));
	}
}

static void button_handler(std::shared_ptr<gamepad::device> dev)
{
	auto event = dev->last_button_event();

	switch (event->vc)
	{
	case gamepad::button::A:
		note_msg(event->virtual_value, 1, 36, 100);
		break;
	case gamepad::button::X:
		note_msg(event->virtual_value, 1, 38, 100);
		break;
	case gamepad::button::B:
		note_msg(event->virtual_value, 1, 42, 100);
		break;
	case gamepad::button::Y:
		note_msg(event->virtual_value, 1, 46, 100);
		break;
	default:
		std::cout << "Button event: Native id: " << event->native_id
			<< ", Virtual id: " << event->vc
			<< ", V.Value: " << event->virtual_value << '\n';
		break;
	}
}

static void axis_cc(float value, uint8_t channel, uint8_t control)
{
	value = std::round(value * 127.0f);
	uint8_t midi_value = static_cast<uint8_t>(value);

	midi.send_message(libremidi::channel_events::control_change(channel, control, midi_value));
}

static void axis_handler(std::shared_ptr<gamepad::device> dev)
{
	auto event = dev->last_axis_event();

	/*std::cout << "Axis event: Native id: " << event->native_id
		<< ", Virtual id: " << event->vc
		<< ", V.Value: " << event->virtual_value << '\n';*/

	switch (event->vc)
	{
		case gamepad::axis::RIGHT_TRIGGER:
			axis_cc(event->virtual_value, 2, 69);
			break;
		case gamepad::axis::LEFT_TRIGGER:
			axis_cc(event->virtual_value, 2, 70);
			break;
	}
}

static void chord_msg(bool press, const std::vector<uint8_t>& notes, uint8_t channel, uint8_t velocity)
{
	std::cout << "Chord event: Channel: " << static_cast<int>(channel)
		<< ", First Note: " << static_cast<int>(notes[0])
		<< ", Velocity: " << static_cast<int>(velocity)
		<< ", Action: " << (press ? "hold" : "release") << '\n';

	for (uint8_t note : notes)
	{
		if (press)
		{
			midi.send_message(libremidi::channel_events::note_on(channel, note, velocity));
		}
		else
		{
			midi.send_message(libremidi::channel_events::note_off(channel, note, velocity));
		}
	}
}

static int values_to_index(float x, float y, int size)
{
	x = -x;
	y = -y;

	float magnitude = std::sqrt(x * x + y * y);
	if (magnitude < DEADZONE)
	{
		return -1; // Outside of the deadzone
	}

	float angleRadians = std::atan2(y, x);
	float angleDegrees = angleRadians * (180.0f / M_PI);

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

static void process_chords(const std::vector<std::vector<uint8_t>>& chord_notes, int& prev_index, float x, float y)
{
	int index = values_to_index(x, y, chord_notes.size());

	if (index == prev_index) return;

	if (prev_index != -1)
	{
		chord_msg(false, chord_notes[prev_index], 2, 100);
	}

	if (index != -1)
	{
		chord_msg(true, chord_notes[index], 2, 100);
	}

	prev_index = index;
}

int main()
{
	std::optional<libremidi::output_port> out_port = select_midi_out_port();
	if (!out_port.has_value())
	{
		std::cout << "No MIDI output port selected. Exiting.\n";
		return 0;
	}

	std::cout << "\n";
	std::cout << "Selected port: " << out_port->port_name << '\n';

	midi.open_port(out_port.value());

	auto h = gamepad::hook::make();
	h->set_plug_and_play(true, gamepad::ms(1000));

	h->set_axis_event_handler(axis_handler);
	h->set_button_event_handler(button_handler);

	if (!h->start()) 
	{
		std::cerr << "Failed to start hook\n";
		return 1;
	}

	int l_prev_index = -1;
	int r_prev_index = -1;
	std::vector<std::vector<uint8_t>> left_notes = 
	{
		{ 61, },
		{ 62, },
		{ 64, },
		{ 65, },
		{ 67, },
		{ 69, },
	};
	std::vector<std::vector<uint8_t>> right_notes =
	{
		{ 61 + 12, },
		{ 62 + 12, },
		{ 64 + 12, },
		{ 65 + 12, },
		{ 67 + 12, },
		{ 69 + 12, },
	};
	while (true)
	{
		float lx, ly;
		lx = h->get_devices()[0]->get_axis(gamepad::axis::LEFT_STICK_X);
		ly = h->get_devices()[0]->get_axis(gamepad::axis::LEFT_STICK_Y);

		process_chords(left_notes, l_prev_index, lx, ly);

		float rx, ry;
		rx = h->get_devices()[0]->get_axis(gamepad::axis::RIGHT_STICK_X);
		ry = h->get_devices()[0]->get_axis(gamepad::axis::RIGHT_STICK_Y);

		process_chords(right_notes, r_prev_index, rx, ry);
	}

	midi.close_port();

	return 0;
}
