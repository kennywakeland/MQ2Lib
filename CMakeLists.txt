cmake_minimum_required(VERSION 2.8.4)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/ArduinoToolchain.cmake)
set(PROJECT_NAME MQLib)
project(${PROJECT_NAME})

#set(${CMAKE_PROJECT_NAME}_SKETCH MQLib.ino)

## https://github.com/queezythegreat/arduino-cmake
## https://github.com/queezythegreat/arduino-cmake/blob/master/example/CMakeLists.txt

#### Uncomment below additional settings as needed.
set(${CMAKE_PROJECT_NAME}_BOARD uno)
set(${CMAKE_PROJECT_NAME}_PORT /dev/ttyACM0)
# set(mega.build.mcu atmega2560)
# set(mega.upload.protocol wiring)
# set(mega.upload.speed 115200)

generate_arduino_firmware(${CMAKE_PROJECT_NAME}
        SRCS MQ_KWGO.cpp MQ2Lib.cpp MQ4Lib.cpp MQ6Lib.cpp MQ8Lib.cpp MQ9Lib.cpp
        HDRS MQ2Lib.h MQ4Lib.h MQ6Lib.h MQ8Lib.h MQ9Lib.h
        )
