#pragma once


#include "DX11/DX11Prerequisites.h"


class BufferResource
{
public:
    BufferResource(
        const ComPtr<ID3D11Device>& device,
        const ComPtr<ID3D11DeviceContext>& context,
        void* pInitData,
        size_t size,
        D3D11_BIND_FLAG bindFlag
    );
    virtual ~BufferResource();

protected:
    ComPtr<ID3D11Device>        m_Device;
    ComPtr<ID3D11DeviceContext> m_Context;
    ComPtr<ID3D11Buffer>        m_BufferResource;
    D3D11_BIND_FLAG             m_BindFlag;
};


class ConstantBuffer : public BufferResource
{
public:
    ConstantBuffer(
        const ComPtr<ID3D11Device>& device,
        const ComPtr<ID3D11DeviceContext>& context,
        void* pInitData,
        size_t size
    );
    virtual ~ConstantBuffer();

    void Update(void* pData);

    void SetVS(u32 slot = 0);
    void SetPS(u32 slot = 0);
};

