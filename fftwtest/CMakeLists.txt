
pybind11_add_module(testfftw1d_normal
${PROJECT_SOURCE_DIR}/fftwtest/generic.cpp
${PROJECT_SOURCE_DIR}/src/dummy.cpp
)

pybind11_add_module(testfftw1d_omp
${PROJECT_SOURCE_DIR}/fftwtest/withomp.cpp
${PROJECT_SOURCE_DIR}/src/dummy.cpp
)

pybind11_add_module(testfftw1d_eigen
${PROJECT_SOURCE_DIR}/fftwtest/useeigeninternal.cpp
${PROJECT_SOURCE_DIR}/src/dummy.cpp
)

pybind11_add_module(testwelch
${PROJECT_SOURCE_DIR}/fftwtest/pwelch.cpp
${PROJECT_SOURCE_DIR}/src/dummy.cpp
)

if(NOT WIN32)
# find_package(FFTW3 REQUIRED) # for ci
target_link_libraries(testfftw1d_normal PUBLIC fftw3 fftw3_omp)
target_link_libraries(testfftw1d_omp PUBLIC fftw3 fftw3_omp)
target_link_libraries(testwelch PUBLIC fftw3 fftw3_omp)
else()
# fftw3.h,libfftw3-3.dll,libfftw3-3.lib
target_include_directories(testfftw1d_normal PUBLIC ${PROJECT_SOURCE_DIR}/winfftw)
target_link_directories(testfftw1d_normal PUBLIC ${PROJECT_SOURCE_DIR}/winfftw)
target_link_libraries(testfftw1d_normal PRIVATE libfftw3-3)

target_include_directories(testfftw1d_omp PUBLIC ${PROJECT_SOURCE_DIR}/winfftw)
target_link_directories(testfftw1d_omp PUBLIC ${PROJECT_SOURCE_DIR}/winfftw)
target_link_libraries(testfftw1d_omp PRIVATE libfftw3-3)

target_include_directories(testwelch PUBLIC ${PROJECT_SOURCE_DIR}/winfftw)
target_link_directories(testwelch PUBLIC ${PROJECT_SOURCE_DIR}/winfftw)
target_link_libraries(testwelch PRIVATE libfftw3-3)
endif()

target_link_libraries(testfftw1d_normal PUBLIC OpenMP::OpenMP_CXX)
target_link_libraries(testfftw1d_omp PUBLIC OpenMP::OpenMP_CXX)
target_link_libraries(testfftw1d_eigen PRIVATE Eigen3::Eigen OpenMP::OpenMP_CXX)
target_link_libraries(testwelch PRIVATE Eigen3::Eigen OpenMP::OpenMP_CXX)

if(NOT MSVC)
target_link_libraries(testfftw1d_normal PRIVATE -static-libstdc++ -static-libgcc)
target_link_libraries(testfftw1d_omp PRIVATE -static-libstdc++ -static-libgcc)
target_link_libraries(testfftw1d_eigen PRIVATE -static-libstdc++ -static-libgcc)
target_link_libraries(testwelch PRIVATE -static-libstdc++ -static-libgcc)
endif()


if(WIN32)
add_custom_command(TARGET testfftw1d_normal POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${PROJECT_SOURCE_DIR}/winfftw/libfftw3-3.dll $<TARGET_FILE_DIR:testfftw1d_normal>
        )
add_custom_target(CopyFiles DEPENDS
        ${PROJECT_SOURCE_DIR}/winfftw/libfftw3-3.dll
        )
add_dependencies(testfftw1d_normal CopyFiles)
endif()