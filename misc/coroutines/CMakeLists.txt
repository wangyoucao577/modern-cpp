
cmake_minimum_required (VERSION 3.22)

project (coroutines)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "$ENV{CMAKE_CXX_FLAGS} -std=c++20 -fcoroutines")

# aux_source_directory(. SRCS)
add_executable (${PROJECT_NAME} coroutines.cc)
add_executable (fibo fibonacci.cc)

