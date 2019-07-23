#pragma once

#include "../DX11Prerequisites.hpp"


class BlendState
{
public:
    BlendState(
        const ComPtr<ID3D11Device>& device,
        const ComPtr<ID3D11DeviceContext>& context
    );

    virtual ~BlendState();

    void UpdateImGui();

    bool Init(const CD3D11_BLEND_DESC& newDesc);

    void Set();

protected:
    ComPtr<ID3D11Device>        m_Device;
    ComPtr<ID3D11DeviceContext> m_Context;
    ComPtr<ID3D11BlendState>    m_BlendState;

    CD3D11_BLEND_DESC           m_BlendDesc;
    FLOAT                       m_BlendFactor[4];
    UINT                        m_SampleMask;

    int                         m_EditRenderTarget;
};

