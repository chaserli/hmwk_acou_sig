#pragma once
#include <complex>
#include <numbers>
#include <bit>
#include <concepts>
#include <cmath>
#include <string>

#include <Python.h>

#define _WSTR(x) _WSTR_(x)
#define _WSTR_(x) L ## #x
#define _STR(x) _STR_(x)
#define _STR_(x) #x

namespace TmpMetaInfo{

constexpr const char* CompilerInfo=

#ifdef __clang__
           "Clang version: " _STR(__clang_major__) "." _STR(__clang_minor__) "." _STR(__clang_patchlevel__) " "
#endif

#if defined(__GNUC__) || defined(__GNUG__)
           "GCC version: " _STR(__GNUC__) "." _STR(__GNUC_MINOR__) "." _STR(__GNUC_PATCHLEVEL__)
#elif defined(_MSC_VER)
           "MSVC version: " _STR(_MSC_FULL_VER)
#elif !defined(__clang__)
           "An unknown compiler"
#endif
               " on "

#ifdef _WIN32
               "Windows"
#elif defined(__APPLE__)
    "Mac OS"
#elif defined(__linux__)
               "Linux"
#endif
 " for Python " PY_VERSION
       ;

constexpr const char *CompileTime = __DATE__ "," __TIME__;
}
