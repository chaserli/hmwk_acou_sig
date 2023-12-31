#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/iostream.h>
#include <pybind11/stl.h>

#include <omp.h>
#include <fftw3.h>

namespace py = pybind11;


// Generated by GPT:
// Releasing and Acquiring GIL: When running long-running tasks or calling blocking functions from C++ (like fftw_execute), it is crucial to release the GIL before performing these operations. This allows other Python threads to run while the operation is ongoing. After the task completes, the GIL should be reacquired before returning control to Python or accessing Python objects. This can be done using py::gil_scoped_release and py::gil_scoped_acquire in Pybind11.
// Potential Crashes with GIL: If the GIL is not properly managed, it can lead to crashes. For instance, if you release the GIL before calling a function and don't reacquire it before the function returns, there's a risk that another Python thread might execute and interact with the Python objects that your function is still working on. This could potentially cause data races or other concurrency issues, leading to a crash.
// Automatic GIL Management: Pybind11 provides a way to automatically release the GIL during a function’s execution using py::call_guard<py::gil_scoped_release>(). However, this only guarantees that the GIL is released at function entry and re-acquired before function exit. If any Python callbacks occur during the function's execution (e.g., due to signal handling), they could potentially access objects concurrently, causing a crash. Manual control over the GIL (i.e., releasing it just before the long-running operation and acquiring it immediately after) can prevent this issue.
py::array_t<std::complex<double>> manual_nogil(py::array_t<std::complex<double>, py::array::c_style | py::array::forcecast> input_array)
{
    py::buffer_info buf_info = input_array.request();

    if (buf_info.ndim != 1)
        throw std::runtime_error("Number of dimensions must be 1");

    auto result = py::array_t<std::complex<double>>(buf_info.size);

    fftw_plan_with_nthreads(omp_get_max_threads());
{
    py::gil_scoped_release release;

    auto plan = fftw_plan_dft_1d(
        buf_info.shape[0],
        reinterpret_cast<fftw_complex *>(buf_info.ptr),
        reinterpret_cast<fftw_complex *>(result.request().ptr),
        FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(plan);

    py::gil_scoped_acquire acquire;
}
    return result;
}

PYBIND11_MODULE(testfftw1d_omp, m)
{
    py::print("omp fftw_plan_dft_1d loaded");
    m.def("manualomp",&manual_nogil);
}
