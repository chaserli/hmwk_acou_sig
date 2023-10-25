from testfftw1d_omp import manualomp as fft1_omp
from testfftw1d_eigen import fwd_fft as fft1_eigen
from testfftw1d_normal import generic as fft1_generic
import scipy.fft as sf
import numpy as np

if __name__ == "__main__":
    N=7355608
    x = np.random.randn(N)
    res_sf = sf.fft(x)
    res_omp = fft1_omp(x)
    res_eigen = fft1_eigen(x)
    assert(np.abs(np.sum(res_sf-res_omp))<1e-5)
    assert(np.abs(np.sum(res_sf-res_eigen))<1e-5)