# Base Template For CMake Based STM32 VSCode Projects

This repo provides a template for STM32 projects using a CMake based build system on VSCode. It contains the STM32CubeL4 repo, relevant CMakeList.txt files, as well as VSCode configurations for debugging. This repo also sets the necessary configurations to override the printf function to output text serially via the Virtual COM Port (plug the board in with a USB, open up a serial monitor and relevant COM port with baud rate of 9600)

## How to Begin
- To clone this repo along with the STM32CubeL4 submodule, make sure to use the `--recursive` flag when initially cloning

## How to Build
- To generate the build files (with debug enabled), run the folowing command from the parent directory
```
cmake -G "MinGW Makefiles" -B "build" ./ -DCMAKE_TOOLCHAIN_FILE="CMake/arm-none-eabi.cmake" -DCMAKE_BUILD_TYPE=debug
```
- To build the project after the build files have been generated, run
```
cmake --build ./build
```

Alternativley, generating the build system, as well as building the project, have both been added as tasks in tasks.json

Therefore, you can conveniently use VS tasks to run the commands. I recommend installing the 'Fast Tasks' extension so that the tasks are easily accessible via the Explorer

## What Should Happen?
- Once you have succesfuly built the project, you an flash it onto the board from the 'Run and Debug Session' menu
- Run the 'Debug with ST-Link Option'
- The debugger should open up and run the program fine
- Additionally, you should be able to see "Hello World :)" print to the serial monitor after that line of code has run
