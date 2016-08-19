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
* cmake
* Doxygen (to build html documentation)

## Build

To build the library, create a directory in the root repo directory called `build`.cd to the build directory and run `cmake ..` to generate the Makefile and then run `make`. Alternatively, you can run the build script from the repo root directory by running:

		scripts/build.sh

Building the library will also automatically build and run the unit tests so make sure you have configured the test environment. See the README in the `test` directory for more info on how to do that.

## Documentation

The documentation for the library is built using Doxygen. From the root directory of the repo run the following command

		scripts/docs.sh

The script will build the latest documentation and automatically open the index html page in your default web browser.

