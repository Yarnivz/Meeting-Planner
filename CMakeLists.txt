cmake_minimum_required(VERSION 4.0)
project(Meeting_Planner)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_STANDARD 20)
# Remove the "-O2" flag if you have problems with the debugger.
set(CMAKE_CXX_FLAGS "-std=c++20 -Wall -Werror -O2")
set(CMAKE_EXE_LINKER_FLAGS -pthread)

# Set include dir
include_directories(./gtest/include ./xml_parser/include)

# Set Library dir
link_directories(gtest/lib)

set(XML_PARSER_SOURCE ./xml_parser/tinyxmlparser.cpp ./xml_parser/tinyxmlerror.cpp ./xml_parser/tinyxml.cpp ./xml_parser/tinystr.cpp)

# Set source files for RELEASE target
# Only add cpp files, no h files!
# Only add files with functionality here, no test files!
# Make sure the "main" function is in "main_release"!
set(RELEASE_SOURCE_FILES ./src/main.cpp
        src/Room.cpp
        src/Room.h
        src/Meeting.cpp
        src/Meeting.h
        src/Participation.cpp
        src/Participation.h
        src/Date.cpp
        src/Date.h
        src/App.cpp
        src/App.h)

# Set source files for DEBUG target
# Only add cpp files, no h files!
# Make sure the "main" function is in "main_debug"!
set(DEBUG_SOURCE_FILES ./src/tests.cpp)

# Create RELEASE target
add_executable(release_target ${RELEASE_SOURCE_FILES} ${XML_PARSER_SOURCE})

# Create DEBUG target
add_executable(debug_target ${DEBUG_SOURCE_FILES} ${XML_PARSER_SOURCE})

# Link library
target_link_libraries(debug_target gtest)
