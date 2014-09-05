set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/arduino-cmake/cmake/modules")
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/arduino-cmake/cmake/toolchains/Arduino.cmake")

cmake_minimum_required(VERSION 2.8)

project(btmobile)
find_package(Arduino 22 REQUIRED)

include_directories(
"${CMAKE_SOURCE_DIR}"
"${CMAKE_SOURCE_DIR}/BtDriver"
"${CMAKE_SOURCE_DIR}/../libraries")

#====================================================================#
#  Settings                                                          #
#====================================================================#
set(FIRMWARE_NAME btmobile)

set(${FIRMWARE_NAME}_BOARD atmega328)
set(${FIRMWARE_NAME}_SRCS 
btmobile.cpp 
BtDriver/BtMotor.cpp
BtDriver/BtSteer.cpp
BtDriver/BtSeeker.cpp

"${CMAKE_SOURCE_DIR}/../libraries/Ultrasonic/Ultrasonic.cpp")

set(${FIRMWARE_NAME}_PORT /dev/ttyUSB0)
set(${FIRMWARE_NAME}_SERIAL picocom @INPUT_PORT@ -b 9600 -l)


#====================================================================#
#  Target generation                                                 #
#====================================================================#
GENERATE_ARDUINO_FIRMWARE(${FIRMWARE_NAME})
