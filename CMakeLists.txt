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


if(NOT WIN32)
find_package(FFTW3 REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC fftw3)
else()
# fftw3.h,libfftw3-3.dll,libfftw3-3.lib
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/winfftw)
target_link_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/winfftw)
target_link_libraries(${PROJECT_NAME} PRIVATE libfftw3-3)
endif()



if(NOT MSVC)
if(NOT "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
target_link_libraries(${PROJECT_NAME} PRIVATE -static-libstdc++ -static-libgcc)
endif()
endif()

message("Python is " ${PYTHON_EXECUTABLE})
message("Python prefix is " ${PYTHON_PREFIX})

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
