# cmake .. -DBUILD_PYTHON=ON
# cmake --build .
cmake_minimum_required(VERSION 3.0)

project(libdb LANGUAGES CXX C)

option(PYTHON "Build a Python Wrapper" OFF)
option(GOLANG "Build a Golang Wrapper" ON)

message("Build a Python Wrapper? ${PYTHON}")
message("Build a Golang Wrapper? ${GOLANG}")


if(PYTHON)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -pthread -std=c++14 -Wall -Wextra -Wno-unused-parameter")
    if(PYTHONLIBS_VERSION_STRING MATCHES "^2.*$" )
        set(CMAKE_SWIG_FLAGS -classic)
    else()
        set(CMAKE_SWIG_FLAGS -py3)
    endif()
endif()

if(GOLANG)
    set(CMAKE_SWIG_FLAGS -c++ -cgo -intgosize 64)
endif()


set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost 1.69.0 REQUIRED)
find_package(PythonLibs 3 REQUIRED)
find_package(PythonInterp ${PYTHONLIBS_VERSION_STRING} REQUIRED)
find_package(SWIG REQUIRED)
include(${SWIG_USE_FILE})

set(SWIG_MODULE_NAME libdb)

message("PYTHONLIBS_VERSION_STRING: ${PYTHONLIBS_VERSION_STRING}")
message("CMAKE_SWIG_FLAGS: ${CMAKE_SWIG_FLAGS}")

include_directories(${Boost_INCLUDE_DIRS})

include_directories(libdb ${PYTHON_INCLUDE_PATH} ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(libdb ${CMAKE_CURRENT_SOURCE_DIR}/src/cpp)

add_library(libdbImpl SHARED src/cpp/libdb.cpp src/cpp/libdb.h)
target_compile_features(libdbImpl PUBLIC cxx_std_14)

set_source_files_properties(src/cpp/libdb.i PROPERTIES CPLUSPLUS ON)

if(PYTHON)
    swig_add_library(libdb TYPE SHARED LANGUAGE python SOURCES src/cpp/libdb.i)
    swig_link_libraries(libdb ${PYTHON_LIBRARIES} libdbImpl)
    set_target_properties(_libdb PROPERTIES SUFFIX ".so")
endif()

if(GOLANG)
    swig_add_library(libdb TYPE SHARED LANGUAGE go SOURCES src/cpp/libdb.i)
    swig_link_libraries(libdb libdbImpl)
endif()


