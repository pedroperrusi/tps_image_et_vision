cmake_minimum_required(VERSION 2.8)

project(operations)
set(EXECUTABLE_OUTPUT_PATH "bin")

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable( ${PROJECT_NAME} src/operations.cpp )

target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS} )