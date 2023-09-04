#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/iostream.h>
#include <pybind11/stl.h>
#include <pybind11/eigen.h>

#include "dummy.hpp"

namespace py = pybind11;
namespace Examples{

template <Addable T>
T add(T i, T j)
{
    py::print("adding 2 ", typeid(T).name());
    return i + j;
}

float q_rsqrt(float num)
{
    static_assert(std::numeric_limits<float>::is_iec559);
    float y = std::bit_cast<float>(0x5F3759DF - (std::bit_cast<uint32_t>(num) >> 1));
    y *= 1.5f - (num * y * y * 0.5f);
    y *= 1.5f - (num * y * y * 0.5f);
    y *= 1.5f - (num * y * y * 0.5f);
    return y;
}

std::complex<double> hankel01(double r)
{
    std::complex<double> j0 = std::cyl_bessel_j(0, r);
    std::complex<double> y0 = std::cyl_neumann(0, r);
    return j0 + std::complex<double>{0.0, 1.0} * y0;
}

}
namespace py = pybind11;

PYBIND11_MODULE(tmphmwk, m) {
    m.doc() = "dummy doc";
    m.attr("CompiledBy")=Examples::CompilerInfo;

    m.def("add",py::vectorize(Examples::add<double>));
    m.def("add",py::vectorize(Examples::add<std::complex<double>>));
    m.def("add",Examples::add<std::string>);

    m.def("fqrsqrt",py::vectorize(Examples::q_rsqrt),"fast inv sqrt");
    m.def("hankel01",py::vectorize(Examples::hankel01),"H_0^1 but stl");

    m.def("TestTimes",TestEigen::Times);
}
