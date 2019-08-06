#pragma once

#include "DX11/Demo/Demo.h"
#include "DX11/DX11Prerequisites.h"
#include "DX11/External/glm/glm_Prerequisites.h"
#include "DX11/Geometry/Transform.h"
#include "DX11/Geometry/Camera.h"


class Sampler;
class Texture;
class BlendState;
class ConstantBuffer;
class BitmapFont;
class Shader;
class TextureDemo : public Demo
{
    struct ConstantBufferData
    {
        glm::mat4x4 MVPMatrix;
        glm::vec4   Color;
        glm::vec2   UVOffset;
        float       dummy[2];

        ConstantBufferData();

        bool UpdateImGui();
    };

public:
    TextureDemo(
        const ComPtr<ID3D11Device>& device,
        const ComPtr<ID3D11DeviceContext>& context,
        const Size2D& clientSize
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
    ComPtr<ID3D11RasterizerState> m_RasterizerState;

    std::unique_ptr<Shader>         m_Shader;
    std::unique_ptr<Sampler>        m_Sampler;
    std::unique_ptr<BlendState>     m_BlendState;
    std::unique_ptr<Texture>        m_Texture1;
    std::unique_ptr<ConstantBuffer> m_ConstantBuffer1;
    ConstantBufferData              m_ConstantBufferData1;
    std::unique_ptr<Texture>        m_Texture2;
    std::unique_ptr<ConstantBuffer> m_ConstantBuffer2;
    ConstantBufferData              m_ConstantBufferData2;

    Transform   m_Transform1;
    Transform   m_Transform2;
    Camera      m_Camera;
    bool m_ForceUpdateConstantBuffer;

    std::unique_ptr<BitmapFont> m_BitmapFont;
};
