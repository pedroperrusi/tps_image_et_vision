cmake_minimum_required(VERSION 2.8)

project(image)
set(EXECUTABLE_OUTPUT_PATH "bin")

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable( ${PROJECT_NAME} src/image.cpp )

target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS} )