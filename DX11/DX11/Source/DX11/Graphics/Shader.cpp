#include "Shader.h"
#include "DX11/Utils/DX11Util.h"
#include "DX11/External/ImGui/ImGui_DX11.h"


Shader::Shader(
    const ComPtr<ID3D11Device>& device,
    const ComPtr<ID3D11DeviceContext>& context
)
    : m_Device(device)
    , m_Context(context)
    , m_ShaderCodeEdit("")
    , m_ShaderCode("")
    , m_ShaderUseFlags(0)
    , m_InputElements(nullptr)
    , m_InputElementsCount(0)
{

}


Shader::~Shader()
{

}


bool Shader::Init(
    const std::string& shaderCode,
    u8 shaderUseFlags,
    const D3D11_INPUT_ELEMENT_DESC* inputElements,
    UINT inputElementsCount
)
{
    bool success = true;

    if (shaderUseFlags & SHADER_USE_FLAG_VS)
    {
        if (inputElements != nullptr && inputElementsCount > 0)
        {
            ComPtr<ID3DBlob> code;
            bool result = DX11Util::CompileShader(
                shaderCode.data(),
                shaderCode.size(),
                "vs_5_0",
                "vs_main",
                code
            );

            if (result)
            {
                ComPtr<ID3D11VertexShader> vertexShader;
                ComPtr<ID3D11InputLayout> inputLayout;
                result = DX11Util::CreateVertexShaderAndInputLayout(
                    m_Device,
                    code,
                    inputElements,
                    inputElementsCount,
                    vertexShader,
                    inputLayout
                );

                if (result)
                {
                    m_InputElements = inputElements;
                    m_InputElementsCount = inputElementsCount;
                    m_VertexShader = vertexShader;
                    m_InputLayout = inputLayout;
                }
            }

            success &= result;
        }
    }

    if (shaderUseFlags & SHADER_USE_FLAG_PS)
    {
        ComPtr<ID3DBlob> code;
        bool result = DX11Util::CompileShader(
            shaderCode.data(),
            shaderCode.size(),
            "ps_5_0",
            "ps_main",
            code
        );

        if (result)
        {
            ComPtr<ID3D11PixelShader> pixelShader;
            result = DX11Util::CreatePixelShader(
                m_Device,
                code,
                pixelShader
            );

            if (result)
            {
                m_PixelShader = pixelShader;
            }
        }

        success &= result;
    }

    if (shaderUseFlags & SHADER_USE_FLAG_GS)
    {
        ComPtr<ID3DBlob> code;
        bool result = DX11Util::CompileShader(
            shaderCode.data(),
            shaderCode.size(),
            "gs_5_0",
            "gs_main",
            code
        );

        if (result)
        {
            ComPtr<ID3D11GeometryShader> geometryShader;
            result = DX11Util::CreateGeometryShader(
                m_Device,
                code,
                geometryShader
            );

            if (result)
            {
                m_GeometryShader = geometryShader;
            }
        }

        success &= result;
    }

    if (shaderUseFlags & SHADER_USE_FLAG_HS)
    {
        ComPtr<ID3DBlob> code;
        bool result = DX11Util::CompileShader(
            shaderCode.data(),
            shaderCode.size(),
            "hs_5_0",
            "hs_main",
            code
        );

        if (result)
        {
            ComPtr<ID3D11HullShader> hullShader;
            result = DX11Util::CreateHullShader(
                m_Device,
                code,
                hullShader
            );

            if (result)
            {
                m_HullShader = hullShader;
            }
        }

        success &= result;
    }

    if (shaderUseFlags & SHADER_USE_FLAG_CS)
    {
        ComPtr<ID3DBlob> code;
        bool result = DX11Util::CompileShader(
            shaderCode.data(),
            shaderCode.size(),
            "cs_5_0",
            "cs_main",
            code
        );

        if (result)
        {
            ComPtr<ID3D11ComputeShader> computeShader;
            result = DX11Util::CreateComputeShader(
                m_Device,
                code,
                computeShader
            );

            if (result)
            {
                m_ComputeShader = computeShader;
            }
        }

        success &= result;
    }

    if (success)
    {
        m_ShaderUseFlags = shaderUseFlags;
        m_ShaderCodeEdit = m_ShaderCode = shaderCode;
    }

    return success;
}


void Shader::UpdateImGui()
{
    ImGui::InputTextMultiline(
        "ShaderCode",
        &m_ShaderCodeEdit
    );
    if (ImGui::Button("CompileShader"))
    {
        bool result = Init(
            m_ShaderCodeEdit,
            m_ShaderUseFlags,
            m_InputElements,
            m_InputElementsCount
        );
        if (!result)
        {
            m_ShaderCodeEdit = m_ShaderCode; // 編集前の状態に戻す
        }
    }
}


void Shader::SetAll()
{
    SetInputLayout();
    SetVS();
    SetPS();
    SetGS();
    SetHS();
    SetCS();
}


void Shader::ResetAll()
{
    ResetInputLayout();
    ResetVS();
    ResetPS();
    ResetGS();
    ResetHS();
    ResetCS();
}


void Shader::SetInputLayout()
{
    if (m_InputLayout)
    {
        m_Context->IASetInputLayout(m_InputLayout.Get());
    }
}


void Shader::SetVS()
{
    if (m_VertexShader)
    {
        m_Context->VSSetShader(m_VertexShader.Get(), nullptr, 0);
    }
}


void Shader::SetPS()
{
    if (m_PixelShader)
    {
        m_Context->PSSetShader(m_PixelShader.Get(), nullptr, 0);
    }
}


void Shader::SetGS()
{
    if (m_GeometryShader)
    {
        m_Context->GSSetShader(m_GeometryShader.Get(), nullptr, 0);
    }
}


void Shader::SetHS()
{
    if (m_HullShader)
    {
        m_Context->HSSetShader(m_HullShader.Get(), nullptr, 0);
    }
}


void Shader::SetCS()
{
    if (m_ComputeShader)
    {
        m_Context->CSSetShader(m_ComputeShader.Get(), nullptr, 0);
    }
}


void Shader::ResetInputLayout()
{
    m_Context->IASetInputLayout(nullptr);
}


void Shader::ResetVS()
{
    m_Context->VSSetShader(nullptr, nullptr, 0);
}


void Shader::ResetPS()
{
    m_Context->PSSetShader(nullptr, nullptr, 0);
}


void Shader::ResetGS()
{
    m_Context->GSSetShader(nullptr, nullptr, 0);
}


void Shader::ResetHS()
{
    m_Context->HSSetShader(nullptr, nullptr, 0);
}


void Shader::ResetCS()
{
    m_Context->CSSetShader(nullptr, nullptr, 0);
}

