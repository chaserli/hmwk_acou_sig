from testfftw1d_omp import manualomp as fft1_omp
from testfftw1d_eigen import fwd_fft as fft1_eigen
from testfftw1d_normal import generic as fft1_generic
from testwelch import my_welch
import scipy.fft as sf
import numpy as np
from scipy.signal import welch

if __name__ == "__main__":
    N=7355608
    x = np.random.randn(N)
    print("Testing fft result:")
    res_sf = sf.fft(x)
    res_omp = fft1_omp(x)
    res_eigen = fft1_eigen(x)
    assert(np.allclose(res_sf,res_omp))
    assert(np.allclose(res_sf,res_eigen))
    print("OK\nTesting welch result")
    f1,q1 = welch(x)
    f2,q2 = my_welch(x)
    assert(np.allclose(f1,f2))
    assert(np.allclose(q1,q2,rtol=1e-2))
    print("OK")