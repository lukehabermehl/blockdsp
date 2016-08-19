cmake_minimum_required(VERSION 3.0)

project(libblockdsp)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "-std=c++11")

include_directories(/usr/local/include "Audio Manager/" "BlockDSP/" "Code Generation/" "Logger/")
file(GLOB SOURCES "Audio\ Manager/*.cpp" "Audio\ Manager/*.hpp" "BlockDSP/*.cpp" "BlockDSP/*.hpp" "Code\ Generation/*.cpp" "Code\ Generation.hpp" "Logger/*.cpp" "Logger/*.hpp")

set(PUBLIC_HEADERS "Audio Manager/autil_audioprocessingunit.hpp"
	"Audio Manager/autil_dspkernel.hpp"
	"Audio Manager/autil_file.hpp"
	"Audio Manager/autil_manager.hpp"
	"Audio Manager/autil_thread_manager.hpp"
	"BlockDSP/bdsp_apu.hpp"
	"BlockDSP/bdsp_node.hpp"
	"BlockDSP/bdsp_number.hpp"
	"BlockDSP/bdsp_parameter.hpp"
	"BlockDSP/bdsp_system.hpp"
	"BlockDSP/bdsp_node_apu.hpp"
	"BlockDSP/dsphelpers.hpp"
	"BlockDSP/dsputil_delaybuffer.hpp"
	"Code Generation/bdsp_apuloader.hpp"
	"Code Generation/bdsp_codebuilder.hpp"
	"Code Generation/bdsp_compiler.hpp"
	"Logger/bdsp_logger.hpp"
	"BuiltIns/bdsp_builtin_delay.hpp")

add_library(blockdsp STATIC ${SOURCES})

install(TARGETS blockdsp DESTINATION /usr/local/lib/)
install(FILES ${PUBLIC_HEADERS} DESTINATION /usr/local/include/blockdsp)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/blockdsp.h DESTINATION /usr/local/include)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test/gunit)