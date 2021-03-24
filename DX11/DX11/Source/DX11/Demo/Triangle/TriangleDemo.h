#pragma once

#include "DX11/Demo/Demo.h"
#include "DX11/DX11Prerequisites.h"


class TriangleDemo : public Demo
{
public:
    TriangleDemo(
        const ComPtr<ID3D11Device>& device,
        const ComPtr<ID3D11DeviceContext>& context
    );
    
    virtual ~TriangleDemo();

    virtual void Update(float deltaTime) override;

    virtual void Render() override;

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
};
