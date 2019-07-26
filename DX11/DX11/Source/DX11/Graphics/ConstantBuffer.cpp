#include "ConstantBuffer.hpp"
#include "../Utils/DX11Util.hpp"


ConstantBuffer::ConstantBuffer(
    const ComPtr<ID3D11Device>& device,
    const ComPtr<ID3D11DeviceContext>& context,
    void* pInitData,
    size_t size
)
    : m_Device(device)
    , m_Context(context)
{
    DX11Util::CreateBuffer(
        m_Device,
        pInitData,
        static_cast<UINT>(size),
        D3D11_BIND_CONSTANT_BUFFER,
        static_cast<UINT>(size),
        m_ConstantBuffer
    );
}


ConstantBuffer::~ConstantBuffer()
{

}


void ConstantBuffer::Update(void* pData)
{
    m_Context->UpdateSubresource(
        m_ConstantBuffer.Get(),
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
    m_ConstantBuffer.Get()
    };
    m_Context->VSSetConstantBuffers(slot, _countof(pConstantBuffers), pConstantBuffers);
}


void ConstantBuffer::SetPS(u32 slot)
{
    ID3D11Buffer* pConstantBuffers[] = {
    m_ConstantBuffer.Get()
    };
    m_Context->PSSetConstantBuffers(slot, _countof(pConstantBuffers), pConstantBuffers);
}

