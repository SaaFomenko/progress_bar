cmake_minimum_required(VERSION 3.18)

project(progress_bar)

#sub_directory(lib)

# set the C++ standard
set(CMAKE_CXX_STANDARD 17)


set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

set(public_header lib/big_integer.h)
set(sources lib/big_integer.cpp)

add_library(${PROJECT_NAME}_lib ${sources} ${public_header})

add_executable(${PROJECT_NAME}.exe main.cpp ${sources})

target_link_libraries(${PROJECT_NAME}.exe ${PROJECT_NAME}_lib)
