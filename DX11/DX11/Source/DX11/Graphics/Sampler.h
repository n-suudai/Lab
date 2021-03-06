﻿#pragma once

#include "DX11/DX11Prerequisites.h"


class Sampler
{
public:
    Sampler(
        const ComPtr<ID3D11Device>& device,
        const ComPtr<ID3D11DeviceContext>& context
    );

    virtual ~Sampler();

    void UpdateImGui();

    bool Init(const CD3D11_SAMPLER_DESC& newDesc);

    void Set(u32 slot = 0);

protected:
    ComPtr<ID3D11Device>        m_Device;
    ComPtr<ID3D11DeviceContext> m_Context;
    ComPtr<ID3D11SamplerState>  m_SamplerState;
    CD3D11_SAMPLER_DESC         m_SamplerDesc;
};

