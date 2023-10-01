#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/iostream.h>
#include <pybind11/stl.h>

#include <cmmnfft.h>
namespace py = pybind11;

py::array_t<std::complex<double>> fftw1d(py::array_t<std::complex<double>, py::array::c_style | py::array::forcecast> input_array)
{
    // request a mutable buffer proxy to the data
    py::buffer_info buf_info = input_array.request();

    // check dimensions
    if (buf_info.ndim != 1)
        throw std::runtime_error("Number of dimensions must be 1");

    // Create new array for output
    auto result = py::array_t<std::complex<double>>(buf_info.size);

    FFTPlan{buf_info.shape[0], buf_info.ptr, result.request().ptr}.execute();

    return result;
}

PYBIND11_MODULE(testfftw1dnormal, m)
{
    py::print("generic fftw_plan_dft_1d loaded");
    m.def("generic", &fftw1d);
    m.def("forget_wisdom",&fftw_forget_wisdom);
}
