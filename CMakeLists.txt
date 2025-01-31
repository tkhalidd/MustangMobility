cmake_minimum_required(VERSION 3.26)
project(build/Mustang_Mobility)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Download json.hpp at configure time
file(DOWNLOAD
    https://github.com/nlohmann/json/releases/download/v3.11.3/json.hpp
    ${CMAKE_BINARY_DIR}/json.hpp
    SHOW_PROGRESS
)

# Add the directory containing json.hpp to the include path
include_directories(${CMAKE_BINARY_DIR})
include_directories(.)

# Define source files
set(SOURCE_FILES
    backend/src/main.cpp
    backend/src/EntranceNode.cpp
    backend/src/EntranceNode.h
    backend/src/PathEdge.cpp
    backend/src/PathEdge.h
    backend/src/Graph.cpp
    backend/src/Graph.h
    backend/src/Dijkstra.cpp
    backend/src/Dijkstra.h
)

# Create the main executable
add_executable(Mustang_Mobility ${SOURCE_FILES})

# Enable testing
enable_testing()

# Find GTest package
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Add test executable (excluding main.cpp)
add_executable(graph_tests
    backend/tests/GraphTest.cpp
    backend/tests/DijkstraTest.cpp
    backend/src/EntranceNode.cpp
    backend/src/PathEdge.cpp
    backend/src/Graph.cpp
    backend/src/Dijkstra.cpp
)

# Link test executable with GTest
target_link_libraries(graph_tests
    GTest::GTest
    GTest::Main
    pthread
)

# Add tests to CTest
add_test(NAME graph_tests COMMAND graph_tests)