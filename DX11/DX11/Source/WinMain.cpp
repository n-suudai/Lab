

#if defined(DEBUG) || defined(_DEBUG)
#include <crtdbg.h>
#define EnableMemoryLeakCheck() _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF)
#else
#define EnableMemoryLeakCheck()
#endif


int main(int argc, char* argv[])
{
    EnableMemoryLeakCheck();

    AppMain(argc, argv);

    return 0;
}

