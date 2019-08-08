#include "ConstantBuffer.h"
#include "DX11/Utils/DX11Util.h"



BufferResource::BufferResource(
    const ComPtr<ID3D11Device>& device,
    const ComPtr<ID3D11DeviceContext>& context,
    void* pInitData,
    size_t size,
    D3D11_BIND_FLAG bindFlag
)
    : m_Device(device)
    , m_Context(context)
    , m_BindFlag(bindFlag)
{
    DX11Util::CreateBuffer(
        m_Device,
        pInitData,
        static_cast<UINT>(size),
        D3D11_BIND_CONSTANT_BUFFER,
        static_cast<UINT>(size),
        m_BufferResource
    );
}


BufferResource::~BufferResource()
{

}


ConstantBuffer::ConstantBuffer(
    const ComPtr<ID3D11Device>& device,
    const ComPtr<ID3D11DeviceContext>& context,
    void* pInitData,
    size_t size
)
    : BufferResource(
        device,
        context,
        pInitData,
        size,
        D3D11_BIND_CONSTANT_BUFFER
    )
{

}


ConstantBuffer::~ConstantBuffer()
{

}


void ConstantBuffer::Update(void* pData)
{
    m_Context->UpdateSubresource(
        m_BufferResource.Get(),
        0,
        nullptr,
        pData,
        0,
        0
    );
}


void ConstantBuffer::SetVS(u32 slot)
{
    ID3D11Buffer* pConstantBuffers[] = {
    m_BufferResource.Get()
    };
    m_Context->VSSetConstantBuffers(slot, _countof(pConstantBuffers), pConstantBuffers);
}


void ConstantBuffer::SetPS(u32 slot)
{
    ID3D11Buffer* pConstantBuffers[] = {
    m_BufferResource.Get()
    };
    m_Context->PSSetConstantBuffers(slot, _countof(pConstantBuffers), pConstantBuffers);
}

