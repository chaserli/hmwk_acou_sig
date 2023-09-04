import tmphmwk as mymod
import numpy as np

def test_main():
    print(mymod.CompiledBy)
    print(mymod.add(1,2))
    print(mymod.add(1j,2))
    print(mymod.add('what ',mymod.add('is ','this')))
    print(mymod.add(np.asarray([1,2,3]),np.asarray([4,5,6])))
    print(mymod.add(np.asarray([1,2,3],dtype=np.complex128),np.asarray([4j,5j,6j])))
    assert(abs(mymod.fqrsqrt(0.25)-1/np.sqrt(0.25))<1e-5)

if __name__ == "__main__":
    test_main()
