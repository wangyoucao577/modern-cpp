
project(io_uring-echo-server)

aux_source_directory(. SRCS)
add_executable(${PROJECT_NAME} ${SRCS})

# import liburing
pkg_check_modules(LIBURING REQUIRED IMPORTED_TARGET liburing)
target_link_libraries(${PROJECT_NAME} PRIVATE PkgConfig::LIBURING)

