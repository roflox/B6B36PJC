cmake_minimum_required(VERSION 3.5)
project(InitB LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(tests
    factorial.cpp
    factorial.hpp
    tests-factorial.cpp
    tests-main.cpp
    catch.hpp
)

if ( CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang|GNU" )
    target_compile_options( tests PRIVATE -Wall -Wextra -Wunreachable-code -Wpedantic -DCATCH_CONFIG_CONSOLE_WIDTH=80)
endif()
if ( CMAKE_CXX_COMPILER_ID MATCHES "MSVC" )
    target_compile_options( tests PRIVATE /W4 )
endif()


if (${CMAKE_VERSION} VERSION_GREATER 3.5)
  set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT tests)
endif()


include(CTest)
add_test(NAME all COMMAND tests)
