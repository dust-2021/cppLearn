cmake_minimum_required(VERSION 3.25)
project(firstProject)

set(CMAKE_CXX_STANDARD 17)

add_executable(firstProject main.cpp util/http/Request.cpp util/http/Request.h util/http/Response.cpp util/http/Response.h
        util/json/JsonParser.cpp
        util/json/JsonParser.h
        py/PythonExecutor.cpp
        py/PythonExecutor.h
        net/Distributor.cpp
        net/Distributor.h
        util/json/JsonElement.cpp
        util/json/JsonElement.h
)