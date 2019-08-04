// DXGI & D3D11 のライブラリをリンク
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "D3DCompiler.lib")


#include "DX11.hpp"
#include "External/ImGui/ImGui_DX11.h"
#include "Utils/DX11Util.hpp"
#include "Demo/Demo.hpp"
#include "Demo/AllDemo.hpp"
#include <thread>


DX11::DX11()
    : m_BufferCount(2)
    , m_BufferFormat(DXGI_FORMAT_R8G8B8A8_UNORM)
    , m_FeatureLevel(D3D_FEATURE_LEVEL_11_1)
    , m_ImGuiActive(true)
    , m_Viewport()
{
    m_ClearColor[0] = 0.0f;
    m_ClearColor[1] = 0.125f;
    m_ClearColor[2] = 0.3f;
    m_ClearColor[3] = 1.0f;
}


DX11::~DX11()
{
    OnTermRequested();
}


void DX11::OnInitEnd()
{
    ResultUtil result;

    // ファクトリーを生成
    result = CreateDXGIFactory(IID_PPV_ARGS(&m_Factory));
    if (!result)
    {
        ShowErrorMessage(result, "CreateDXGIFactory");
        return;
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
            result = D3D11CreateDevice(
                nullptr,
                D3D_DRIVER_TYPE_REFERENCE,
                nullptr,
                createDeviceFlags,
                featureLevels,
                _countof(featureLevels),
                D3D11_SDK_VERSION,
                &m_Device,
                &m_FeatureLevel,
                &m_Context
            );
        }
        if (!result)
        {
            ShowErrorMessage(result, "D3D11CreateDevice");
            return;
        }
    }

    // スワップチェインを生成
    {
        // 使用可能なMSAAを取得
        ZeroMemory(&m_SampleDesc, sizeof(m_SampleDesc));
        for (int i = 1; i <= D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT; i <<= 1)
        {
            UINT Quality;
            if (SUCCEEDED(m_Device->CheckMultisampleQualityLevels(DXGI_FORMAT_D24_UNORM_S8_UINT, i, &Quality)))
            {
                if (0 < Quality)
                {
                    m_SampleDesc.Count = i;
                    m_SampleDesc.Quality = Quality - 1;
                }
            }
        }

        // スワップチェインを生成
        DXGI_SWAP_CHAIN_DESC swapChainDesc;
        ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
        swapChainDesc.BufferDesc.Width = static_cast<UINT>(m_ClientSize.width);
        swapChainDesc.BufferDesc.Height = static_cast<UINT>(m_ClientSize.height);
        swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
        swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
        swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_CENTERED;
        swapChainDesc.BufferDesc.Format = m_BufferFormat;
        swapChainDesc.SampleDesc = m_SampleDesc;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
        swapChainDesc.BufferCount = m_BufferCount;
        swapChainDesc.Windowed = TRUE;
        swapChainDesc.OutputWindow = m_hWnd;
        swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

        result = m_Factory->CreateSwapChain(
            m_Device.Get(),
            &swapChainDesc,
            &m_SwapChain
        );
        if (!result)
        {
            ShowErrorMessage(result, "m_Factory->CreateSwapChain");
            return;
        }
    }

    // バックバッファを生成
    if (!CreateBackBuffer(m_ClientSize))
    {
        return;
    }

    // imgui 初期化
    {
        ImGui_DX11::InitParams initParams = {
            m_hWnd,
            m_Device.Get(),
            m_Context.Get(),
            "c:\\Windows\\Fonts\\meiryo.ttc", // 日本語要らないならnullptrをセット
            16.0f,
            nullptr,
            nullptr,
            false
        };

        if (!ImGui_DX11::Init(initParams))
        {
            result = ResultUtil(false, "Initializing ImGui failed.");
            ShowErrorMessage(result, "ImGui_DX11::Init");
            return;
        }
    }

    std::vector<std::string> demoNames = {
        "Triangle",
        "Texture",
        "DiffuseLighting",
    };

    m_DemoSelector = std::make_unique<DemoSelector>(demoNames);
}


void DX11::OnTermRequested()
{
    // imgui 解放
    ImGui_DX11::Shutdown();
}


void DX11::OnIdle()
{
    static constexpr std::chrono::nanoseconds frame_duration =
        std::chrono::nanoseconds(15666666);

    auto start = std::chrono::high_resolution_clock::now();

    ImGui_DX11::NewFrame();

    Update();

    Render();

    ImGui_DX11::Render();

    Present();

    auto end = std::chrono::high_resolution_clock::now();

    auto duration = frame_duration - (end - start);

    if (duration.count() > 0)
    {
        std::this_thread::sleep_for(
            duration
        );
    }
}


void DX11::OnResize(const Size2D& newSize)
{
    // バックバッファを再生成
    CreateBackBuffer(newSize);
    m_ClientSize = newSize;
    if (m_DemoSelector)
    {
        m_DemoSelector->OnResizedBuffer(newSize);
    }
}


void DX11::OnKeyDown(KEY_CODE key)
{
    if (key == KEY_CODE_ESCAPE)
    {
        PostQuit();
    }

    if (key == KEY_CODE_F1)
    {
        m_ImGuiActive = !m_ImGuiActive;
    }
}


LRESULT CALLBACK DX11::WindowProcedureInstance(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    // ImGuiへ転送
    ImGui_DX11::WndProc(
        hWnd,
        uMsg,
        wParam,
        lParam
    );

    return AppWin::WindowProcedureInstance(
        hWnd,
        uMsg,
        wParam,
        lParam
    );
}


// バックバッファを作成
bool DX11::CreateBackBuffer(const Size2D& newSize)
{
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
        ComPtr<ID3D11Texture2D> backBuffer;

        result = m_SwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
        if (!result)
        {
            ShowErrorMessage(result, "m_SwapChain->GetBuffer");
            return false;
        }

        result = m_Device->CreateRenderTargetView(
            backBuffer.Get(),
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
    m_Context->OMSetRenderTargets(_countof(pRenderTargetViews), pRenderTargetViews, nullptr);

    // ビューポートを設定
    m_Viewport.Width = static_cast<FLOAT>(newSize.width);
    m_Viewport.Height = static_cast<FLOAT>(newSize.height);
    m_Viewport.MinDepth = 0.0f;
    m_Viewport.MaxDepth = 1.0f;
    m_Viewport.TopLeftX = 0.0f;
    m_Viewport.TopLeftY = 0.0f;
    m_Context->RSSetViewports(1, &m_Viewport);
    
    //return DX11Util::CreateRasterizerState(
    //    m_Device,
    //    D3D11_CULL_BACK,
    //    FALSE,
    //    m_RasterizerState
    //);

    return true;
}


// 深度ステンシルステートを作成
bool DX11::CreateDepthStencilState()
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
void DX11::ShowErrorMessage(const ResultUtil& result, const std::string& text)
{
    ShowMessageBox(
        result.GetText() + "\n\n" + text,
        "エラー"
    );
}


void DX11::Update()
{
    if (m_ImGuiActive)
    {
        ImGui::SetNextWindowSize(ImVec2(480, 480), ImGuiCond_FirstUseEver);
        ImGui::Begin("DX11");

        ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::Text("Framebuffer size: (%d, %d)", (int)m_ClientSize.width, (int)m_ClientSize.height);
        ImGui::ColorEdit4("ClearColor", m_ClearColor);

        if (ImGui::TreeNode("Viewport"))
        {
            bool changed = false;

            changed |= ImGui::DragFloat("Width", &m_Viewport.Width, 1.0f, 0.0f, (float)m_ClientSize.width);
            changed |= ImGui::DragFloat("Height", &m_Viewport.Height, 1.0f, 0.0f, (float)m_ClientSize.height);
            changed |= ImGui::DragFloat("TopLeftX", &m_Viewport.TopLeftX, 1.0f, 0.0f, (float)m_ClientSize.width);
            changed |= ImGui::DragFloat("TopLeftY", &m_Viewport.TopLeftY, 1.0f, 0.0f, (float)m_ClientSize.height);
            changed |= ImGui::DragFloat("MinDepth", &m_Viewport.MinDepth, 1.0f, D3D11_MIN_DEPTH, m_Viewport.MaxDepth);
            changed |= ImGui::DragFloat("MaxDepth", &m_Viewport.MaxDepth, 1.0f, m_Viewport.MinDepth, FLT_MAX);

            if (changed)
            {
                m_Context->RSSetViewports(1, &m_Viewport);
            }

            ImGui::TreePop();
        }

        ImGui::Separator();

        m_DemoSelector->UpdateSelector(
            [&](const std::string& name) -> std::unique_ptr<Demo>
            {
                if (name == "Triangle")
                {
                    return std::make_unique<TriangleDemo>(
                        m_Device,
                        m_Context
                        );
                }
                else if (name == "Texture")
                {
                    return std::make_unique<TextureDemo>(
                        m_Device,
                        m_Context,
                        m_ClientSize
                        );
                }
                else if (name == "DiffuseLighting")
                {
                    return std::make_unique<DiffuselightingDemo>(
                        m_Device,
                        m_Context
                        );
                }

                return std::make_unique<Demo>();
            }
        );

        ImGui::Separator();

        m_DemoSelector->UpdateDemo();

        ImGui::End();
    }
}


void DX11::Render()
{
    // 指定色でクリア
    {
        m_Context->ClearRenderTargetView(m_RenderTargetView.Get(), m_ClearColor);
    }

    m_DemoSelector->RenderDemo();
}


void DX11::Present()
{
    // 結果をウインドウに反映
    ResultUtil result = m_SwapChain->Present(0, 0);
    if (!result)
    {
        ShowErrorMessage(result, "m_SwapChain->Present");
    }
}

