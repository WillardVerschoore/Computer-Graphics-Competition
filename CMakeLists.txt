cmake_minimum_required(VERSION 3.2)

project(competition)

# Create a debug build (add -fopenmp for faster renderings)
set(CMAKE_CXX_FLAGS "-Wall --std=c++17 -g")

# Set all CPP files to be source files
file(GLOB_RECURSE SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
