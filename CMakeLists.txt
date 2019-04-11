# cmake .. -DBUILD_PYTHON=ON
# cmake --build .
cmake_minimum_required(VERSION 3.0)

project(libdb LANGUAGES CXX C)

option(PYTHON "Build a Python wrapper" OFF)
option(GOLANG "Build a Golang wrapper" OFF)
option(CSHARP "Build a C# wrapper" OFF)
option(PHP "Build a PHP wrapper" OFF)

if( NOT PYTHON AND NOT GOLANG AND NOT CSHARP AND NOT PHP)
  message( FATAL_ERROR "You need to select a language target" )
endif()

message("Build a Python wrapper? ${PYTHON}")
message("Build a Golang wrapper? ${GOLANG}")
message("Build a C# wrapper? ${CSHARP}")
message("Build a PHP wrapper? ${PHP}")


if(PYTHON)
    set(ASYNC_API TRUE)
    find_package(PythonLibs 3 REQUIRED)
    find_package(PythonInterp ${PYTHONLIBS_VERSION_STRING} REQUIRED)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -pthread -std=c++14 -Wall -Wextra -Wno-unused-parameter")
    if(PYTHONLIBS_VERSION_STRING MATCHES "^2.*$" )
        set(CMAKE_SWIG_FLAGS -classic)
    else()
        set(CMAKE_SWIG_FLAGS -py3)
    endif()
    message("PYTHONLIBS_VERSION_STRING: ${PYTHONLIBS_VERSION_STRING}")
    include_directories(libdb ${PYTHON_INCLUDE_PATH} ${CMAKE_CURRENT_SOURCE_DIR})

endif()

if(GOLANG)
    set(ASYNC_API FALSE)
    set(CMAKE_SWIG_FLAGS -c++ -cgo -intgosize 64)
endif()

if(CSHARP)
    set(ASYNC_API TRUE)
    #set(CMAKE_SWIG_FLAGS -c++ -csharp) # for C# this is redundant
endif()

if(PHP)
    set(ASYNC_API FALSE)
    #set(CMAKE_SWIG_FLAGS -c++ -php7) # for PHP this is redundant
    find_program(PHP_CONFIG php-config)
    if (PHP_CONFIG-NOTFOUND)
            message(FATAL_ERROR "Cannot find php-config, please set the variable PHP_CONFIG to the location of php-config then try again.")
            return ()
    endif ()

    execute_process(
        COMMAND ${PHP_CONFIG} --includes
        RESULT_VARIABLE PHP_CONFIG_INCLUDES_RESULT
        OUTPUT_VARIABLE PHP_CONFIG_INCLUDES
        ERROR_VARIABLE PHP_CONFIG_INCLUDES_ERR
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    if ("${PHP_CONFIG_INCLUDES_RESULT}" STREQUAL "0")
        message(STATUS "Using PHP include directories: ${PHP_CONFIG_INCLUDES}")
    else ()
        message(FATAL_ERROR "An error has occurred while getting PHP include directories. Error: ${PHP_CONFIG_INCLUDES_ERR}")
        return ()
    endif ()
    include_directories(libdb ${PHP_CONFIG_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR})
endif()

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost 1.69.0 REQUIRED)
find_package(SWIG REQUIRED)
include(${SWIG_USE_FILE})

set(SWIG_MODULE_NAME libdb)

message("CMAKE_SWIG_FLAGS: ${CMAKE_SWIG_FLAGS}")

include_directories(${Boost_INCLUDE_DIRS})


if(ASYNC_API)
    add_library(libdbImpl SHARED src/cpp/async/libdb.cpp src/cpp/async/libdb.h)
    set_source_files_properties(src/cpp/async/libdb.i PROPERTIES CPLUSPLUS ON)
    include_directories(libdb ${CMAKE_CURRENT_SOURCE_DIR}/src/cpp/async)
else()
    add_library(libdbImpl SHARED src/cpp/blocking/libdb.cpp src/cpp/blocking/libdb.h)
    set_source_files_properties(src/cpp/blocking/libdb.i PROPERTIES CPLUSPLUS ON)
    include_directories(libdb ${CMAKE_CURRENT_SOURCE_DIR}/src/cpp/blocking)
endif()

target_compile_features(libdbImpl PUBLIC cxx_std_14)

if(PYTHON)
    swig_add_library(libdb TYPE SHARED LANGUAGE python SOURCES src/cpp/async/libdb.i)
    swig_link_libraries(libdb ${PYTHON_LIBRARIES} libdbImpl)
    set_target_properties(_libdb PROPERTIES SUFFIX ".so")
elseif(GOLANG)
    swig_add_library(libdb TYPE SHARED LANGUAGE go SOURCES src/cpp/blocking/libdb.i)
    swig_link_libraries(libdb libdbImpl)
elseif(CSHARP)
    swig_add_library(libdb TYPE SHARED LANGUAGE csharp SOURCES src/cpp/async/libdb.i)
    swig_link_libraries(libdb libdbImpl)
elseif(PHP)
    swig_add_library(libdb TYPE SHARED LANGUAGE php7 SOURCES src/cpp/blocking/libdb.i)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${PHP_CONFIG_INCLUDES} -Wl,-undefined,dynamic_lookup")
    swig_link_libraries(libdb libdbImpl)
    set_target_properties(libdb PROPERTIES SUFFIX ".so")
endif()
