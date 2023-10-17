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

include_directories(inc)

pybind11_add_module(testfftw1d_normal fftwtest/generic.cpp src/dummy.cpp)
pybind11_add_module(testfftw1d_omp fftwtest/withomp.cpp src/dummy.cpp)
file(GLOB_RECURSE KSOURCES "src/*.cpp")
pybind11_add_module(${PROJECT_NAME} ${KSOURCES})

if(NOT WIN32)
# find_package(FFTW3 REQUIRED) # for ci
target_link_libraries(testfftw1d_normal PUBLIC fftw3 fftw3_omp)
target_link_libraries(testfftw1d_omp PUBLIC fftw3 fftw3_omp)
else()
# fftw3.h,libfftw3-3.dll,libfftw3-3.lib
target_include_directories(testfftw1d_normal PUBLIC ${PROJECT_SOURCE_DIR}/winfftw)
target_link_directories(testfftw1d_normal PUBLIC ${PROJECT_SOURCE_DIR}/winfftw)
target_link_libraries(testfftw1d_normal PRIVATE libfftw3-3)

target_include_directories(testfftw1d_omp PUBLIC ${PROJECT_SOURCE_DIR}/winfftw)
target_link_directories(testfftw1d_omp PUBLIC ${PROJECT_SOURCE_DIR}/winfftw)
target_link_libraries(testfftw1d_omp PRIVATE libfftw3-3)
endif()


target_link_libraries(${PROJECT_NAME} PUBLIC OpenMP::OpenMP_CXX)
target_link_libraries(testfftw1d_normal PUBLIC OpenMP::OpenMP_CXX)
target_link_libraries(testfftw1d_omp PUBLIC OpenMP::OpenMP_CXX)


add_subdirectory(eigen)
find_package(Eigen3 REQUIRED)

pybind11_add_module(testfftw1d_eigen fftwtest/useeigeninternal.cpp src/dummy.cpp)
target_link_libraries(testfftw1d_eigen PRIVATE Eigen3::Eigen OpenMP::OpenMP_CXX)

if(NOT MSVC)
target_link_libraries(${PROJECT_NAME} PRIVATE -static-libstdc++ -static-libgcc)
target_link_libraries(testfftw1d_normal PRIVATE -static-libstdc++ -static-libgcc)
target_link_libraries(testfftw1d_omp PRIVATE -static-libstdc++ -static-libgcc)
endif()

message("Python is " ${PYTHON_EXECUTABLE})
# message("Python prefix is " ${PYTHON_PREFIX})

if(WIN32)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${PROJECT_SOURCE_DIR}/winfftw/libfftw3-3.dll $<TARGET_FILE_DIR:${PROJECT_NAME}>
        )
add_custom_target(CopyFiles DEPENDS
        ${PROJECT_SOURCE_DIR}/winfftw/libfftw3-3.dll
        )
add_dependencies(${PROJECT_NAME} CopyFiles)
endif()
