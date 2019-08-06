#pragma once

#include "DX11/DX11Prerequisites.h"

enum SHADER_USE_FLAG : u8
{
    SHADER_USE_FLAG_VS = (1 << 0),
    SHADER_USE_FLAG_PS = (1 << 1),
    SHADER_USE_FLAG_GS = (1 << 2),
    SHADER_USE_FLAG_HS = (1 << 3),
    SHADER_USE_FLAG_CS = (1 << 4),
};

class Shader
{
public:
    Shader(
        const ComPtr<ID3D11Device>& device,
        const ComPtr<ID3D11DeviceContext>& context
    );
    virtual ~Shader();

    bool Init(
        const std::string& shaderCode,
        u8 shaderUseFlags,
        const D3D11_INPUT_ELEMENT_DESC* inputElements = nullptr,
        UINT inputElementsCount = 0
    );

    void UpdateImGui();

    void SetAll();
    void ResetAll();

    void SetInputLayout();
    void SetVS();
    void SetPS();
    void SetGS();
    void SetHS();
    void SetCS();

    void ResetInputLayout();
    void ResetVS();
    void ResetPS();
    void ResetGS();
    void ResetHS();
    void ResetCS();

protected:
    ComPtr<ID3D11Device>        m_Device;
    ComPtr<ID3D11DeviceContext> m_Context;

    std::string m_ShaderCodeEdit; // 編集用
    std::string m_ShaderCode;
    u8          m_ShaderUseFlags;

    const D3D11_INPUT_ELEMENT_DESC* m_InputElements;
    UINT                            m_InputElementsCount;
    ComPtr<ID3D11InputLayout>       m_InputLayout;
    ComPtr<ID3D11VertexShader>      m_VertexShader;
    ComPtr<ID3D11PixelShader>       m_PixelShader;
    ComPtr<ID3D11GeometryShader>    m_GeometryShader;
    ComPtr<ID3D11HullShader>        m_HullShader;
    ComPtr<ID3D11ComputeShader>     m_ComputeShader;
};