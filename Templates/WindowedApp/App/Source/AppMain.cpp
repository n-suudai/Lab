#define UNUSED(x) (x)



// リサイズ
void OnResize(const Size2D& newSize, void* pUser)
{
    UNUSED(newSize);
    UNUSED(pUser);
}


// キーダウン
void OnKeyDown(KEY_CODE key, void* pUser)
{
    IApp* pApp = reinterpret_cast<IApp*>(pUser);
    if (pApp == nullptr)
    {
        return;
    }

    if (key == KEY_CODE_ESCAPE)
    {
        pApp->PostQuit();
    }
}


// キーアップ
void OnKeyUp(KEY_CODE key, void* pUser)
{
    UNUSED(key);
    UNUSED(pUser);
}


// マウスキーダウン
void OnMouseKeyDown(const Position2D& position, MOUSE_BUTTON button, void* pUser)
{
    UNUSED(position);
    UNUSED(button);
    UNUSED(pUser);
}


// マウスキーアップ
void OnMouseKeyUp(const Position2D& position, MOUSE_BUTTON button, void* pUser)
{
    UNUSED(position);
    UNUSED(button);
    UNUSED(pUser);
}


// マウスホイール
void OnMouseWheel(const Position2D& position, s32 delta, void* pUser)
{
    UNUSED(position);
    UNUSED(delta);
    UNUSED(pUser);
}



// シーン更新処理
void UpdateScene()
{

}


// シーン描画処理
void RenderScene()
{

}



// エントリポイント
void AppMain(int argc, char* argv[])
{
    UNUSED(argc);
    //UNUSED(argv);

    IApp* pApp = nullptr;
    Size2D clientSize = { 640, 480 };

    if (!CreateApp(clientSize, argv[0], &pApp))
    {
        return;
    }

    // コールバックの設定
    AppCallbacks callbacks = { 0 };
    // リサイズ
    callbacks.pOnResize = OnResize;
    callbacks.pOnResizeUser = pApp;
    // キーダウン
    callbacks.pOnKeyDown = OnKeyDown;
    callbacks.pOnKeyDownUser = pApp;
    // キーアップ
    callbacks.pOnKeyUp = OnKeyUp;
    callbacks.pOnKeyUpUser = pApp;
    // マウスキーダウン
    callbacks.pOnMouseKeyDown = OnMouseKeyDown;
    callbacks.pOnMouseKeyDownUser = pApp;
    // マウスキーアップ
    callbacks.pOnMouseKeyUp = OnMouseKeyUp;
    callbacks.pOnMouseKeyUpUser = pApp;
    // マウスホイール
    callbacks.pOnMouseWheel = OnMouseWheel;
    callbacks.pOnMouseWheelUser = pApp;

    pApp->SetAppCallbacks(callbacks);

    // メインループ
    while (pApp->IsLoop())
    {
        UpdateScene();
        RenderScene();
    }

    delete pApp;
}

