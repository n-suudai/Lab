#pragma once

#include "../Demo.hpp"
#include "../../DX11Prerequisites.hpp"


class Sampler;
class Texture;
class TextureDemo : public Demo
{
public:
    TextureDemo(
        const ComPtr<ID3D11Device>& device,
        const ComPtr<ID3D11DeviceContext>& context
    );

    virtual ~TextureDemo();

    virtual void Update() override;

    virtual void Render() override;

private:
    void RenderTexture(Texture* pTexture);

private:
    ComPtr<ID3D11Device>        m_Device;
    ComPtr<ID3D11DeviceContext> m_Context;

    UINT                        m_IndexCount;
    ComPtr<ID3D11Buffer>        m_VertexBuffer;
    ComPtr<ID3D11Buffer>        m_IndexBuffer;
    ComPtr<ID3D11InputLayout>   m_InputLayout;
    ComPtr<ID3D11VertexShader>  m_VertexShader;
    ComPtr<ID3D11PixelShader>   m_PixelShader;
    ComPtr<ID3D11RasterizerState> m_RasterizerState;
    ComPtr<ID3D11BlendState>      m_BlendState;

    std::string                 m_ShaderCode;

    std::unique_ptr<Sampler> m_Sampler;
    std::unique_ptr<Texture> m_Texture1;
    std::unique_ptr<Texture> m_Texture2;
};
