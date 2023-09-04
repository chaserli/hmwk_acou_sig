cmake_minimum_required(VERSION 3.18)
set(CMAKE_SUPPRESS_REGENERATION ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(tmphmwk)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(eigen)
find_package(Eigen3 REQUIRED)
find_package(OpenMP)

set(PYBIND11_FINDPYTHON ON)
add_subdirectory(pybind11)

file(GLOB_RECURSE SOURCES "src/*.cpp")

pybind11_add_module(${PROJECT_NAME} ${SOURCES})

if(OpenMP_FOUND)
target_link_libraries(${PROJECT_NAME} PUBLIC OpenMP::OpenMP_CXX)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE Eigen3::Eigen)

if(NOT MSVC)
if(NOT "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
target_link_libraries(${PROJECT_NAME} PRIVATE -static-libstdc++ -static-libgcc)
endif()
endif()