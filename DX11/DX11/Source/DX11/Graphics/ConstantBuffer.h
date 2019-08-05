#pragma once


#include "DX11/DX11Prerequisites.h"


class ConstantBuffer
{
public:
    ConstantBuffer(
        const ComPtr<ID3D11Device>& device,
        const ComPtr<ID3D11DeviceContext>& context,
        void* pInitData,
        size_t size
    );
    ~ConstantBuffer();

    void Update(void* pData);

    void SetVS(u32 slot = 0);
    void SetPS(u32 slot = 0);

protected:
    ComPtr<ID3D11Device>        m_Device;
    ComPtr<ID3D11DeviceContext> m_Context;
    ComPtr<ID3D11Buffer>        m_ConstantBuffer;
};

