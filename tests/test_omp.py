import tmphmwk as mynothing
from testfftw1d_omp import manualomp as fftw1_omp
from testfftw1d_eigen import fwd_fft as eigen_fft1
from testfftw1d_normal import forget_wisdom
import time
import scipy.fft as sf
import numpy as np

def time0(x):
    t0=time.time()
    y = sf.fft(x)
    t1=time.time()
    print(f"scipy pocket fft : {t1-t0}")

def time1(x):
    t0=time.time()
    y = eigen_fft1(x)
    t1=time.time()
    print(f"Eigen fft using fftw: {t1-t0}")

def time2(x):
    t0=time.time()
    y = fftw1_omp(x)
    t1=time.time()
    print(f"omp fftw call : {t1-t0}")

def prepare():
    x=[1,2,3,4]
    y = sf.fft(x)
    y = eigen_fft1(x)
    y = fftw1_omp(x)
    return y

if __name__ == "__main__":
    N=55555555
    prepare()
    for i in range(8):
        x = np.random.randn(N)
        print(f'---------run {i} starts--{N}--')
        time0(x)
        time1(x)
        time2(x)
        if i<2:
            continue
        elif(i<4):
            forget_wisdom()
            print('wisdom forgotten')
        else:
            N = np.random.randint(33333333,55555555)

'''
---------run 0 starts--55555555--
scipy pocket fft : 6.879881143569946
numpy fftpack : 16.46525764465332
omp fftw call : 4.555744409561157
---------run 1 starts--55555555--
scipy pocket fft : 6.567472696304321
numpy fftpack : 15.528905630111694
omp fftw call : 4.456118583679199
---------run 2 starts--55555555--
scipy pocket fft : 6.629348516464233
numpy fftpack : 15.7329843044281
omp fftw call : 4.504320383071899
wisdom forgotten
---------run 3 starts--55555555--
scipy pocket fft : 6.448444366455078
numpy fftpack : 15.925235986709595
omp fftw call : 4.587244033813477
wisdom forgotten
---------run 4 starts--55555555--
scipy pocket fft : 7.2491981983184814
numpy fftpack : 17.0102596282959
omp fftw call : 4.661801338195801
---------run 5 starts--36382688--
scipy pocket fft : 12.42493200302124
numpy fftpack : 13.234381675720215
omp fftw call : 3.159186840057373
---------run 6 starts--47969988--
scipy pocket fft : 14.852909803390503
numpy fftpack : 18.552459716796875
omp fftw call : 6.612776517868042
---------run 7 starts--40321768--
scipy pocket fft : 11.5531907081604
numpy fftpack : 14.81855058670044
omp fftw call : 11.473275184631348
'''
