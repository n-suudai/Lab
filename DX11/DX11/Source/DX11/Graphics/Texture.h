#pragma once

#include "DX11/DX11Prerequisites.h"
#include <vector>


class Texture
{
public:
    Texture(
        const ComPtr<ID3D11Device>& device,
        const ComPtr<ID3D11DeviceContext> context
    );

    virtual ~Texture();

    bool Init(const std::string& fileName);

    bool InitAsTextureArray(const std::vector<std::string>& fileNames);

    void Bind(u32 slot = 0);

protected:
    ComPtr<ID3D11Device>                m_Device;               // デバイス
    ComPtr<ID3D11DeviceContext>         m_Context;              // デバイスコンテキスト
    ComPtr<ID3D11ShaderResourceView>    m_ShaderResourceView;   // シェーダーリソースビュー
    ComPtr<ID3D11Texture2D>             m_Texture2D;            // テクスチャ
};
