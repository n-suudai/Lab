#include "test_module.h"
#include "test_lib.h"
#include "test_dll.h"


//#pragma comment(lib, "test_lib.lib")


class TestModule final : public IModule
{
public:
    ~TestModule() = default;

    const char* getName() const
    {
        return test_lib::getModuleName2();
    }
};


MODULEAPI IModule* test_func()
{
    Test_DLL* p = create_Test_DLL();

    p->func();

    delete p;

    return new TestModule();
}

