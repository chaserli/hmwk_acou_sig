#include <vector>
#include <cmath>
#include <numeric>
#include <numbers>
#include <mutex>

#include <fftw3.h>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include <omp.h>

namespace py = pybind11;

std::mutex fftw_plan_mutex;

std::vector<double> HannWindow(int size)
{
    std::vector<double> window(size);
    for (int i = 0; i < size; i++)
        window[i] = 0.5 * (1 - std::cos(2 * std::numbers::pi * i / (size - 1)));
    return window;
}

double ScaleFactor(const std::vector<double> &window, double fs, bool isDensity = true)
{
    if (isDensity)
        return 1.0 / (fs * std::inner_product(window.cbegin(), window.cend(), window.cbegin(), 0.0));
    else // "spectrum"
        return 1.0 / std::pow(std::accumulate(window.cbegin(), window.cend(), 0.0), 2);
}

std::vector<double> welchPSD(py::array_t<double, py::array::c_style | py::array::forcecast> inputArray, int nperseg, int noverlap, double fs, const std::string &scaling)
{
    py::buffer_info bufInfo = inputArray.request();
    double *ptr = static_cast<double *>(bufInfo.ptr);
    int inputSize = bufInfo.size;

    std::vector<double> window = HannWindow(nperseg);
    double scale = ScaleFactor(window, fs, scaling == "density");

    int step = nperseg - noverlap;
    int num_segments = (inputSize - noverlap) / step;
    std::vector<double> psd(nperseg / 2 + 1, 0.0);

{
    py::gil_scoped_release release;

    #pragma omp parallel for
    for (int i = 0; i < num_segments; i++)
    {
        std::vector<double> segment(nperseg);
        double seg_mean = std::accumulate(ptr + i * step, ptr + i * step + nperseg, 0.0) / nperseg;
        for (int j = 0; j < nperseg; j++)
            segment[j] = (ptr[i * step + j] - seg_mean) * window[j];

        fftw_complex *out = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * (nperseg / 2 + 1));
        fftw_plan p;

        {
            std::lock_guard<std::mutex> lock(fftw_plan_mutex);
            p = fftw_plan_dft_r2c_1d(nperseg, segment.data(), out, FFTW_ESTIMATE);
        }

        fftw_execute(p);
        fftw_destroy_plan(p);

        #pragma omp critical
        for (int j = 0; j < nperseg / 2 + 1; j++)
            psd[j] += (out[j][0] * out[j][0] + out[j][1] * out[j][1]) / num_segments;

            fftw_free(out);
        }

    py::gil_scoped_acquire acquire;
}

    for (double &val : psd)
        val *= scale;

    return psd;
}

PYBIND11_MODULE(testwelch, m)
{
    py::print("this welch impl is still incorrect");
    m.def("my_welch", &welchPSD, "Calculate Welch PSD",
          py::arg("input_signal"),
          py::arg("nperseg") = 256,
          py::arg("noverlap") = 128,
          py::arg("fs") = 1,
          py::arg("scaling") = "density");
}
