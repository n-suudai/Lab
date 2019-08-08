#pragma once


#include "DX11/DX11Prerequisites.h"


class RasterizerState
{
public:
    RasterizerState(
        const ComPtr<ID3D11Device>& device,
        const ComPtr<ID3D11DeviceContext>& context
    );
    virtual ~RasterizerState();

    void UpdateImGui();

    bool Init(const CD3D11_RASTERIZER_DESC& newDesc);

    void Set();

protected:
    ComPtr<ID3D11Device>            m_Device;
    ComPtr<ID3D11DeviceContext>     m_Context;
    ComPtr<ID3D11RasterizerState>   m_RasterizerState;

    CD3D11_RASTERIZER_DESC          m_RasterizerDesc;
};
