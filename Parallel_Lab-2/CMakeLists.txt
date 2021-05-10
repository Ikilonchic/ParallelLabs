cmake_minimum_required(VERSION 3.0)
project(ParallelLab2 C CXX)

find_package(MPI REQUIRED)

include_directories(. ${MPI_INCLUDE_PATH})

add_executable(ParallelLab2 main.c)

target_link_libraries(ParallelLab2 PUBLIC MPI::MPI_CXX)
