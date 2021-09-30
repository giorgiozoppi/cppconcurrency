cmake_minimum_required(VERSION 3.11)
project(monitor)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
file(GLOB SOURCES "src/*.cpp")
include_directories("/usr/local/include")

add_executable(demo ${SOURCES})
target_link_libraries(demo )
# TODO: Run -Werror in CI.
target_compile_options(monitor PRIVATE -Wall -Wextra -Werror)
