#include "Texture.h"
#include "DX11/Utils/DX11Util.h"


Texture::Texture(
    const ComPtr<ID3D11Device>& device,
    const ComPtr<ID3D11DeviceContext> context
)
    : m_Device(device)
    , m_Context(context)
{

}

Texture::~Texture()
{

}

bool Texture::Initialize(const std::string& fileName)
{
    ComPtr<ID3D11Texture2D> texture2D;
    ComPtr<ID3D11ShaderResourceView> shaderResourceView;

    bool result = DX11Util::CreateTextureFromFile(
        m_Device,
        fileName,
        texture2D,
        shaderResourceView
    );

    if (result)
    {
        m_Texture2D = texture2D;
        m_ShaderResourceView = shaderResourceView;
    }

    return result;
}

void Texture::Bind(u32 slot)
{
    if (!m_ShaderResourceView) { return; }

    ID3D11ShaderResourceView* pShaderResourceViews[] = {
        m_ShaderResourceView.Get()
    };

    m_Context->PSSetShaderResources(
        static_cast<UINT>(slot),
        _countof(pShaderResourceViews),
        pShaderResourceViews
    );
}


