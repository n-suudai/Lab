#define UNUSED(x) (x)


#include "DX11/DX11.hpp"


bool CreateApp(const Size2D& clientSize, const std::string& title, AppWin ** ppOut)
{
    AppWin* pApp = new DX11();
    if (!pApp->Init(clientSize, title))
    {
        delete pApp;
        *ppOut = nullptr;
        return false;
    }

    *ppOut = pApp;
    return true;
}


// エントリポイント
void AppMain(int argc, char* argv[])
{
    UNUSED(argc);
    //UNUSED(argv);

    AppWin* pApp = nullptr;
    Size2D clientSize = { 1280, 720 };

    if (!CreateApp(clientSize, argv[0], &pApp))
    {
        return;
    }

    // メインループ
    while (pApp->IsLoop())
    {
        pApp->Idle();
    }

    delete pApp;
}

