cmake_minimum_required(VERSION 3.16)
project(IridiumProject)

enable_language(C CXX ASM)

# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#Include file specifying startup and linker script
include(${CMAKE_SOURCE_DIR}/CMake/scripts.cmake)
#Include file specifying CPU compile options
include(${CMAKE_SOURCE_DIR}/CMake/mcu.cmake)


add_compile_options(${CPU_COMPILE_OPTIONS})

add_link_options(
    ${CPU_COMPILE_OPTIONS}
    --specs=nano.specs
	-T${LINKER_SCRIPT}

	LINKER:--start-group
	-lc
	-lm
	# -lnosys
	LINKER:--end-group
	LINKER:--gc-sections
	LINKER:--print-memory-usage
)

# Source directories
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Src)

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/Inc)

include(${CMAKE_CURRENT_SOURCE_DIR}/CMake/stm32cubel4.cmake)

add_subdirectory(${SRC_DIR})