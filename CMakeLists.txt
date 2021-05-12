cmake_minimum_required(VERSION 3.16)
project(Rubik)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_subdirectory(core)
add_executable(r main.cpp PrettyPrint.cpp)
target_link_libraries(r rcore)
