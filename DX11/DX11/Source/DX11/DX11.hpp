#pragma once


#include "../AppWin.hpp"
#include "Utils/ResultUtil.hpp"
#include "DX11Prerequisites.hpp"


class DemoSelector;

class DX11 : public AppWin
{
public:
    DX11();

    virtual ~DX11();

private:
    virtual void OnInitEnd() override;

    virtual void OnTermRequested() override;

    virtual void OnIdle() override;

    virtual void OnResize(const Size2D& newSize) override;

    virtual void OnKeyDown(KEY_CODE key) override;

    virtual LRESULT CALLBACK WindowProcedureInstance(HWND, UINT, WPARAM, LPARAM) override;

private:
    // バックバッファを作成
    bool CreateBackBuffer(const Size2D& newSize);

    // 深度ステンシルステートを作成
    bool CreateDepthStencilState();

    // エラーメッセージ表示
    void ShowErrorMessage(const ResultUtil& result, const std::string& text);


private:
    void Update();

    void Render();

    void Present();

private:
    // DXGI
    UINT                    m_BufferCount;
    DXGI_FORMAT             m_BufferFormat;
    DXGI_SAMPLE_DESC        m_SampleDesc;
    ComPtr<IDXGIFactory>    m_Factory;
    ComPtr<IDXGISwapChain>  m_SwapChain;

    // D3D11
    D3D_FEATURE_LEVEL               m_FeatureLevel;
    ComPtr<ID3D11Device>            m_Device;
    ComPtr<ID3D11DeviceContext>     m_Context;
    ComPtr<ID3D11RenderTargetView>  m_RenderTargetView;
    ComPtr<ID3D11DepthStencilView>  m_DepthStencilView;
    ComPtr<ID3D11Texture2D>         m_DepthStencilTexture;
    ComPtr<ID3D11DepthStencilState> m_DepthStencilState;
    ComPtr<ID3D11RasterizerState>   m_RasterizerState;


    std::unique_ptr<DemoSelector>   m_DemoSelector;
    bool    m_ImGuiActive;
};

