#include "BlendState.h"
#include "DX11/Utils/ResultUtil.h"
#include "DX11/External/ImGui/ImGui_DX11.h"
#include <map>


BlendState::BlendState(
    const ComPtr<ID3D11Device>& device,
    const ComPtr<ID3D11DeviceContext>& context
)
    : m_Device(device)
    , m_Context(context)
    , m_BlendDesc(D3D11_DEFAULT)
    , m_SampleMask(0xFFFFFFFF)
    , m_EditRenderTarget(0)
{
    m_BlendFactor[0] = 1.0f;
    m_BlendFactor[1] = 1.0f;
    m_BlendFactor[2] = 1.0f;
    m_BlendFactor[3] = 1.0f;

    // アルファブレンド 加算
    CD3D11_BLEND_DESC blendDesc(D3D11_DEFAULT);
    blendDesc.RenderTarget[0].BlendEnable = TRUE;
    blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    Init(blendDesc);
}


BlendState::~BlendState()
{

}


void BlendState::UpdateImGui()
{
    if (ImGui::TreeNode("BlendState"))
    {
        bool changed = false;
        CD3D11_BLEND_DESC newDesc(m_BlendDesc);

        // AlphaToCoverageEnable
        {
            bool enable = newDesc.AlphaToCoverageEnable == TRUE;
            if (ImGui::Checkbox("AlphaToCoverageEnable", &enable))
            {
                changed = true;
                newDesc.AlphaToCoverageEnable = enable ? TRUE : FALSE;
            }
        }

        // IndependentBlendEnable
        {
            bool enable = newDesc.IndependentBlendEnable == TRUE;
            if (ImGui::Checkbox("IndependentBlendEnable", &enable))
            {
                changed = true;
                newDesc.IndependentBlendEnable = enable ? TRUE : FALSE;
            }
        }

        // RenderTargetIndex
        if (ImGui::InputInt("RenderTarget", &m_EditRenderTarget))
        {
            if (m_EditRenderTarget < 0)
            {
                m_EditRenderTarget = 0;
            }
            else if (m_EditRenderTarget > 7)
            {
                m_EditRenderTarget = 7;
            }
        }

        D3D11_RENDER_TARGET_BLEND_DESC& editRTBDesc = newDesc.RenderTarget[m_EditRenderTarget];

        static std::map<D3D11_COLOR_WRITE_ENABLE, std::string> colorWriteEnableMap = {
            { D3D11_COLOR_WRITE_ENABLE_RED,     "RED"      },
            { D3D11_COLOR_WRITE_ENABLE_GREEN,   "GREEN"    },
            { D3D11_COLOR_WRITE_ENABLE_BLUE,    "BLUE"     },
            { D3D11_COLOR_WRITE_ENABLE_ALPHA,   "ALPHA"    },
        };

        // BlendEnable
        {
            bool enable = editRTBDesc.BlendEnable == TRUE;
            if (ImGui::Checkbox("BlendEnable", &enable))
            {
                changed = true;
                editRTBDesc.BlendEnable = enable ? TRUE : FALSE;
            }
        }

        changed |= ImGui_DX11::ComboEnum("SrcBlend", &editRTBDesc.SrcBlend);
        changed |= ImGui_DX11::ComboEnum("DestBlend", &editRTBDesc.DestBlend);
        changed |= ImGui_DX11::ComboEnum("BlendOp", &editRTBDesc.BlendOp);

        changed |= ImGui_DX11::ComboEnum("SrcBlendAlpha", &editRTBDesc.SrcBlendAlpha);
        changed |= ImGui_DX11::ComboEnum("DestBlendAlpha", &editRTBDesc.DestBlendAlpha);
        changed |= ImGui_DX11::ComboEnum("BlendOpAlpha", &editRTBDesc.BlendOpAlpha);

        // RenderTargetWriteMask
        changed |= ImGui_DX11::CheckBox_ColorWriteEnable(
            "RenderTargetWriteMask", &editRTBDesc.RenderTargetWriteMask);

        // BlendFactor
        ImGui::ColorEdit4("BlendFactor", m_BlendFactor);

        // SampleMask
        ImGuiEx::InputU32("SampleMask", &m_SampleMask, 1, 100, ImGuiInputTextFlags_CharsHexadecimal);

        if (changed)
        {
            Init(newDesc);
        }

        ImGui::TreePop();
    }
}


bool BlendState::Init(const CD3D11_BLEND_DESC& newDesc)
{
    ComPtr<ID3D11BlendState> blendState;

    ResultUtil result = m_Device->CreateBlendState(
        &newDesc,
        &blendState
    );

    if (!result)
    {
        result.ShowMessageBox("device->CreateBlendState");
        return false;
    }
    else
    {
        m_BlendState = blendState;
        m_BlendDesc = newDesc;
    }

    return true;
}


void BlendState::Set()
{
    m_Context->OMSetBlendState(
        m_BlendState.Get(),
        m_BlendFactor,
        m_SampleMask
    );
}
