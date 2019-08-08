#include "RasterizerState.h"
#include "DX11/Utils/ResultUtil.h"
#include "DX11/External/ImGui/ImGui_DX11.h"


RasterizerState::RasterizerState(
    const ComPtr<ID3D11Device>& device,
    const ComPtr<ID3D11DeviceContext>& context
)
    : m_Device(device)
    , m_Context(context)
    , m_RasterizerDesc(D3D11_DEFAULT)
{
    Init(m_RasterizerDesc);
}


RasterizerState::~RasterizerState()
{

}


void RasterizerState::UpdateImGui()
{
    if (ImGui::TreeNode("RasterizerState"))
    {
        CD3D11_RASTERIZER_DESC newDesc(m_RasterizerDesc);

        bool changed = false;

        changed |= ImGui_DX11::ComboEnum("FillMode", &newDesc.FillMode);
        changed |= ImGui_DX11::ComboEnum("CullMode", &newDesc.CullMode);

        {
            bool enable = newDesc.FrontCounterClockwise == TRUE;
            changed |= ImGui::Checkbox("FrontCounterClockwise", &enable);
            newDesc.FrontCounterClockwise = enable ? TRUE : FALSE;
        }
        
        changed |= ImGui::DragInt("DepthBias", &newDesc.DepthBias);
        changed |= ImGui::DragFloat("DepthBiasClamp", &newDesc.DepthBiasClamp, 0.1f);
        changed |= ImGui::DragFloat("SlopeScaledDepthBias", &newDesc.SlopeScaledDepthBias, 0.1f);

        {
            bool enable = newDesc.DepthClipEnable == TRUE;
            changed |= ImGui::Checkbox("DepthClipEnable", &enable);
            newDesc.DepthClipEnable = enable ? TRUE : FALSE;

            enable = newDesc.ScissorEnable == TRUE;
            changed |= ImGui::Checkbox("ScissorEnable", &enable);
            newDesc.ScissorEnable = enable ? TRUE : FALSE;

            enable = newDesc.MultisampleEnable == TRUE;
            changed |= ImGui::Checkbox("MultisampleEnable", &enable);
            newDesc.MultisampleEnable = enable ? TRUE : FALSE;

            enable = newDesc.AntialiasedLineEnable == TRUE;
            changed |= ImGui::Checkbox("AntialiasedLineEnable", &enable);
            newDesc.AntialiasedLineEnable = enable ? TRUE : FALSE;
        }

        if (changed)
        {
            Init(newDesc);
        }

        ImGui::TreePop();
    }
}


bool RasterizerState::Init(const CD3D11_RASTERIZER_DESC& newDesc)
{
    ComPtr<ID3D11RasterizerState> rasterizerState;

    ResultUtil result = m_Device->CreateRasterizerState(
        &newDesc,
        &rasterizerState
    );
    if (!result)
    {
        result.ShowMessageBox("device->CreateRasterizerState");
        return false;
    }
    else
    {
        m_RasterizerDesc = newDesc;
        m_RasterizerState = rasterizerState;
    }

    return true;
}


void RasterizerState::Set()
{
    m_Context->RSSetState(m_RasterizerState.Get());
}

