
cmake_minimum_required(VERSION 3.22)

project(asyncio)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

find_package(PkgConfig REQUIRED)


add_subdirectory(io_uring-echo-server)

