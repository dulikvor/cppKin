#pragma once

#if defined(WIN32)
    #ifdef CPPKIN_DLL
    #define CPPKIN_EXPORT __declspec(dllexport)
    #else
    #define CPPKIN_EXPORT __declspec(dllimport)
    #endif
#else
    #define CPPKIN_EXPORT
#endif

