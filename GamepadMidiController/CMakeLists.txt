cmake_minimum_required(VERSION 3.14)
project(GamepadMidiController)

# Ensure C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# libremidi
FetchContent_Declare(
    libremidi
    GIT_REPOSITORY https://github.com/celtera/libremidi.git
    GIT_TAG        v5.3.1
)
# libgamepad
FetchContent_Declare(
    libgamepad
    GIT_REPOSITORY https://github.com/univrsal/libgamepad
    GIT_TAG        v1.1
)

FetchContent_MakeAvailable(libremidi)
FetchContent_MakeAvailable(libgamepad)

add_executable(GamepadMidiController
    GamepadMidiController.cpp
    GamepadMidiController.h
)

target_link_libraries(GamepadMidiController PRIVATE libremidi::libremidi)
target_link_libraries(GamepadMidiController PRIVATE gamepad)
