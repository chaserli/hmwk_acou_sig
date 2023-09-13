import tmphmwk as mymod
import numpy as np
import time
N=55555555
x = np.random.randn(N)
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
    y = mymod.fftw1d_omp_manual(x)
    t1=time.time()
    print(f"omp manual nogil : {t1-t0}")

def prepare():
    t = [1,2,3,4,5,6,7,8]
    y=np.fft.fft(t)
    y=mymod.fftw1d(t)
    y=mymod.fftw1d_omp(t)
    y=mymod.fftw1d_omp_manual(t)
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
    x = np.random.randn(N)
    test_main()
    print('---------try again----------')
    x = np.random.randn(N)
    test_main()


'''
last run result:

numpy pocket fft : 9.233307600021362
regular fftw call : 7.75908350944519
omp fftw call : 3.1535239219665527
omp manual nogil : 3.1531460285186768
---------try again----------
numpy pocket fft : 9.684146404266357
regular fftw call : 7.8256189823150635
omp fftw call : 3.1559600830078125
omp manual nogil : 3.1514058113098145
---------try again----------
numpy pocket fft : 9.338199138641357
regular fftw call : 7.825775861740112
omp fftw call : 3.1905932426452637
omp manual nogil : 3.1539878845214844

'''
