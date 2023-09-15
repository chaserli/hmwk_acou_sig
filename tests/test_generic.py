import tmphmwk as mynothing
from testfftw1dnormal import generic as fftw1_generic
import numpy as np
from testfftw1dnormal import forget_wisdom
import time
import scipy.fft as sf

def time0(x):
    t0=time.time()
    y = sf.fft(x)
    t1=time.time()
    print(f"scipy pocket fft : {t1-t0}")

def time1(x):
    t0=time.time()
    y = np.fft.fft(x)
    t1=time.time()
    print(f"numpy fftpack : {t1-t0}")

def time2(x):
    t0=time.time()
    y = fftw1_generic(x)
    t1=time.time()
    print(f"regular fftw call : {t1-t0}")

def prepare():
    x=[1,2,3,4]
    y = sf.fft(x)
    y = np.fft.fft(x)
    y = fftw1_generic(x)
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
scipy pocket fft : 6.489821434020996
numpy fftpack : 15.444567680358887
regular fftw call : 11.57296872138977
---------run 1 starts--55555555--
scipy pocket fft : 6.395160436630249
numpy fftpack : 15.570196628570557
regular fftw call : 12.00192141532898
---------run 2 starts--55555555--
scipy pocket fft : 7.015598773956299
numpy fftpack : 18.1287944316864
regular fftw call : 12.21267032623291
wisdom forgotten
---------run 3 starts--55555555--
scipy pocket fft : 7.175774574279785
numpy fftpack : 16.380894899368286
regular fftw call : 11.745761156082153
wisdom forgotten
---------run 4 starts--55555555--
scipy pocket fft : 6.3171844482421875
numpy fftpack : 15.460948467254639
regular fftw call : 11.507086992263794
---------run 5 starts--43552779--
scipy pocket fft : 16.59389090538025
numpy fftpack : 41.067885398864746
regular fftw call : 8.269939661026001
---------run 6 starts--55201489--
scipy pocket fft : 17.13475465774536
numpy fftpack : 19.348788022994995
regular fftw call : 12.037051439285278
---------run 7 starts--36041818--
scipy pocket fft : 10.928171396255493
numpy fftpack : 12.015292167663574
regular fftw call : 18.052471160888672
'''
