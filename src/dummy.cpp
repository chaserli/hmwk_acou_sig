#include "dummy.hpp"
#include <pybind11/iostream.h>
#include <pybind11/stl.h>
#include <omp.h>
#include <fftw3.h>

#ifndef DEBUG
#ifdef __linux__

void __attribute__((constructor)) load_so()
{
    std::cout << "Dll loaded\n";
}

void __attribute__((destructor)) unload_so()
{
    std::cout << "Dll detached\n";
}
#endif

#ifdef _WIN32
#include <Windows.h>

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        std::cout << "Dll process attached\n";
        break;

    case DLL_THREAD_ATTACH:
        break;

    case DLL_THREAD_DETACH:
        break;

    case DLL_PROCESS_DETACH:
        if (lpvReserved != nullptr)
        {
            std::cout << "Dll process detached\n";
            break;
        }
        break;
    }
    return TRUE;
}
#endif
#endif
namespace TestFFT
{
    namespace py = pybind11;
    py::array_t<std::complex<double>> fftw1d_omp_nogil(py::array_t<std::complex<double>, py::array::c_style | py::array::forcecast> input_array)
    {
        // request a mutable buffer proxy to the data
        py::buffer_info buf_info = input_array.request();

        // check dimensions
        if (buf_info.ndim != 1)
            throw std::runtime_error("Number of dimensions must be 1");

        // Create new array for output
        auto result = py::array_t<std::complex<double>>(buf_info.size);

        // Set the number of threads to use for FFTW
        fftw_plan_with_nthreads(omp_get_max_threads());

        // setup FFTW plan
        fftw_plan p = fftw_plan_dft_1d(buf_info.shape[0], reinterpret_cast<fftw_complex *>(buf_info.ptr), reinterpret_cast<fftw_complex *>(result.request().ptr), FFTW_FORWARD, FFTW_ESTIMATE);

        // Release the GIL
        py::gil_scoped_release release{};

        // execute the plan
        fftw_execute(p);

        // Re-acquire the GIL
        py::gil_scoped_acquire acquire{};

        // do cleanup
        fftw_destroy_plan(p);

        return result;
    }
    py::array_t<std::complex<double>> fftw1d_omp(py::array_t<std::complex<double>, py::array::c_style | py::array::forcecast> input_array)
    {
        // request a mutable buffer proxy to the data
        py::buffer_info buf_info = input_array.request();

        // check dimensions
        if (buf_info.ndim != 1)
            throw std::runtime_error("Number of dimensions must be 1");

        // Create new array for output
        auto result = py::array_t<std::complex<double>>(buf_info.size);

        // Set the number of threads to use for FFTW
        fftw_plan_with_nthreads(omp_get_max_threads());

        // setup FFTW plan
        fftw_plan p = fftw_plan_dft_1d(buf_info.shape[0], reinterpret_cast<fftw_complex *>(buf_info.ptr), reinterpret_cast<fftw_complex *>(result.request().ptr), FFTW_FORWARD, FFTW_ESTIMATE);

        // execute the plan
        fftw_execute(p);

        // do cleanup
        fftw_destroy_plan(p);

        return result;
    }
    py::array_t<std::complex<double>> fftw1d(py::array_t<std::complex<double>, py::array::c_style | py::array::forcecast> input_array)
    {
        // for comparison, reset nthread used
        fftw_plan_with_nthreads(1);
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
}