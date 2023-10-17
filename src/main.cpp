#include "dummy.hpp"
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/iostream.h>
#include <pybind11/stl.h>

namespace py = pybind11;

std::complex<double> hankel01(double r)
{
    return std::complex<double>{std::cyl_bessel_j(0, r), std::cyl_neumann(0, r)};
}

PYBIND11_MODULE(tmphmwk, m)
{
    py::print("loading custom pybind submodule bulit by", TmpMetaInfo::CompilerInfo, " at ",TmpMetaInfo::CompileTime);

    m.doc() = "dummy doc";
    m.attr("CompilerInfo") = TmpMetaInfo::CompilerInfo;
    m.attr("CompileTime") = TmpMetaInfo::CompileTime;

    m.def("hankel01", py::vectorize(hankel01), "H_0^1 but stl");

}
