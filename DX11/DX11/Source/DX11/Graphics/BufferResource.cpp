#include "BufferResource.h"
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
        bindFlag,
        static_cast<UINT>(size),
        m_BufferResource
    );
}


BufferResource::~BufferResource()
{

}


void BufferResource::Update(void* pData)
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




VertexBuffer::VertexBuffer(
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
        D3D11_BIND_VERTEX_BUFFER
    )
{

}


VertexBuffer::~VertexBuffer()
{

}


void VertexBuffer::Set(u32 slot, u32 strides, u32 offsets)
{
    ID3D11Buffer* pVertexBuffers[] = {
        m_BufferResource.Get()
    };

    UINT strideSize = strides;
    UINT offsetSize = offsets;
    m_Context->IASetVertexBuffers(slot, _countof(pVertexBuffers), pVertexBuffers, &strideSize, &offsetSize);
}


IndexBuffer::IndexBuffer(
    const ComPtr<ID3D11Device>& device,
    const ComPtr<ID3D11DeviceContext>& context,
    std::vector<u16>& indices
)
    : BufferResource(
        device,
        context,
        indices.data(),
        sizeof(u16) * indices.size(),
        D3D11_BIND_INDEX_BUFFER
    )
{

}

IndexBuffer::~IndexBuffer()
{

}


void IndexBuffer::Set()
{
    m_Context->IASetIndexBuffer(m_BufferResource.Get(), DXGI_FORMAT_R16_UINT, 0);
}

