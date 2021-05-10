cmake_minimum_required(VERSION 3.17)
project(ParallelLab5 LANGUAGES C)

set(CMAKE_C_STANDARD 11)

find_package(MPI REQUIRED)

include_directories(SYSTEM ${MPI_INCLUDE_PATH})

file(GLOB_RECURSE sources src/*.c)
file(GLOB_RECURSE includes include/*.h)

add_executable(${PROJECT_NAME} ${sources} ${includes})

target_compile_options(${PROJECT_NAME} PUBLIC -Wall)
target_link_libraries(${PROJECT_NAME} ${MPI_C_LIBRARIES})
