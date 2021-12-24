cmake_minimum_required(VERSION 2.8.11)

SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

set(files
    Spreadsheet.h
    Spreadsheet.cpp
    Cell.h
    Cell.cpp
    )

# Locate GTest
find_package(GTest REQUIRED)

# Link runTests with what we want to test and the GTest library
add_executable(runTests FunctionTesting.cpp ${files})

target_link_libraries(runTests GTest::Main)