
pybind11_add_module(testfftw1d_normal
${PROJECT_SOURCE_DIR}/fftwtest/generic.cpp
)

pybind11_add_module(testfftw1d_omp
${PROJECT_SOURCE_DIR}/fftwtest/withomp.cpp
)

find_package(Eigen3 REQUIRED)
pybind11_add_module(testfftw1d_eigen
${PROJECT_SOURCE_DIR}/fftwtest/useeigeninternal.cpp
)

pybind11_add_module(testwelch
${PROJECT_SOURCE_DIR}/fftwtest/pwelch.cpp
)

find_package(FFTW3 CONFIG REQUIRED)
find_package(OpenMP REQUIRED)

target_link_libraries(testfftw1d_normal PRIVATE FFTW3::fftw3)
target_link_libraries(testfftw1d_omp PRIVATE OpenMP::OpenMP_CXX FFTW3::fftw3 FFTW3::fftw3_omp)
target_link_libraries(testfftw1d_eigen PRIVATE Eigen3::Eigen OpenMP::OpenMP_CXX FFTW3::fftw3 FFTW3::fftw3_omp)
target_link_libraries(testwelch PRIVATE Eigen3::Eigen OpenMP::OpenMP_CXX FFTW3::fftw3 FFTW3::fftw3_omp)

