cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME TASKA)

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM and C languages
enable_language(C ASM)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # 硬件层 bsp
    bsp/bsp_can.c
    bsp/bsp_servo.c
    bsp/bsp_motor.c
    bsp/bsp_joystick.c
    bsp/bsp_led.c
    bsp/bsp_oled.c
    bsp/bsp_oled_i2c.c
    # 中间层 component
    component/controller/PID.c
    component/algorithm/ramp.c
    component/algorithm/filter.c
    # 应用层 application
    application/Communication/can_send.c
    application/Communication/can_receive.c
    application/Gimbal/gimbal.c
    application/Chassis/chassis.c
    application/OLED/oled_app.c
    application/OLED/gimbal_oled.c
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    bsp
    component/controller
    component/algorithm
    application
    application/Communication
    application/Gimbal
    application/Chassis
    application/OLED
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
)

# Remove wrong libob.a library dependency when using cpp files
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx

    # Add user defined libraries
)
