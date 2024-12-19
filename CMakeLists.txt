cmake_minimum_required(VERSION 3.10)

# Set the project name
project(SpiderVerseExtended)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Create the shared library
add_library(amd_ags_x64 SHARED Spider-Verse-Extended.cpp)