#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/iostream.h>
#include <pybind11/stl.h>

#include <fftw3.h>
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

    // setup FFTW plan
    fftw_plan p = fftw_plan_dft_1d(buf_info.shape[0], reinterpret_cast<fftw_complex *>(buf_info.ptr), reinterpret_cast<fftw_complex *>(result.request().ptr), FFTW_FORWARD, FFTW_ESTIMATE);

    // execute the plan
    fftw_execute(p);

    // do cleanup
    fftw_destroy_plan(p);

    return result;
}

PYBIND11_MODULE(testfftw1dnormal, m)
{
    m.def("generic", &fftw1d);
}
