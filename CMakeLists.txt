cmake_minimum_required(VERSION 3.20)
set(CMAKE_SUPPRESS_REGENERATION ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(tmphmwk LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
message(FATAL_ERROR "Clang sucks with anaconda libstdc++ version atm")
endif()

#add_compile_definitions(PYBIND11_NOINLINE_DISABLED)

find_package(OpenMP REQUIRED)
#set(PYBIND11_FINDPYTHON ON)
add_subdirectory(pybind11)

add_subdirectory(eigen)
find_package(Eigen3 REQUIRED)

add_subdirectory(fftwtest)

file(GLOB_RECURSE KSOURCES "src/*.cpp")
pybind11_add_module(${PROJECT_NAME} ${KSOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC mylib)

target_link_libraries(${PROJECT_NAME} PUBLIC OpenMP::OpenMP_CXX)

if(NOT MSVC)
target_link_libraries(${PROJECT_NAME} PRIVATE -static-libstdc++ -static-libgcc)
endif()

message("Python is " ${PYTHON_EXECUTABLE})
# message("Python prefix is " ${PYTHON_PREFIX})

