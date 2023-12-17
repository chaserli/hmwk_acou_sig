cmake_minimum_required(VERSION 3.20)
set(CMAKE_SUPPRESS_REGENERATION ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(tmphmwk)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_definitions(
    EIGEN_FFTW_DEFAULT
    #PYBIND11_NOINLINE_DISABLED
    )

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set(BUILD_TESTING OFF)
find_package(OpenMP REQUIRED)
set(PYBIND11_FINDPYTHON ON)
add_subdirectory(pybind11)

add_subdirectory(eigen)
find_package(Eigen3 REQUIRED)
add_subdirectory(fftwtest)

file(GLOB_RECURSE KSOURCES "src/*.cpp")
pybind11_add_module(${PROJECT_NAME} ${KSOURCES})

target_link_libraries(${PROJECT_NAME} PUBLIC OpenMP::OpenMP_CXX)

find_package(Boost)
if(${Boost_FOUND})
target_include_directories(${PROJECT_NAME} PUBLIC ${Boost_INCLUDE_DIRS})
endif()
