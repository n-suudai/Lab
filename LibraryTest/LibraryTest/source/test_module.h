#pragma once


#ifndef MODULEAPI

#if defined(USE_MODULES)

#define MODULEAPI extern "C" __declspec(dllimport)

#elif defined(BUILD_MODULES)

#define MODULEAPI extern "C" __declspec(dllexport)

#else

#error "USE_MODULES or BUILD_MODULES not defined."

#endif

#endif


class IModule
{
public:
    virtual ~IModule() = default;

    virtual const char* getName() const = 0;
};


MODULEAPI IModule* test_func();

