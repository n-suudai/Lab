#pragma once


#ifndef TEST_DLL_API
#if defined(USE_TEST_DLL)

#define TEST_DLL_API __declspec(dllimport)

#elif defined(BUILD_TEST_DLL)

#define TEST_DLL_API __declspec(dllexport)

#else

#error "USE_TEST_DLL or BUILD_TEST_DLL not defined."

#endif
#endif


class Test_DLL
{
public:
    virtual void func() = 0;
};


extern "C" TEST_DLL_API Test_DLL* create_Test_DLL();
