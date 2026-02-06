cmake_minimum_required(VERSION 3.10)
project(STM32)


# Collect all .c source files in Core/Sr
file(GLOB SRC_FILES "${CMAKE_SOURCE_DIR}/Src/*.c")

# Add the executable using SRC files
add_executable(MAIN 
    ${SRC_FILES}
    ${STARTUP_SCRIPT}    
)

# Include headers
target_include_directories(MAIN 
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Inc"
)

target_link_libraries(MAIN PUBLIC STM32CUBEL4_HAL)