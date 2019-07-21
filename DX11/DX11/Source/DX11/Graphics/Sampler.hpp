#pragma once

#include "../DX11Prerequisites.hpp"


class Sampler
{
public:
    Sampler(
        const ComPtr<ID3D11Device>& device,
        const ComPtr<ID3D11DeviceContext>& context
    );

    virtual ~Sampler();

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
    ComPtr<ID3D11SamplerState>  m_SamplerState;

    D3D11_FILTER                m_Filter;
    D3D11_TEXTURE_ADDRESS_MODE  m_AddressU;
    D3D11_TEXTURE_ADDRESS_MODE  m_AddressV;
    D3D11_TEXTURE_ADDRESS_MODE  m_AddressW;
};

