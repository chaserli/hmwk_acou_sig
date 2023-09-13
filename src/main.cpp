#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/iostream.h>
#include <pybind11/stl.h>
#include <pybind11/eigen.h>

#include "dummy.hpp"

#include <fftw3.h>

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
    return std::complex<double>{std::cyl_bessel_j(0, r),std::cyl_neumann(0, r)};
}

}

py::array_t<std::complex<double>> fftw_transform1d(py::array_t<std::complex<double>, py::array::c_style | py::array::forcecast> input_array) {
    // request a mutable buffer proxy to the data
    py::buffer_info buf_info = input_array.request();

    // check dimensions
    if (buf_info.ndim != 1)
        throw std::runtime_error("Number of dimensions must be 1");

    // Create new array for output
    auto result = py::array_t<std::complex<double>>(buf_info.size);

    // setup FFTW plan
    fftw_plan p = fftw_plan_dft_1d(buf_info.shape[0]
    , reinterpret_cast<fftw_complex *>(buf_info.ptr),reinterpret_cast<fftw_complex*>(result.request().ptr)
    , FFTW_FORWARD, FFTW_ESTIMATE);

    // execute the plan
    fftw_execute(p);

    // do cleanup
    fftw_destroy_plan(p);

    return result;
}

namespace py = pybind11;

PYBIND11_MODULE(tmphmwk, m) {
    m.doc() = "dummy doc";
    m.attr("CompiledBy")=Examples::CompilerInfo;
    m.attr("version")=114514;

    m.def("add",py::vectorize(Examples::add<double>));
    m.def("add",py::vectorize(Examples::add<std::complex<double>>));
    m.def("add",Examples::add<std::string>);

    m.def("fqrsqrt",py::vectorize(Examples::q_rsqrt),"fast inv sqrt");
    m.def("hankel01",py::vectorize(Examples::hankel01),"H_0^1 but stl");

    m.def("TestTimes",TestEigen::Times);
    m.def("fftw1d",fftw_transform1d,"rfft1d from fftw");
}
