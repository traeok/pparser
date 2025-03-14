cmake_minimum_required(VERSION 3.5)
project(lexer-cpp)

# Include directories
include_directories(include)

# Source files
set(SOURCES
    src/lexer/token.cpp
    src/lexer/source.cpp
    src/lexer/error.cpp
    src/lexer/lexer.cpp
    src/parser/cli_parser.cpp
)

add_library(lexer STATIC ${SOURCES})

add_executable(lexer_demo src/lexer_demo.cpp ${SOURCES})

# Set output directory
set_target_properties(lexer PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Add compiler flags
if(MSVC)
    # MSVC flags
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    # GCC/Clang flags
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
endif() 