
project(io_uring_echo_server)

set(IO_URING_ECHO_SERVER_TARGET_NAME echo_server)

aux_source_directory(. SRCS)
add_executable(${IO_URING_ECHO_SERVER_TARGET_NAME} ${SRCS})

# import liburing
pkg_check_modules(LIBURING REQUIRED IMPORTED_TARGET liburing)
target_link_libraries(${IO_URING_ECHO_SERVER_TARGET_NAME} PRIVATE PkgConfig::LIBURING)
