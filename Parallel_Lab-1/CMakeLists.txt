cmake_minimum_required(VERSION 3.17)
project(lab1 C)

include_directories(include)

set(CMAKE_C_STANDARD 17)

find_package(OpenMP REQUIRED)

add_executable(lab1 main.c)

target_link_libraries(lab1 PRIVATE OpenMP::OpenMP_C)
