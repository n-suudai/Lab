#include "TextureDemo.hpp"
#include "DX11/Utils/DX11Util.hpp"
#include "DX11/External/ImGui/ImGui_DX11.h"
#include "DX11/Graphics/Sampler.hpp"
#include "DX11/Graphics/Texture.hpp"
#include "DX11/Graphics/BlendState.hpp"
#include "DX11/Graphics/ConstantBuffer.hpp"


struct Vertex
{
    glm::vec4 position;
    glm::vec2 texcoord;
};


const D3D11_INPUT_ELEMENT_DESC inputElements[] = {
    { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,    0, 0,                            D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,          0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};


TextureDemo::ConstantBufferData::ConstantBufferData()
    : MVPMatrix(1.0f)
    , Color(1.0f)
    , UVOffset(0.0f)
{

}


bool TextureDemo::ConstantBufferData::UpdateImGui()
{
    bool changed = false;

    changed |= ImGuiEx::DragVec4("Color", &Color, 0.01f);
    changed |= ImGuiEx::DragVec2("UVOffset", &UVOffset, 0.01f);

    return changed;
}


TextureDemo::TextureDemo(
    const ComPtr<ID3D11Device>& device,
    const ComPtr<ID3D11DeviceContext>& context,
    const Size2D& clientSize
)
    : m_Device(device)
    , m_Context(context)
    , m_IndexCount(0)
    , m_ForceUpdateConstantBuffer(false)
{
    m_Camera.eye = glm::vec3(0.0f, 2.8f, -6.0f);
    m_Camera.center = glm::vec3(0.0f, 0.0f, 0.0f);
    m_Camera.screenSize.x = static_cast<float>(clientSize.width);
    m_Camera.screenSize.y = static_cast<float>(clientSize.height);
    m_Camera.UpdateMatrix();
    m_ForceUpdateConstantBuffer = true;

    m_Transform1.position.x = 640.0f;
    m_Transform1.position.y = 360.0f;
    m_Transform1.scale.x = 640.0f;
    m_Transform1.scale.y = 360.0f;
    m_Transform1.UpdateMatrix();

    m_Transform2.position.x = 640.0f;
    m_Transform2.position.y = 360.0f;
    m_Transform2.scale.x = 640.0f;
    m_Transform2.scale.y = 360.0f;
    m_Transform2.UpdateMatrix();

    // 頂点配列
    const Vertex vertices[] = {
        { {  0.5f,  0.5f, 0.0f, 1.0f }, { 1.0f, 1.0f } },
        { { -0.5f, -0.5f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
        { {  0.5f, -0.5f, 0.0f, 1.0f }, { 1.0f, 0.0f } },
        { { -0.5f,  0.5f, 0.0f, 1.0f }, { 0.0f, 1.0f } },
    };

    // インデックス配列
    const u16 indices[] = { 0, 1, 2, 0, 3, 1 };

    DX11Util::CreateBuffer(
        m_Device,
        vertices,
        _countof(vertices) * sizeof(Vertex),
        D3D11_BIND_VERTEX_BUFFER,
        sizeof(Vertex),
        m_VertexBuffer
    );

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
    float4x4 MVPMatrix;
    float4 Color;
    float2 UVOffset;
    float2 dummy;
};

struct VS_INPUT {
    float4 position : POSITION;
    float2 texcoord : TEXCOORD;
};

struct VS_OUTPUT {
    float4 position : SV_POSITION;
    float4 color    : COLOR;
    float2 texcoord : TEXCOORD;
};

VS_OUTPUT vs_main(VS_INPUT In) {
    VS_OUTPUT Out = (VS_OUTPUT)0;
    Out.position = mul(In.position, MVPMatrix);
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
    auto updateTexture = [&](
        const char* label,
        ConstantBuffer* pCBuffer,
        ConstantBufferData* pCBufferData,
        Transform* pTransform
        )
    {
        bool update = m_ForceUpdateConstantBuffer;

        if (ImGui::TreeNode(label))
        {
            if (pCBufferData->UpdateImGui())
            {
                update = true;
            }

            if (pTransform->UpdateImGui("Transform"))
            {
                update = true;
            }

            ImGui::TreePop();
        }

        if (update)
        {
            pTransform->UpdateMatrix();
            pCBufferData->MVPMatrix = glm::mat4x4(1.0f);
            pCBufferData->MVPMatrix = glm::transpose
            (
                m_Camera.orthoGraphicMatrix * m_Camera.viewMatrix * pTransform->matrix
            );
            pCBuffer->Update(pCBufferData);
        }
    };

    if (m_Camera.UpdateImGui())
    {
        m_Camera.UpdateMatrix();
        m_ForceUpdateConstantBuffer = true;
    }

    updateTexture("Texture1", m_ConstantBuffer1.get(), &m_ConstantBufferData1, &m_Transform1);
    updateTexture("Texture2", m_ConstantBuffer2.get(), &m_ConstantBufferData2, &m_Transform2);
    m_ForceUpdateConstantBuffer = false;

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


void TextureDemo::OnResizedBuffer(const Size2D& size)
{
    m_Camera.screenSize.x = static_cast<float>(size.width);
    m_Camera.screenSize.y = static_cast<float>(size.height);
    m_Camera.UpdateMatrix();
    m_ForceUpdateConstantBuffer = true;
}


void TextureDemo::RenderTexture(Texture* pTexture)
{
    pTexture->Bind(0);

    m_Context->DrawIndexed(m_IndexCount, 0, 0);
}

