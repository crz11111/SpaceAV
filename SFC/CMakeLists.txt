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

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(sfc src/main.cpp src/Morton.hpp src/Hilbert.hpp)

target_link_libraries(sfc DataFrame::DataFrame)

enable_testing()
add_executable(hilbert-runner ${CMAKE_CURRENT_SOURCE_DIR}/test/tests-main.cpp ${CMAKE_CURRENT_SOURCE_DIR}/test/tests-hilbert.cpp)
target_link_libraries(hilbert-runner ${LIBRARIES})
add_test(NAME hilbert-runner COMMAND hilbert-runner)

install(TARGETS ${PROJECT_NAME} DESTINATION bin COMPONENT ${PROJECT_NAME})
