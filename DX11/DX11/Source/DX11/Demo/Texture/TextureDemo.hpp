#pragma once

#include "../Demo.hpp"
#include "../../DX11Prerequisites.hpp"
#include "../../External/glm/glm_Prerequisites.h"


class Sampler;
class Texture;
class BlendState;
class ConstantBuffer;
class TextureDemo : public Demo
{
    struct ConstantBufferData
    {
        glm::vec2 Position;
        glm::vec2 UVOffset;
        glm::vec4 Color;

        ConstantBufferData();

        bool UpdateImGui();
    };

public:
    TextureDemo(
        const ComPtr<ID3D11Device>& device,
        const ComPtr<ID3D11DeviceContext>& context
    );

    virtual ~TextureDemo();

    virtual void Update() override;

    virtual void Render() override;

    virtual void OnResizedBuffer(const Size2D& size) override;

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

    std::string                 m_ShaderCode;

    std::unique_ptr<Sampler>        m_Sampler;
    std::unique_ptr<BlendState>     m_BlendState;
    std::unique_ptr<Texture>        m_Texture1;
    std::unique_ptr<ConstantBuffer> m_ConstantBuffer1;
    ConstantBufferData              m_ConstantBufferData1;
    std::unique_ptr<Texture>        m_Texture2;
    std::unique_ptr<ConstantBuffer> m_ConstantBuffer2;
    ConstantBufferData              m_ConstantBufferData2;
};
