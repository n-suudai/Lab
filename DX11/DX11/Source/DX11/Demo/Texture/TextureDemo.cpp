﻿#include "TextureDemo.hpp"
#include "../../Utils/DX11Util.hpp"
#include "../../External/ImGui/ImGui_DX11.h"
#include "../../Graphics/Sampler.hpp"
#include "../../Graphics/Texture.hpp"
#include "../../Graphics/BlendState.hpp"
#include "../../Graphics/ConstantBuffer.hpp"


struct Vertex
{
    float position[2];
    float texcoord[2];
};


const D3D11_INPUT_ELEMENT_DESC inputElements[] = {
    { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT,   0, 0,                            D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,   0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};


TextureDemo::ConstantBufferData::ConstantBufferData()
    : Position(0.0f)
    , UVOffset(0.0f)
    , Color(1.0f)
{

}


bool TextureDemo::ConstantBufferData::UpdateImGui()
{
    bool changed = false;

    changed |= ImGuiEx::DragVec2("Position", &Position, 0.01f);
    changed |= ImGuiEx::DragVec2("UVOffset", &UVOffset, 0.01f);
    changed |= ImGuiEx::DragVec4("Color", &Color, 0.01f);

    return changed;
}


TextureDemo::TextureDemo(
    const ComPtr<ID3D11Device>& device,
    const ComPtr<ID3D11DeviceContext>& context
)
    : m_Device(device)
    , m_Context(context)
    , m_IndexCount(0)
{
    const Vertex vertices[] = {
        { {  0.5f,  0.5f }, { 1.0f, 0.0f } },
        { { -0.5f, -0.5f }, { 0.0f, 1.0f } },
        { {  0.5f, -0.5f }, { 1.0f, 1.0f } },
        { { -0.5f,  0.5f }, { 0.0f, 0.0f } },
    };

    DX11Util::CreateBuffer(
        m_Device,
        vertices,
        _countof(vertices) * sizeof(Vertex),
        D3D11_BIND_VERTEX_BUFFER,
        sizeof(Vertex),
        m_VertexBuffer
    );


    const u16 indices[] = { 0, 2, 1, 0, 1, 3 };
    m_IndexCount = _countof(indices);

    DX11Util::CreateBuffer(
        m_Device,
        indices,
        _countof(indices) * sizeof(u16),
        D3D11_BIND_INDEX_BUFFER,
        sizeof(u16),
        m_IndexBuffer
    );

    m_ShaderCode = R"(
Texture2D diffuseTexture : register(t0);
SamplerState normalSampler : register(s0);

cbuffer CB0 : register(b0)
{
    float2 Position;
    float2 UVOffset;
    float4 Color;
};

struct VS_INPUT {
    float2 position : POSITION;
    float2 texcoord : TEXCOORD;
};

struct VS_OUTPUT {
    float4 position : SV_POSITION;
    float4 color    : COLOR;
    float2 texcoord : TEXCOORD;
};

VS_OUTPUT vs_main(VS_INPUT In) {
    VS_OUTPUT Out = (VS_OUTPUT)0;
    Out.position = float4(In.position.x + Position.x, In.position.y + Position.y, 0, 1);
    Out.color = Color;
    Out.texcoord = In.texcoord + UVOffset;
    return Out;
}

float4 ps_main(VS_OUTPUT In) : SV_TARGET {
    return diffuseTexture.Sample(normalSampler, In.texcoord) * In.color;
}
        )";

    {
        ComPtr<ID3DBlob> code;
        bool result = DX11Util::CompileShader(
            m_ShaderCode.data(),
            m_ShaderCode.size(),
            "vs_5_0",
            "vs_main",
            code
        );

        if (result)
        {
            DX11Util::CreateVertexShaderAndInputLayout(
                m_Device,
                code,
                inputElements,
                _countof(inputElements),
                m_VertexShader,
                m_InputLayout
            );
        }
    }

    {
        ComPtr<ID3DBlob> code;
        bool result = DX11Util::CompileShader(
            m_ShaderCode.data(),
            m_ShaderCode.size(),
            "ps_5_0",
            "ps_main",
            code
        );

        if (result)
        {
            DX11Util::CreatePixelShader(
                m_Device,
                code,
                m_PixelShader
            );
        }
    }

    DX11Util::CreateRasterizerState(
        m_Device,
        D3D11_CULL_BACK,
        FALSE,
        m_RasterizerState
    );

    m_Sampler = std::make_unique<Sampler>(
        m_Device,
        m_Context
        );

    m_BlendState = std::make_unique<BlendState>(
        m_Device,
        m_Context
        );

    m_Texture1 = std::make_unique<Texture>(
        m_Device,
        m_Context
        );
    m_Texture1->Initialize("Assets\\Image\\icon.png");

    m_Texture2 = std::make_unique<Texture>(
        m_Device,
        m_Context
        );
    m_Texture2->Initialize("Assets\\Image\\sample.png");

    m_ConstantBuffer1 = std::make_unique<ConstantBuffer>(
        m_Device,
        m_Context,
        &m_ConstantBufferData1,
        sizeof(ConstantBufferData)
        );

    m_ConstantBuffer2 = std::make_unique<ConstantBuffer>(
        m_Device,
        m_Context,
        &m_ConstantBufferData2,
        sizeof(ConstantBufferData)
        );
}


TextureDemo::~TextureDemo()
{

}


void TextureDemo::Update()
{
    ImGui::Text("ConstantBuffer");
    if (ImGui::TreeNode("ConstantBuffer1"))
    {
        if (m_ConstantBufferData1.UpdateImGui())
        {
            m_ConstantBuffer1->Update(&m_ConstantBufferData1);
        }
        ImGui::TreePop();
    }
    if (ImGui::TreeNode("ConstantBuffer2"))
    {
        if (m_ConstantBufferData2.UpdateImGui())
        {
            m_ConstantBuffer2->Update(&m_ConstantBufferData2);
        }
        ImGui::TreePop();
    }

    ImGui::Separator();

    ImGui::Text("Sampler");
    m_Sampler->UpdateImGui();

    ImGui::Separator();

    ImGui::Text("BlendState");
    m_BlendState->UpdateImGui();

    ImGui::Separator();

    ImGui::Text("Shader");
    if (ImGui::TreeNode("Shader"))
    {
        ImGui::InputTextMultiline(
            "ShaderCode",
            &m_ShaderCode
        );
        if (ImGui::Button("CompileShader"))
        {
            {
                ComPtr<ID3DBlob> code;
                bool result = DX11Util::CompileShader(
                    m_ShaderCode.data(),
                    m_ShaderCode.size(),
                    "vs_5_0",
                    "vs_main",
                    code
                );

                if (result)
                {
                    DX11Util::CreateVertexShaderAndInputLayout(
                        m_Device,
                        code,
                        inputElements,
                        _countof(inputElements),
                        m_VertexShader,
                        m_InputLayout
                    );
                }
            }

            {
                ComPtr<ID3DBlob> code;
                bool result = DX11Util::CompileShader(
                    m_ShaderCode.data(),
                    m_ShaderCode.size(),
                    "ps_5_0",
                    "ps_main",
                    code
                );

                if (result)
                {
                    DX11Util::CreatePixelShader(
                        m_Device,
                        code,
                        m_PixelShader
                    );
                }
            }
        }

        ImGui::TreePop();
    }
}


void TextureDemo::Render()
{
    // 入力レイアウトを設定
    m_Context->IASetInputLayout(m_InputLayout.Get());

    // 頂点バッファを設定
    {
        constexpr UINT stride = sizeof(Vertex);
        UINT offset = 0;

        ID3D11Buffer* pVertexBuffers[] = {
            m_VertexBuffer.Get()
        };
        m_Context->IASetVertexBuffers(
            0,
            _countof(pVertexBuffers),
            pVertexBuffers,
            &stride,
            &offset
        );
    }

    // インデックスバッファを設定
    m_Context->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

    // プリミティブトポロジーの設定
    m_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // 頂点シェーダーの設定
    m_Context->VSSetShader(m_VertexShader.Get(), nullptr, 0);

    // ピクセルシェーダーを設定
    m_Context->PSSetShader(m_PixelShader.Get(), nullptr, 0);

    m_Context->RSSetState(m_RasterizerState.Get());

    m_Sampler->Set();

    m_BlendState->Set();

    m_ConstantBuffer1->SetVS();
    RenderTexture(m_Texture1.get());

    m_ConstantBuffer2->SetVS();
    RenderTexture(m_Texture2.get());
}


void TextureDemo::OnResizedBuffer(const Size2D&)
{

}


void TextureDemo::RenderTexture(Texture* pTexture)
{
    pTexture->Bind(0);

    m_Context->DrawIndexed(m_IndexCount, 0, 0);
}

