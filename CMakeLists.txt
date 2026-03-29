cmake_minimum_required(VERSION 3.31.6)
project(Meeting_Planner)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_STANDARD 20)
# Remove the "-O2" flag if you have problems with the debugger.
set(CMAKE_CXX_FLAGS "-std=c++20 -Wall -Werror -O2")
set(CMAKE_EXE_LINKER_FLAGS -pthread)

# Set include dir
include_directories(./gtest/include ./xml_parser/include ./file_utils)

# Set Library dir
link_directories(gtest/lib)

# Add xml parser to the project
set(XML_PARSER_SOURCE ./xml_parser/tinyxmlparser.cpp ./xml_parser/tinyxmlerror.cpp ./xml_parser/tinyxml.cpp ./xml_parser/tinystr.cpp)

#Add doxygen to project
#For windows: it checks the bin folder for the doxygen executable and uses that
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/bin/doxygen.exe")
    set(DOXYGEN_EXECUTABLE "${CMAKE_CURRENT_SOURCE_DIR}/bin/doxygen.exe")
endif()

#For linux: it checks whether the system has the doxygen package installed
find_package(Doxygen OPTIONAL_COMPONENTS)

set(DOCS_TOOLS_SOURCES tools/ContractsDocumentationGenerator.cpp)
if (DOXYGEN_FOUND)
    # add doxygen docs every time the project is build
    add_executable(docs_tools_target ${DOCS_TOOLS_SOURCES})
    doxygen_add_docs(
            generate_docs
            ${CMAKE_CURRENT_SOURCE_DIR}/src  # Folder containing the code to document
            CONFIG_FILE "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile"  # set doxygen configuration
            COMMENT "Auto-generating Doxygen documentation..."
    )
else ()
    # if doxygen is not found we give the user a warning
    message(WARNING "Doxygen or CLANG not found. Documentation will not be generated.")
endif ()

set(COMMON_SOURCE_FILES src/Room.cpp src/Meeting.cpp src/Participation.cpp src/Date.cpp src/App.cpp
        src/Parser.cpp
        src/Parser.h
        src/XmlParser.cpp
        src/XmlParser.h)

# Set source files for RELEASE target
# Only add cpp files, no h files!
# Only add files with functionality here, no test files!
# Make sure the "main" function is in "main_release"!
set(RELEASE_SOURCE_FILES ./src/main.cpp)

# Set source files for DEBUG target
# Only add cpp files, no h files!
# Make sure the "main" function is in "main_debug"!
set(DEBUG_SOURCE_FILES src/tests.cpp file_utils/FileUtils.cpp)

# Create RELEASE target
add_executable(release_target ${RELEASE_SOURCE_FILES} ${COMMON_SOURCE_FILES} ${XML_PARSER_SOURCE})

# Create DEBUG target
add_executable(debug_target ${DEBUG_SOURCE_FILES} ${COMMON_SOURCE_FILES} ${XML_PARSER_SOURCE})

# Link library
target_link_libraries(debug_target gtest)