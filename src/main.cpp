#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/iostream.h>
#include <pybind11/stl.h>

#include "dummy.hpp"

namespace py = pybind11;
namespace Examples{

std::complex<double> hankel01(double r)
{
    return std::complex<double>{std::cyl_bessel_j(0, r),std::cyl_neumann(0, r)};
}

}


namespace py = pybind11;

PYBIND11_MODULE(tmphmwk, m) {
    py::print("loading custom pybind submodule bulit by",Examples::CompilerInfo," ...");

    m.doc() = "dummy doc";
    m.attr("CompiledBy")=Examples::CompilerInfo;
    m.attr("version")=0x1234567890;

    m.def("hankel01",py::vectorize(Examples::hankel01),"H_0^1 but stl");


    m.def("fftw1d",TestFFT::fftw1d,py::call_guard<py::gil_scoped_release>());
    m.def("fftw1d_omp",TestFFT::fftw1d_omp, py::call_guard<py::gil_scoped_release>());
    m.def("fftw1d_omp_manual",TestFFT::fftw1d_omp_nogil);
}
