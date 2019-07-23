#pragma once

#include "../DX11Prerequisites.hpp"


class BlendState
{
public:
    BlendState();

    virtual ~BlendState();

    void UpdateImGui();

    bool Init(
        D3D11_FILTER filter,
        D3D11_TEXTURE_ADDRESS_MODE addressU,
        D3D11_TEXTURE_ADDRESS_MODE addressV,
        D3D11_TEXTURE_ADDRESS_MODE addressW
    );

    void Set();

protected:
    ComPtr<ID3D11Device>        m_Device;
    ComPtr<ID3D11DeviceContext> m_Context;
    ComPtr<ID3D11BlendState>    m_BlendState;

    CD3D11_BLEND_DESC m_BlendDesc;
};

