cmake_minimum_required(VERSION 3.28)
project(SynthTest)

set(CMAKE_CXX_STANDARD 17)

# The important library finder
set(PORTAUDIO_DIR /opt/homebrew/Cellar/portaudio/19.7.0)
set(RTMIDI_DIR /opt/homebrew/Cellar/rtmidi/6.0.0)

include_directories(${PORTAUDIO_DIR}/include/ ${RTMIDI_DIR}/include/)
link_directories(${PORTAUDIO_DIR}/lib/)

link_directories(${RTMIDI_DIR}/lib/)

add_executable(SynthTest main.cpp
        WavetableOscillator.h
        WavetableOscillator.cpp
        MidiParser.h
        MidiParser.cpp
        ADSR.h
        ADSR.cpp)
target_link_libraries(SynthTest portaudio RtMidi)