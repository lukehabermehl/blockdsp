cmake_minimum_required(VERSION 3.0)

project(blockdsp_unit_tests)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "-std=c++11 -ggdb")

find_library(PORTAUDIO portaudio
	PATHS /usr/local/lib)

find_library(SNDFILE sndfile
	PATHS /usr/local/lib)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../include/ ${CMAKE_CURRENT_SOURCE_DIR}/../include/)

set (SOURCES "test_bdsp_system.cpp" 
	"test_apu_node.cpp"
	"test_audiomanager_api.cpp"
	"test_apunumber.cpp"
	"test_apu_parameter.cpp"
	"test_code_builder.cpp"
	"test_dsp_blocks.cpp"
	"test_thread_manager.cpp"
	"test_bdsp_parameter.cpp"
	"test_all.cpp")

add_executable (test_all ${SOURCES})
target_link_libraries(test_all 
						${PORTAUDIO}
						${SNDFILE}
						${CMAKE_CURRENT_SOURCE_DIR}/../../lib/libblockdsp.a
						${CMAKE_CURRENT_SOURCE_DIR}/../lib/libgtest.a
						${CMAKE_CURRENT_SOURCE_DIR}/../lib/libgtest_main.a
						libpthread.a)

add_custom_command(TARGET test_all
					POST_BUILD
					COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test_all)