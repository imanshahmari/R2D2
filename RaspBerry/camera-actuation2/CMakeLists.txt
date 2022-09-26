cmake_minimum_required(VERSION 2.8.12)

PROJECT(obj-detection-yolo)
# PATH to OpenCV ( uncomment and provide path if required )
# set(OpenCV_DIR /Users/visionifai/codes/installations/OpenCV4/lib/cmake/opencv4/)

find_package( OpenCV REQUIRED)
find_package(Curses REQUIRED)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

include_directories( ${OpenCV_INCLUDE_DIRS})
include_directories(third-party/include ${CURSES_INCLUDE_DIR})

set(serial_SRCS third-party/src/serial.cc third-party/include/serial/serial.h 
    third-party/include/serial/v8stdint.h third-party/src/impl/list_ports/list_ports_linux.cc)
list(APPEND serial_SRCS third-party/src/impl/unix.cc)

add_library(serial ${serial_SRCS})
target_link_libraries(serial rt)

add_executable(object_detection_yolo object_detection_yolo.cpp)

target_link_libraries(object_detection_yolo ${OpenCV_LIBS} ${CURSES_LIBRARIES} serial)
