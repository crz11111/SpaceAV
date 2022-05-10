cmake_minimum_required(VERSION 3.11)
project(SFC)

set(CMAKE_CXX_STANDARD 17)

include (FetchContent)

FetchContent_Declare(
        DataFrame
        GIT_REPOSITORY https://github.com/hosseinmoein/DataFrame.git
        GIT_TAG        1.20.0
)

FetchContent_MakeAvailable(DataFrame)

add_executable(SFC main.cpp MortonExperiment.hpp Hilbert.hpp)

target_link_libraries (SFC PRIVATE DataFrame::DataFrame)
