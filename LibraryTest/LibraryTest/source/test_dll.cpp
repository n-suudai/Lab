#include "test_module.h"
#include "test_lib.h"
#include "test_dll.h"
#include <cstdio>


//#pragma comment(lib, "test_lib.lib")

void Test_DLL::func()
{
    printf_s("Test_DLL::func\n");
}

Test_DLL* create_Test_DLL()
{
    return new Test_DLL();
}


class TestModule final : public IModule
{
public:
    ~TestModule() = default;

    const char* getName() const
    {
        return test_lib::getModuleName();
    }
};


MODULEAPI IModule* test_func()
{
    return new TestModule();
}

