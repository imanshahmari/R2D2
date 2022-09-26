project(detectball)

cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

find_package(OpenCV REQUIRED)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

include_directories(${PROJECT_BINARY_DIR})
include_directories(third-party/include ${CURSES_INCLUDE_DIR})

set(serial_SRCS third-party/src/serial.cc third-party/include/serial/serial.h 
    third-party/include/serial/v8stdint.h third-party/src/impl/list_ports/list_ports_linux.cc)
list(APPEND serial_SRCS third-party/src/impl/unix.cc)


add_library(serial ${serial_SRCS})
target_link_libraries(serial rt)

add_executable (${PROJECT_NAME} detectball.cpp)
target_link_libraries(${PROJECT_NAME}  ${OpenCV_LIBS} serial)


