# libBlockDSP

This library provides some helpful tools for audio signal processing in C++ as well as a framework for building audio processing chains dynamically using "blocks". As the library is currently in development, there are several areas of key functionality that have not yet been implemented.

## Overview
BlockDSP will allow for the creation of virtually any kind of audio effect without using hard-coded math. The library provides several types of Block object classes that each perform a certain operation on an input signal and are contained within a System. These blocks can be dynamically created, connected, and destroyed by your program. Additionally, the library provides the capability to build dynamic libraries that contain your audio processing systems to allow for exporting to other applications as well as custom code insertion.

## Features

* Dynamically build real-time audio effects and other processing chains
* Export processing chains as dynamic libraries that can be wrapped with any audio processing framework (AudioUnits, VST, etc.)
* Built-in playback through PortAudio
* Built-in audio file reading

## Dependencies

* PortAudio
* libsndfile