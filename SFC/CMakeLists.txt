cmake_minimum_required(VERSION 3.11)
project(sfc)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED Yes)
set (CMAKE_CXX_EXTENSIONS No)

include(FetchContent)

FetchContent_Declare(
        DataFrame
        GIT_REPOSITORY https://github.com/hosseinmoein/DataFrame.git
        GIT_TAG 1.20.0
)

FetchContent_MakeAvailable(DataFrame)

add_executable(sfc main.cpp Morton.hpp Hilbert.hpp)

target_link_libraries(sfc DataFrame::DataFrame)

install(TARGETS ${PROJECT_NAME} DESTINATION bin COMPONENT ${PROJECT_NAME})
