# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# initalize pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
# set(PICO_SDK_PATH "/YOUR_PICO_SDK_PATH/pico-sdk")

# Pull in Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(pico-pwm-audio C CXX ASM)

set(PICO_CXX_ENABLE_EXCEPTIONS 1)

set(PICO_CXX_ENABLE_RTTI 1)

# Initialise the Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.2

add_executable(picosounds picosounds.c 
                          pwm_channel.c 
                          debounce_button.c 
                          double_buffer.c 
                          colour_noise.c
                          hw_config.c
                          fs_mount.c
                          ./picomp3lib/interface/music_file.c
               )

pico_set_program_name(picosounds "picosounds")
pico_set_program_version(picosounds "0.1")

pico_enable_stdio_uart(picosounds 1)
pico_enable_stdio_usb(picosounds 0)

add_subdirectory(no-OS-FatFS-SD-SPI-RPi-Pico/FatFs_SPI build)
add_subdirectory(picomp3lib/src)

# Add the standard library to the build
target_link_libraries(picosounds pico_stdlib)

# Add any user requested libraries
target_link_libraries(picosounds
                      hardware_dma
                      hardware_pio
                      hardware_timer
                      hardware_clocks
                      hardware_pwm
                      FatFs_SPI 
                      picomp3lib
                     )

pico_add_extra_outputs(picosounds)

