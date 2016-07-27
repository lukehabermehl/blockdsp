# Testing BlockDSP

## Setup

1. Clone the [googletest](https://github.com/google/googletest) repository to this directory (ROOT/test/) or anywhere else.

2. Build googletest and copy the *include* directory to this directory

3. Create a directory called *lib* within this directory and copy the **libgtest.a** and **libgtest_main.a** library files to it.

4. Your directory structure should look like this:

		test
			gunit
			include
				gtest
					-> gtest.h
						...
			lib
				-> libgtest.a
				-> libgtest_main.a

## Build and run unit tests

Run the following commands from this directory

	make
	./test_all
