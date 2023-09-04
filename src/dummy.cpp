#include "dummy.hpp"
#include <pybind11/iostream.h>
#include <Eigen/LU>

#ifdef DEBUG
#ifdef __linux__

void __attribute__((constructor)) load_so()
{
std::cout<<"Dll loaded\n";
}

void __attribute__((destructor)) unload_so()
{
std::cout<<"Dll detached\n";
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

Eigen::MatrixXd TestEigen::Times(Eigen::MatrixXd &A, Eigen::MatrixXd &B)
{
    std::ostringstream oss;
    oss << "Product of a " << A.rows() << 'x' << A.cols() << " matrix by a " << B.rows() << 'x' << B.cols() << " matrix, ";
    auto &&res = A * B;
    oss << "resulting a " << res.rows() << "x" << res.cols() << " matrix";
    pybind11::print(oss.str());
    return res;
}
