cmake_minimum_required(VERSION 3.10)
project(path_tracer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SRC_FILES
    "src/*.cpp"
)

add_executable(path_tracer ${SRC_FILES}) 