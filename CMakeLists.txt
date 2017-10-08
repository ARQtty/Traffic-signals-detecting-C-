cmake_minimum_required(VERSION 3.7)
project(HackCV)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package( OpenCV REQUIRED )
include_directories(${OpenCV_INCLUDE_DIRS})
set(SOURCE_FILES main.cpp Classifier.cpp Classifier.h)
add_executable(HackCV ${SOURCE_FILES})

include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries( HackCV ${OpenCV_LIBS} )




