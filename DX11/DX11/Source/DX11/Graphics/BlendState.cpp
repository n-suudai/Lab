#include "BlendState.hpp"
#include "../Utils/ResultUtil.hpp"
#include "../External/ImGui/ImGui_DX11.h"
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

    static std::map<D3D11_BLEND, std::string> blendMap = {
        { D3D11_BLEND_ZERO,             "ZERO"              },
        { D3D11_BLEND_ONE,              "ONE"               },
        { D3D11_BLEND_SRC_COLOR,        "SRC_COLOR"         },
        { D3D11_BLEND_INV_SRC_COLOR,    "INV_SRC_COLOR"     },
        { D3D11_BLEND_SRC_ALPHA,        "SRC_ALPHA"         },
        { D3D11_BLEND_INV_SRC_ALPHA,    "INV_SRC_ALPHA"     },
        { D3D11_BLEND_DEST_ALPHA,       "DEST_ALPHA"        },
        { D3D11_BLEND_INV_DEST_ALPHA,   "INV_DEST_ALPHA"    },
        { D3D11_BLEND_DEST_COLOR,       "DEST_COLOR"        },
        { D3D11_BLEND_INV_DEST_COLOR,   "INV_DEST_COLOR"    },
        { D3D11_BLEND_SRC_ALPHA_SAT,    "SRC_ALPHA_SAT"     },
        { D3D11_BLEND_BLEND_FACTOR,     "BLEND_FACTOR"      },
        { D3D11_BLEND_INV_BLEND_FACTOR, "INV_BLEND_FACTOR"  },
        { D3D11_BLEND_SRC1_COLOR,       "SRC1_COLOR"        },
        { D3D11_BLEND_INV_SRC1_COLOR,   "INV_SRC1_COLOR"    },
        { D3D11_BLEND_SRC1_ALPHA,       "SRC1_ALPHA"        },
        { D3D11_BLEND_INV_SRC1_ALPHA,   "INV_SRC1_ALPHA"    },
    };

    static std::map<D3D11_BLEND_OP, std::string> blendOPMap = {
        { D3D11_BLEND_OP_ADD,           "ADD"            },
        { D3D11_BLEND_OP_SUBTRACT,      "SUBTRACT"       },
        { D3D11_BLEND_OP_REV_SUBTRACT,  "REV_SUBTRACT"   },
        { D3D11_BLEND_OP_MIN,           "MIN"            },
        { D3D11_BLEND_OP_MAX,           "MAX"            },
    };

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
    
    auto selectBlend = [&](const char* label, D3D11_BLEND& currentBlend) {
        const char* pSelectedBlend = blendMap[currentBlend].c_str();

        if (ImGui::BeginCombo(label, pSelectedBlend))
        {
            for (auto& blend : blendMap)
            {
                bool selected = blend.first == currentBlend;

                if (ImGui::Selectable(blend.second.c_str(), &selected))
                {
                    changed = true;
                    currentBlend = blend.first;
                }

                if (selected)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }
    };

    auto selectBlendOP = [&](const char* label, D3D11_BLEND_OP& currentBlendOP) {
        const char* pSelectedBlendOP = blendOPMap[currentBlendOP].c_str();

        if (ImGui::BeginCombo(label, pSelectedBlendOP))
        {
            for (auto& blendOP : blendOPMap)
            {
                bool selected = blendOP.first == currentBlendOP;

                if (ImGui::Selectable(blendOP.second.c_str(), &selected))
                {
                    changed = true;
                    currentBlendOP = blendOP.first;
                }

                if (selected)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }
    };

    selectBlend("SrcBlend", editRTBDesc.SrcBlend);
    selectBlend("DestBlend", editRTBDesc.DestBlend);
    selectBlendOP("BlendOp", editRTBDesc.BlendOp);

    selectBlend("SrcBlendAlpha", editRTBDesc.SrcBlendAlpha);
    selectBlend("DestBlendAlpha", editRTBDesc.DestBlendAlpha);
    selectBlendOP("BlendOpAlpha", editRTBDesc.BlendOpAlpha);

    // RenderTargetWriteMask
    ImGui::Text("RenderTargetWriteMask");

    for (auto& colorWriteEnable : colorWriteEnableMap)
    {
        bool enable = editRTBDesc.RenderTargetWriteMask & colorWriteEnable.first;

        if (ImGui::Checkbox(colorWriteEnable.second.c_str(), &enable))
        {
            changed = true;

            if (enable)
            {
                editRTBDesc.RenderTargetWriteMask |= colorWriteEnable.first;
            }
            else
            {
                editRTBDesc.RenderTargetWriteMask &= ~colorWriteEnable.first;
            }
        }
    }

    // BlendFactor
    ImGui::ColorEdit4("BlendFactor", m_BlendFactor);

    // SampleMask
    ImGui::InputUInt("SampleMask", &m_SampleMask, 1, 100, ImGuiInputTextFlags_CharsHexadecimal);

    if (changed)
    {
        Init(newDesc);
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
