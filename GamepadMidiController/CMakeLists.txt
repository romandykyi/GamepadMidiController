cmake_minimum_required(VERSION 3.14)  # Ensure minimum version for FetchContent
project(GamepadMidiController)

# Ensure C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# Fetch libremidi from Git
FetchContent_Declare(
    libremidi
    GIT_REPOSITORY https://github.com/celtera/libremidi.git
    GIT_TAG        v5.3.1
)

FetchContent_MakeAvailable(libremidi)

# Add executable
add_executable(GamepadMidiController
    GamepadMidiController.cpp
    GamepadMidiController.h
)

# Link libremidi (uses alias target)
target_link_libraries(GamepadMidiController PRIVATE libremidi::libremidi)
