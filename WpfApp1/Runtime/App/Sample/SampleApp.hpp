#pragma once


namespace TcpProtocol
{
    class Peer;
}


class SampleApp
{
public:
    SampleApp(IApp* pApp);

    ~SampleApp();

    // 初期化
    bool Init();

    // 解放
    void Term();

    // 更新処理
    void Update();

    // 描画処理
    void Render();

    // リサイズ
    void OnResize(const Size2D& newSize);

    // キー
    void OnKey(KEY_CODE key, bool isDown);

    // マウスボタン
    void OnMouse(const Position2D& position, MOUSE_BUTTON button, bool isDown);

    // マウスホイール
    void OnMouseWheel(const Position2D& position, s32 wheelDelta);


private:
    // バックバッファを作成
    bool CreateBackBuffer(const Size2D& newSize);

    // 深度ステンシルステートを作成
    bool CreateDepthStencilState();

    // エラーメッセージ表示
    void ShowErrorMessage(const ResultUtil& result, const std::string& text);

    // imgui 初期化
    bool InitializeImGui();

    // imgui 解放
    void TerminateImGui();

    // imgui 描画
    void RenderImGui();

private:
    IApp * m_pApp;
    UINT m_BufferCount;

    Size2D m_BackBufferSize;

    DXGI_FORMAT       m_BufferFormat;
    D3D_FEATURE_LEVEL m_FeatureLevel;
    DXGI_SAMPLE_DESC  m_SampleDesc;

    ComPtr<IDXGIFactory>   m_Factory;
    ComPtr<IDXGISwapChain> m_SwapChain;

    ComPtr<ID3D11Device>           m_Device;
    ComPtr<ID3D11DeviceContext>    m_Context;
    ComPtr<ID3D11RenderTargetView> m_RenderTargetView;
    ComPtr<ID3D11DepthStencilView> m_DepthStencilView;
    ComPtr<ID3D11Texture2D> m_BackBuffer;
    ComPtr<ID3D11Texture2D> m_ResolveBuffer; // マルチサンプル解決用バッファ https://blog.techlab-xe.net/directx11%E3%81%A7msaa/
    ComPtr<ID3D11Texture2D> m_CaptureBuffer; // キャプチャ用バッファ

    ComPtr<ID3D11Texture2D> m_DepthStencilTexture;
    ComPtr<ID3D11DepthStencilState> m_DepthStencilState;


    std::unique_ptr<BitmapFont> m_BitmapFont;
    std::unique_ptr<Shape>      m_Torus;
    std::unique_ptr<Shape>      m_Sphere;
    std::unique_ptr<Shape>      m_Cube;
    std::unique_ptr<Shape>      m_Cylinder;

    bool                        m_ImGuiActive;
    int                         m_VisibleObject;

    std::unique_ptr<TcpProtocol::Peer> m_Peer;
};


