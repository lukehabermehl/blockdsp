cmake_minimum_required(VERSION 3.0)

project(dsptestproj)
set(CMAKE_CXX_FLAGS "-std=c++11")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)

set(SOURCES "main.cpp")

find_library(PORTAUDIO portaudio
	PATHS /usr/local/lib)

find_library(SNDFILE sndfile
	PATHS /usr/local/lib)

add_executable(dsptest.out ${SOURCES})

target_link_libraries(dsptest.out ${CMAKE_CURRENT_SOURCE_DIR}/../lib/libblockdsp.a
							${PORTAUDIO}
							${SNDFILE})

install(TARGETS dsptest.out DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../)
