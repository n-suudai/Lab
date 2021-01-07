#include "test_lib.h"

static const char* g_pModuleName = "test";
static const char* g_pModuleName2 = "test2";


namespace test_lib {


const char* getModuleName()
{
    return g_pModuleName;
}

const char* getModuleName2()
{
    return g_pModuleName2;
}


} // namespace test_lib

