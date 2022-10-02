cmake_minimum_required(VERSION 3.0)

project(application)

set (CMAKE_CXX_STANDARD 17)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

file(GLOB SOURCES "*.c")

add_executable(run ${SOURCES} main.cpp)

add_library(typer src/typeTrainer.cpp src/typeTrainer.hpp)
add_library(app src/app.hpp src/app.cpp)

target_link_libraries(typer PUBLIC ${CURSES_LIBRARIES})
target_link_libraries(app PUBLIC typer)
target_link_libraries(run PUBLIC ${CURSES_LIBRARIES} app)

