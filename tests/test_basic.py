import tmphmwk as mymod
import numpy as np
import time
x = np.random.randn(33333333)
def time1():
    t0=time.time()
    y = np.fft.fft(x)
    t1=time.time()
    print(f"numpy pocket fft : {t1-t0}")

def time2():
    t0=time.time()
    y = mymod.fftw1d(x)
    t1=time.time()
    print(f"regular fftw call : {t1-t0}")

def time3():
    t0=time.time()
    y = mymod.fftw1d_omp(x)
    t1=time.time()
    print(f"omp fftw call : {t1-t0}")

def time4():
    t0=time.time()
    y = mymod.fftw1d_ompnogil(x)
    t1=time.time()
    print(f"omp nogil fftw call : {t1-t0}")

def prepare():
    t = [1,2,3,4,5,6,7,8]
    y=np.fft.fft(t)
    y=mymod.fftw1d(t)
    y=mymod.fftw1d_omp(t)
    y=mymod.fftw1d_ompnogil(t)
    return y

def test_main():
    time1()
    time2()
    time3()
    time4()

if __name__ == "__main__":
    prepare()
    test_main()
    print('---------try again----------')
    x = np.random.randn(33333333)
    test_main()
    print('---------try again----------')
    x = np.random.randn(33333333)
    test_main()
