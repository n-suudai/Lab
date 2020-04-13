
#pragma once


class Texture
{
public:
    Texture(
        const ComPtr<ID3D11Device>& device,
        const ComPtr<ID3D11DeviceContext> context
    );

    ~Texture();

    void Initialize(const std::string& fileName);

    void Bind(u32 iSlot = 0);

protected:
    ComPtr<ID3D11Device>                m_Device;               // デバイス
    ComPtr<ID3D11DeviceContext>         m_Context;              // デバイスコンテキスト
    ComPtr<ID3D11ShaderResourceView>    m_ShaderResourceView;   // シェーダーリソースビュー
    ComPtr<ID3D11Texture2D>             m_Texture2D;            // テクスチャ
};

