
cmake_minimum_required(VERSION 3.22)

project(io_uring-echo-server)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

find_package(PkgConfig REQUIRED)

aux_source_directory(. SRCS)
add_executable(${PROJECT_NAME} ${SRCS})

# import liburing
pkg_check_modules(LIBURING REQUIRED IMPORTED_TARGET liburing)
target_link_libraries(${PROJECT_NAME} PRIVATE PkgConfig::LIBURING)

