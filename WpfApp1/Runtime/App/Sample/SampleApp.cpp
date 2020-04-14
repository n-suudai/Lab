#include "SampleApp.hpp"
#include "../../TcpProtocol/TcpProtocol.h"
#include "../../TcpProtocol/Peer.h"
#include "../../TcpProtocol/SendImage.h"


// DXGI & D3D11 のライブラリをリンク
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "dxguid.lib")


#define DUSE_MSAA 1


SampleApp::SampleApp(IApp* pApp)
    : m_pApp(pApp)
    , m_BufferCount(2)
    , m_BufferFormat(DXGI_FORMAT_B8G8R8A8_UNORM)
    , m_FeatureLevel(D3D_FEATURE_LEVEL_11_1)
    , m_ImGuiActive(true)
    , m_VisibleObject(1)
{

}

SampleApp::~SampleApp()
{
    Term();
}

// 初期化
bool SampleApp::Init()
{
    ResultUtil result;

    // Tcpサーバーに接続 
    {
        result = TcpProtocol::RemoteEntity::Initialize();
        if (!result)
        {
            ShowErrorMessage(result, "TcpProtocol::RemoteEntity::Initialize");
            return false;
        }

        m_Peer = std::make_unique<TcpProtocol::Peer>();

        result = m_Peer->Connect("127.0.0.1", 9999);
        if (!result)
        {
            ShowErrorMessage(result, "m_Peer->Connect");
            return false;
        }

        m_Peer->Start();
    }

    // ウィンドウハンドルを取得
    HWND hWnd = reinterpret_cast<HWND>(m_pApp->GetWindowHandle());

    // 描画領域のサイズを取得
    const Size2D& clientSize = m_pApp->GetClientSize();

    // ファクトリーを生成
    result = CreateDXGIFactory(IID_PPV_ARGS(&m_Factory));
    if (!result)
    {
        ShowErrorMessage(result, "CreateDXGIFactory");
        return false;
    }

    // デバイス&コンテキストを生成
    {
        // BGRAサポートを有効化
        uint32_t createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(DEBUG) || defined(_DEBUG)
        createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

        D3D_FEATURE_LEVEL featureLevels[] = {
            D3D_FEATURE_LEVEL_11_1,
            D3D_FEATURE_LEVEL_11_0,
        };

        result = D3D11CreateDevice(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE, // ハードウェア ドライバー を使用
            nullptr,
            createDeviceFlags,
            featureLevels,
            _countof(featureLevels),
            D3D11_SDK_VERSION,
            &m_Device,
            &m_FeatureLevel,
            &m_Context
        );
        if (!result)
        {
            ShowErrorMessage(result, "D3D11CreateDevice");
            return false;
        }
    }

    // スワップチェインを生成
    {
#if DUSE_MSAA
        // 使用可能なMSAAを取得
        ZeroMemory(&m_SampleDesc, sizeof(m_SampleDesc));
        for (int i = 1; i <= D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT; i <<= 1)
        {
            UINT Quality;
            if (SUCCEEDED(m_Device->CheckMultisampleQualityLevels(m_BufferFormat, i, &Quality)))
            {
                if (0 < Quality)
                {
                    m_SampleDesc.Count = i;
                    m_SampleDesc.Quality = Quality - 1;
                }
            }
        }
#else
        m_SampleDesc.Count = 1;
        m_SampleDesc.Quality = 0;
#endif

        // スワップチェインを生成
        DXGI_SWAP_CHAIN_DESC swapChainDesc;
        ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
        swapChainDesc.BufferDesc.Width = static_cast<UINT>(clientSize.width);
        swapChainDesc.BufferDesc.Height = static_cast<UINT>(clientSize.height);
        swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
        swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
        swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_CENTERED;
        swapChainDesc.BufferDesc.Format = m_BufferFormat;
        swapChainDesc.SampleDesc = m_SampleDesc;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
        swapChainDesc.BufferCount = m_BufferCount;
        swapChainDesc.Windowed = TRUE;
        swapChainDesc.OutputWindow = hWnd;
        swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

        result = m_Factory->CreateSwapChain(
            m_Device.Get(),
            &swapChainDesc,
            &m_SwapChain
        );
        if (!result)
        {
            ShowErrorMessage(result, "m_Factory->CreateSwapChain");
            return false;
        }
    }

    // バックバッファを生成
    if (!CreateBackBuffer(clientSize))
    {
        return false;
    }

    // ビットマップフォントを生成
    {
        glm::mat4x4 projection = glm::orthoLH(
            0.0f,
            static_cast<f32>(clientSize.width),
            static_cast<f32>(clientSize.height),
            0.0f,
            0.0f,
            1.0f
        );
        projection = glm::transpose(projection);

        m_BitmapFont = std::make_unique<BitmapFont>(
                m_Device,
                m_Context,
                projection
            );
        m_BitmapFont->Initialize("test");
    }

    // シェイプを生成
    {
        {
            glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

            // トーラス
            m_Torus = std::make_unique<Shape>(
                m_Device,
                m_Context
                );
            m_Torus->InitializeAsTorus(50, 50, 0.3f, 1.0f, &color);

            // 球体
            m_Sphere = std::make_unique<Shape>(
                m_Device,
                m_Context
                );
            m_Sphere->InitializeAsSphere(50, 50, 1.0f, &color);

            // 立方体
            m_Cube = std::make_unique<Shape>(
                m_Device,
                m_Context
                );
            m_Cube->InitializeAsCube(2.0f, &color);

            // 円柱
            m_Cylinder = std::make_unique<Shape>(
                m_Device,
                m_Context
                );
            m_Cylinder->InitializeAsCylinder(50, 50, 1.0f, 2.0f, &color);
        }

        // 平行光設定
        {
            glm::vec4 light(8.0f, 2.8f, 6.0f, 1.0f);
            m_Torus->SetLightPosition(light);
            m_Sphere->SetLightPosition(light);
            m_Cube->SetLightPosition(light);
            m_Cylinder->SetLightPosition(light);
        }

        // 環境光設定
        {
            glm::vec4 ambientColor(0.1f, 0.1f, 0.1f, 1.0f);
            m_Torus->SetAmbientColor(ambientColor);
            m_Sphere->SetAmbientColor(ambientColor);
            m_Cube->SetAmbientColor(ambientColor);
            m_Cylinder->SetAmbientColor(ambientColor);
        }

        // 視線ベクトル & ビュー行列 設定
        {
            glm::vec3 eye(0.0f, 2.8f, 6.0f);
            glm::vec3 center(0.0f, 0.0f, 0.0f);

            glm::mat4x4 view = glm::lookAtLH(
                eye,
                center,
                glm::vec3(0.0f, 1.0f, 0.0f)
            );

            m_Torus->SetEyePosition(glm::vec4(eye.x, eye.y, eye.z, 1.0f));
            m_Sphere->SetEyePosition(glm::vec4(eye.x, eye.y, eye.z, 1.0f));
            m_Cube->SetEyePosition(glm::vec4(eye.x, eye.y, eye.z, 1.0f));
            m_Cylinder->SetEyePosition(glm::vec4(eye.x, eye.y, eye.z, 1.0f));

            m_Torus->SetViewMatrix(view);
            m_Sphere->SetViewMatrix(view);
            m_Cube->SetViewMatrix(view);
            m_Cylinder->SetViewMatrix(view);
        }

        // プロジェクション行列設定
        {
            glm::mat4x4 projection = glm::perspectiveFovLH(
                glm::radians(45.0f),
                static_cast<f32>(clientSize.width),
                static_cast<f32>(clientSize.height),
                0.01f,
                100.0f
            );

            m_Torus->SetProjectionMatrix(projection);
            m_Sphere->SetProjectionMatrix(projection);
            m_Cube->SetProjectionMatrix(projection);
            m_Cylinder->SetProjectionMatrix(projection);
        }
    }

    // imgui 初期化
    if (!InitializeImGui())
    {
        return false;
    }

    return true;
}

// 解放
void SampleApp::Term()
{
    // imgui 解放
    TerminateImGui();
}

// 更新処理
void SampleApp::Update()
{
    m_BitmapFont->Put(glm::vec2(10.0f, 10.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), "サンプル");

    {
        static f32 delta = 0.0f;
        delta += 0.003f;

        glm::vec3 translate = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 rotate = glm::vec3(0.0f, 0.0f, delta);
        glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

        m_Torus->Update(&translate, &rotate, &scale);
        m_Sphere->Update(&translate, &rotate, &scale);
        m_Cube->Update(&translate, &rotate, &scale);
        m_Cylinder->Update(&translate, &rotate, &scale);
    }

    if (m_ImGuiActive)
    {
        ImGui::SetNextWindowSize(ImVec2(320, 120), ImGuiCond_Once);
        ImGui::Begin("hoge");

        ImGui::RadioButton("Torus", &m_VisibleObject, 0);
        ImGui::RadioButton("Sphere", &m_VisibleObject, 1);
        ImGui::RadioButton("Cube", &m_VisibleObject, 2);
        ImGui::RadioButton("Cylinder", &m_VisibleObject, 3);

        ImGui::End();

        // imgui 描画
        ImGui::Render();
    }
}

// 描画処理
void SampleApp::Render()
{
    // レンダーターゲットを設定
    ID3D11RenderTargetView* pRenderTargetViews[] = {
        m_RenderTargetView.Get()
    };
    m_Context->OMSetRenderTargets(_countof(pRenderTargetViews), pRenderTargetViews, m_DepthStencilView.Get());

    // 指定色でクリア
    {
        // red, green, blue, alpha
        FLOAT clearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
        m_Context->ClearRenderTargetView(m_RenderTargetView.Get(), clearColor);
    }

    // 深度ステンシルビューをクリア
    m_Context->ClearDepthStencilView(m_DepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    // 深度ステンシルステートを設定
    m_Context->OMSetDepthStencilState(m_DepthStencilState.Get(), 0);

    // シェイプを描画
    if (m_VisibleObject == 0)
    {
        m_Torus->Draw();
    }
    if (m_VisibleObject == 1)
    {
        m_Sphere->Draw();
    }
    if (m_VisibleObject == 2)
    {
        m_Cube->Draw();
    }
    if (m_VisibleObject == 3)
    {
        m_Cylinder->Draw();
    }

    // レンダーターゲットを設定 深度バッファを無効に
    m_Context->OMSetRenderTargets(_countof(pRenderTargetViews), pRenderTargetViews, nullptr);

    // ビットマップフォント描画
    m_BitmapFont->Flush();

    // imgui 描画
    RenderImGui();

    ResultUtil result;

    m_Context->Flush();

    // 描画結果をキャプチャ
    {
        // CPU読み込み可能バッファにGPU上でデータをコピー
        {
#if DUSE_MSAA // マルチサンプル有効な場合、解決処理が必要
            // 解決処理
            m_Context->ResolveSubresource(
                m_ResolveBuffer.Get(), 0,
                m_BackBuffer.Get(), 0,
                m_BufferFormat
            );
            m_Context->CopyResource(m_CaptureBuffer.Get(), m_ResolveBuffer.Get());
#else
            ComPtr<ID3D11Resource> resource;
            m_RenderTargetView->GetResource(&resource);
            m_Context->CopyResource(m_CaptureBuffer.Get(), resource.Get());
#endif            
        }
        
        {
            // GPU上の読み込み可能バッファのメモリアドレスのマップを開く
            D3D11_MAPPED_SUBRESOURCE mappedResource;
            result = m_Context->Map(m_CaptureBuffer.Get(), 0, D3D11_MAP_READ, 0, &mappedResource);
            if (!result)
            {
                ShowErrorMessage(result, "m_Context->Map");
            }

            // CPU上のメモリにバッファを確保
            u32 height = m_BackBufferSize.height;
            u32 src_stride = (u32)mappedResource.RowPitch;    // ※ m_BackBufferSize.width * 4 とは必ずしも一致しない
            size_t buffer_size = src_stride * height;
            
            // Tcpサーバーへ送るクエリ
            std::shared_ptr<TcpProtocol::SendImageQuery> query = std::make_shared<TcpProtocol::SendImageQuery>();
            query->width = static_cast<int>(m_BackBufferSize.width);
            query->height = static_cast<int>(m_BackBufferSize.height);
            query->imageBuffer.resize(buffer_size);

            // バッファコピー
            CopyMemory(query->imageBuffer.data(), mappedResource.pData, buffer_size);

            //// アルファ値の解決
            //for (int y = 0; y < query->height; y++)
            //{
            //    for (int x = 0; x < query->width; x++)
            //    {
            //        int i = 4 * (y * query->width + x);
            //        if (query->imageBuffer[i + 3])
            //        {
            //            query->imageBuffer[i + 3] = 255;
            //        }
            //    }
            //}

            // Tcpサーバーへ送る
            m_Peer->Send(query);

            // アンマップ
            m_Context->Unmap(m_CaptureBuffer.Get(), 0);
        }

    }    

    // 結果をウインドウに反映
    result = m_SwapChain->Present(0, 0);
    if (!result)
    {
        ShowErrorMessage(result, "m_SwapChain->Present");
    }
}

// リサイズ
void SampleApp::OnResize(const Size2D& newSize)
{
    if (m_BackBufferSize.width != newSize.width ||
        m_BackBufferSize.height != newSize.height)
    {
        // バックバッファを再生成
        CreateBackBuffer(newSize);
    }
}

// キー
void SampleApp::OnKey(KEY_CODE key, bool isDown)
{
    if (key == KEY_CODE_ESCAPE && isDown)
    {
        m_pApp->PostQuit();
    }

    if (key == KEY_CODE_I && isDown)
    {
        m_ImGuiActive = !m_ImGuiActive;
    }
}

// マウスボタン
void SampleApp::OnMouse(const Position2D& position, MOUSE_BUTTON button, bool isDown)
{
    position; button; isDown;
}

// マウスホイール
void SampleApp::OnMouseWheel(const Position2D& position, s32 wheelDelta)
{
    position; wheelDelta;
}

// バックバッファを作成
bool SampleApp::CreateBackBuffer(const Size2D& newSize)
{
    m_BackBuffer.Reset();
    m_ResolveBuffer.Reset();
    m_CaptureBuffer.Reset();

    ResultUtil result;

    // バックバッファを破棄
    m_Context->OMSetRenderTargets(0, nullptr, nullptr);
    m_RenderTargetView.Reset();

    // バッファのサイズを変更
    result = m_SwapChain->ResizeBuffers(
        m_BufferCount,
        static_cast<UINT>(newSize.width),
        static_cast<UINT>(newSize.height),
        m_BufferFormat,
        DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
    );
    if (!result)
    {
        ShowErrorMessage(result, "m_SwapChain->ResizeBuffers");
        return false;
    }

    // レンダーターゲットを生成
    {
        result = m_SwapChain->GetBuffer(0, IID_PPV_ARGS(&m_BackBuffer));
        if (!result)
        {
            ShowErrorMessage(result, "m_SwapChain->GetBuffer");
            return false;
        }

#if DUSE_MSAA // マルチサンプル有効な場合、解決処理が必要
        // 解決用バッファ
        {
            D3D11_TEXTURE2D_DESC resolveDesc = {};
            m_BackBuffer->GetDesc(&resolveDesc); // バックバッファの設定をコピー
            resolveDesc.ArraySize = 1;
            resolveDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE; // 一応 ShaderResource として使用できるように
            resolveDesc.MipLevels = 1;
            resolveDesc.MiscFlags = 0;
            resolveDesc.SampleDesc.Count = 1;
            resolveDesc.SampleDesc.Quality = 0;
            resolveDesc.Usage = D3D11_USAGE_DEFAULT;
            result = m_Device->CreateTexture2D(&resolveDesc, nullptr, &m_ResolveBuffer);
            if (!result)
            {
                ShowErrorMessage(result, "m_Device->CreateTexture2D");
                return false;
            }
        }
#endif

        // キャプチャ用バッファを作成
        {
            D3D11_TEXTURE2D_DESC captureDesc = {};
            m_BackBuffer->GetDesc(&captureDesc); // バックバッファの設定をコピー
            captureDesc.ArraySize = 1;
            captureDesc.BindFlags = 0;
            captureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ; // CPU読み取り可能
            captureDesc.MipLevels = 1;
            captureDesc.MiscFlags = 0;
            captureDesc.SampleDesc.Count = 1;
            captureDesc.SampleDesc.Quality = 0;
            captureDesc.Usage = D3D11_USAGE_STAGING; // GPUコピー用として使用？
            result = m_Device->CreateTexture2D(&captureDesc, nullptr, &m_CaptureBuffer);
            if (!result)
            {
                ShowErrorMessage(result, "m_Device->CreateTexture2D");
                return false;
            }
        }

        result = m_Device->CreateRenderTargetView(
            m_BackBuffer.Get(),
            nullptr,
            &m_RenderTargetView
        );
        if (!result)
        {
            ShowErrorMessage(result, "m_Device->CreateRenderTargetView");
            return false;
        }
    }

    // 深度ステンシルビューを生成
    {
        DXGI_FORMAT depthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
        DXGI_FORMAT textureFormat = depthStencilFormat;
        DXGI_FORMAT resourceFormat = depthStencilFormat;

        // テクスチャとシェーダリソースビューのフォーマットを適切なものに変更.
        switch (depthStencilFormat)
        {
        case DXGI_FORMAT_D16_UNORM:
        {
            textureFormat = DXGI_FORMAT_R16_TYPELESS;
            resourceFormat = DXGI_FORMAT_R16_UNORM;
        }
        break;

        case DXGI_FORMAT_D24_UNORM_S8_UINT:
        {
            textureFormat = DXGI_FORMAT_R24G8_TYPELESS;
            resourceFormat = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
        }
        break;

        case DXGI_FORMAT_D32_FLOAT:
        {
            textureFormat = DXGI_FORMAT_R32_TYPELESS;
            resourceFormat = DXGI_FORMAT_R32_FLOAT;
        }
        break;

        case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
        {
            textureFormat = DXGI_FORMAT_R32G8X24_TYPELESS;
            resourceFormat = DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;
        }
        break;
        }

        // 深度ステンシルテクスチャを生成
        {
            CD3D11_TEXTURE2D_DESC texture2DDesc(
                textureFormat,
                static_cast<UINT>(newSize.width),
                static_cast<UINT>(newSize.height),
                1,
                1,
                D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE,
                D3D11_USAGE_DEFAULT,
                0,
                m_SampleDesc.Count,
                m_SampleDesc.Quality,
                0
            );

            result = m_Device->CreateTexture2D(
                &texture2DDesc,
                nullptr,
                &m_DepthStencilTexture
            );
            if (!result)
            {
                ShowErrorMessage(result, "m_Device->CreateTexture2D");
                return false;
            }
        }

        // 深度ステンシルビューを生成
        {
            D3D11_DSV_DIMENSION dimension = D3D11_DSV_DIMENSION_TEXTURE2D;
            if (m_SampleDesc.Count > 0)
            {
                dimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
            }

            CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(
                dimension,
                depthStencilFormat
            );

            result = m_Device->CreateDepthStencilView(
                m_DepthStencilTexture.Get(),
                &depthStencilViewDesc,
                &m_DepthStencilView
            );
            if (!result)
            {
                ShowErrorMessage(result, "m_Device->CreateDepthStencilView");
                return false;
            }
        }
    }

    // レンダーターゲットを設定
    ID3D11RenderTargetView* pRenderTargetViews[] = {
        m_RenderTargetView.Get()
    };
    m_Context->OMSetRenderTargets(_countof(pRenderTargetViews), pRenderTargetViews, m_DepthStencilView.Get());

    // ビューポートを設定
    D3D11_VIEWPORT viewport;
    viewport.Width = static_cast<FLOAT>(newSize.width);
    viewport.Height = static_cast<FLOAT>(newSize.height);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;
    m_Context->RSSetViewports(1, &viewport);

    m_BackBufferSize = newSize;

    return true;
}

// 深度ステンシルステートを作成
bool SampleApp::CreateDepthStencilState()
{
    // 深度ステンシルステートを作成する
    CD3D11_DEPTH_STENCIL_DESC depthStencilDesc(D3D11_DEFAULT);
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

    ResultUtil result = m_Device->CreateDepthStencilState(
        &depthStencilDesc,
        &m_DepthStencilState
    );
    if (!result)
    {
        ShowErrorMessage(result, "m_Device->CreateDepthStencilState");
        return false;
    }
    return true;
}

// エラーメッセージ表示
void SampleApp::ShowErrorMessage(const ResultUtil& result, const std::string& text)
{
    m_pApp->ShowMessageBox(
        result.GetText() + "\n\n" + text,
        "エラー"
    );
}

// imgui 初期化
bool SampleApp::InitializeImGui()
{
    ImGui::CreateContext();

    ResultUtil result = ImGui_ImplDX11_Init(
        m_Device.Get(),
        m_Context.Get()
    );

    if (!result)
    {
        ShowErrorMessage(result, "ImGui_ImplDX11_Init");
        return false;
    }

    result = ImGui_ImplWin32_Init(m_pApp->GetWindowHandle());
    if (!result)
    {
        ShowErrorMessage(result, "ImGui_ImplWin32_Init");
        return false;
    }

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    return true;
}

// imgui 解放
void SampleApp::TerminateImGui()
{
    // imgui 解放
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();

    ImGui::DestroyContext();
}

// imgui 描画
void SampleApp::RenderImGui()
{
    if (m_ImGuiActive)
    {
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
    }
}

