#include "TriangleDemo.hpp"
#include "DX11/Utils/DX11Util.hpp"
#include "DX11/External/ImGui/ImGui_DX11.h"



struct Vertex
{
    float position[2];
    u32   color;
};


const D3D11_INPUT_ELEMENT_DESC inputElements[] = {
    { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT,   0, 0,                            D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "COLOR",    0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};


TriangleDemo::TriangleDemo(
    const ComPtr<ID3D11Device>& device,
    const ComPtr<ID3D11DeviceContext>& context
)
    : m_Device(device)
    , m_Context(context)
    , m_IndexCount(0)
{
    const Vertex vertices[] = {
        { {  0.0f,  0.5f }, 0xFFFFFFFF },
        { { -0.5f, -0.5f }, 0xFFFFFFFF },
        { {  0.5f, -0.5f }, 0xFFFFFFFF },
    };

    DX11Util::CreateBuffer(
        m_Device,
        vertices,
        _countof(vertices) * sizeof(Vertex),
        D3D11_BIND_VERTEX_BUFFER,
        sizeof(Vertex),
        m_VertexBuffer
    );


    const u16 indices[] = { 0, 2, 1 };
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
struct VS_INPUT {
    float2 position : POSITION;
    float4 color    : COLOR;
};
struct VS_OUTPUT {
    float4 position : SV_POSITION;
    float4 color    : COLOR;
};

VS_OUTPUT vs_main(VS_INPUT In) {
    VS_OUTPUT Out = (VS_OUTPUT)0;
    Out.position = float4(In.position.x, In.position.y, 0, 1);
    Out.color = In.color;
    return Out;
}

float4 ps_main(VS_OUTPUT In) : SV_TARGET {
    return In.color;
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
}


TriangleDemo::~TriangleDemo()
{

}


void TriangleDemo::Update()
{
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


void TriangleDemo::Render()
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

    m_Context->DrawIndexed(m_IndexCount, 0, 0);
}
