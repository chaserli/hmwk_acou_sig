#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/iostream.h>
#include <pybind11/stl.h>

#include <fftw3.h>
namespace py = pybind11;

py::array_t<std::complex<double>> fftw1d(py::array_t<std::complex<double>, py::array::c_style | py::array::forcecast> input_array)
{
    py::buffer_info buf_info = input_array.request();

    if (buf_info.ndim != 1)
        throw std::runtime_error("Number of dimensions must be 1");

    auto result = py::array_t<std::complex<double>>(buf_info.size);
    auto plan = fftw_plan_dft_1d(
        buf_info.shape[0],
        reinterpret_cast<fftw_complex *>(buf_info.ptr),
        reinterpret_cast<fftw_complex *>(result.request().ptr),
        FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(plan);

    return result;
}

PYBIND11_MODULE(testfftw1d_normal, m)
{
    py::print("generic fftw_plan_dft_1d loaded");
    m.def("generic", &fftw1d);
    m.def("forget_wisdom",&fftw_forget_wisdom);
}
