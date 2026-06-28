// GamepadMidiController.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#define _USE_MATH_DEFINES
#define NOMINMAX // Disable windows macros, because they suck

#include <libgamepad.hpp>
#include <iostream>
#include <cstdio>
#include <cmath>
#include <cstdint>
#include <limits>
#include <thread>
#include <chrono>
#include <libremidi/libremidi.hpp>
#include <libremidi/message.hpp>

#include "midi1_output.h"
#include "g2m_processor.h"
#include "input_handler.h"