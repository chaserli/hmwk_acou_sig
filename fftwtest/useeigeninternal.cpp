#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/iostream.h>
#include <pybind11/stl.h>

#include <Eigen/Dense>
#include <unsupported/Eigen/FFT>
#include <pybind11/eigen.h>

#include <omp.h>

namespace py = pybind11;
Eigen::FFT<double> EigenFFT;
Eigen::VectorXcd UseEigenFFT(const Eigen::Ref<const Eigen::VectorXcd>& input)
{
    py::gil_scoped_release {};

    Eigen::VectorXcd output {input.size()};
    EigenFFT.fwd(output.data(),input.data(),input.size());

    py::gil_scoped_acquire {};
    return output;
}


PYBIND11_MODULE(testfftw1d_eigen, m)
{
    py::print("Eigen",EIGEN_WORLD_VERSION, ".",EIGEN_MAJOR_VERSION,".",EIGEN_MINOR_VERSION, "internal kiss fft loaded");
    m.def("fwd_fft", &UseEigenFFT);
}
