#include "test_module.h"
#include <string>
#include <vector>
#include <Windows.h>


typedef IModule* (*TEST_FUNC)();

struct ModuleInfo
{
    IModule* pModule;
    HMODULE hDll;
};

static std::vector<ModuleInfo> s_modules;

bool loadModule(const std::string& filename)
{
    HMODULE hDll = ::LoadLibraryA(filename.c_str());
    if (hDll == NULL)
    {
        LPVOID lpMsgBuf;
        ::FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            GetLastError(),
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
            (LPSTR)&lpMsgBuf,
            0,
            NULL
        );

        ::OutputDebugStringA((char*)lpMsgBuf);

        LocalFree(lpMsgBuf);
        return false;
    }


    TEST_FUNC pFunc = (TEST_FUNC)::GetProcAddress(hDll, ("test_func"));
    if (pFunc == NULL)
        return false;


    IModule* pModule = pFunc();
    if (pModule == NULL)
        return false;


    ModuleInfo info;
    info.pModule = pModule;
    info.hDll = hDll;

    s_modules.push_back(info);

    return true;
}

void unloadAll(void)
{
    for (auto& info : s_modules)
    {
        delete info.pModule;
        ::FreeLibrary(info.hDll);
    }

    s_modules.clear();
}

void printModules()
{
    for (auto& info : s_modules)
    {
        printf_s("Module : %s\n", info.pModule->getName());
    }
}


int main()
{
    if (!loadModule("test_dll.dll"))
    {
        return 0;
    }

    if (!loadModule("test_dll2.dll"))
    {
        return 0;
    }

    printModules();

    unloadAll();

    return 0;
}
