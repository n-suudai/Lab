#include "test_module.h"
#include "test_lib.h"


//#pragma comment(lib, "test_lib.lib")


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

