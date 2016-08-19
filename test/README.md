# Testing BlockDSP

## Setup

From this directory (`/test`), run the `setup_test_env.sh script`. This will clone and install a local copy of the googletest framework needed to run the unit tests. You will only have to do this once.

## Build and run unit tests

Run the following commands from this directory
	
	cd build
	cmake ..
	make

Or you can run the unit tests as part of building the main library
