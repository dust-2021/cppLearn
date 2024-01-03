cmake_minimum_required(VERSION 3.25)
project(firstProject)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")

add_executable(firstProject main.cpp
        py/PythonExecutor.cpp
        py/PythonExecutor.h
        util/http/request.cpp
        util/http/request.h
        util/json/elements.cpp
        util/json/elements.h
)