#include "Sampler.hpp"
#include "../Utils/DX11Util.hpp"
#include "../External/ImGui/ImGui_DX11.h"
#include <map>


Sampler::Sampler(
    const ComPtr<ID3D11Device>& device,
    const ComPtr<ID3D11DeviceContext>& context
)
    : m_Device(device)
    , m_Context(context)
    , m_SamplerDesc(D3D11_DEFAULT)
{
    FLOAT borderColor[4] = { 0.0f };
    m_SamplerDesc = CD3D11_SAMPLER_DESC(
        D3D11_FILTER_ANISOTROPIC,       // サンプリング時に使用するフィルタ。ここでは異方性フィルターを使用する(D3D11_FILTER_ANISOTROPIC)
        D3D11_TEXTURE_ADDRESS_WRAP,     // 0 ～ 1 の範囲外にある u テクスチャー座標の描画方法
        D3D11_TEXTURE_ADDRESS_WRAP,     // 0 ～ 1 の範囲外にある v テクスチャー座標
        D3D11_TEXTURE_ADDRESS_WRAP,     // 0 ～ 1 の範囲外にある w テクスチャー座標
        0,                              // 計算されたミップマップ レベルからのバイアス
        16,                             // サンプリングに異方性補間を使用している場合の限界値。有効な値は 1 ～ 16 
        D3D11_COMPARISON_ALWAYS,        // 比較オプション
        borderColor,                    // 境界色
        0,                              // アクセス可能なミップマップの下限値
        D3D11_FLOAT32_MAX               // アクセス可能なミップマップの上限値
    );

    Init(m_SamplerDesc);
}


Sampler::~Sampler()
{

}


void Sampler::UpdateImGui()
{
    static std::map<D3D11_FILTER, std::string> filterMap = {
        { D3D11_FILTER_MIN_MAG_MIP_POINT,                           "MIN_MAG_MIP_POINT"                             },
        { D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR,                    "MIN_MAG_POINT_MIP_LINEAR"                      },
        { D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT,              "MIN_POINT_MAG_LINEAR_MIP_POINT"                },
        { D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR,                    "MIN_POINT_MAG_MIP_LINEAR"                      },
        { D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT,                    "MIN_LINEAR_MAG_MIP_POINT"                      },
        { D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR,             "MIN_LINEAR_MAG_POINT_MIP_LINEAR"               },
        { D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT,                    "MIN_MAG_LINEAR_MIP_POINT"                      },
        { D3D11_FILTER_MIN_MAG_MIP_LINEAR,                          "MIN_MAG_MIP_LINEAR"                            },
        { D3D11_FILTER_ANISOTROPIC,                                 "ANISOTROPIC"                                   },
        { D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT,                "COMPARISON_MIN_MAG_MIP_POINT"                  },
        { D3D11_FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR,         "COMPARISON_MIN_MAG_POINT_MIP_LINEAR"           },
        { D3D11_FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT,   "COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT"     },
        { D3D11_FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR,         "COMPARISON_MIN_POINT_MAG_MIP_LINEAR"           },
        { D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT,         "COMPARISON_MIN_LINEAR_MAG_MIP_POINT"           },
        { D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR,  "COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR"    },
        { D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT,         "COMPARISON_MIN_MAG_LINEAR_MIP_POINT"           },
        { D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR,               "COMPARISON_MIN_MAG_MIP_LINEAR"                 },
        { D3D11_FILTER_COMPARISON_ANISOTROPIC,                      "COMPARISON_ANISOTROPIC"                        },
        { D3D11_FILTER_MINIMUM_MIN_MAG_MIP_POINT,                   "MINIMUM_MIN_MAG_MIP_POINT"                     },
        { D3D11_FILTER_MINIMUM_MIN_MAG_POINT_MIP_LINEAR,            "MINIMUM_MIN_MAG_POINT_MIP_LINEAR"              },
        { D3D11_FILTER_MINIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT,      "MINIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT"        },
        { D3D11_FILTER_MINIMUM_MIN_POINT_MAG_MIP_LINEAR,            "MINIMUM_MIN_POINT_MAG_MIP_LINEAR"              },
        { D3D11_FILTER_MINIMUM_MIN_LINEAR_MAG_MIP_POINT,            "MINIMUM_MIN_LINEAR_MAG_MIP_POINT"              },
        { D3D11_FILTER_MINIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR,     "MINIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR"       },
        { D3D11_FILTER_MINIMUM_MIN_MAG_LINEAR_MIP_POINT,            "MINIMUM_MIN_MAG_LINEAR_MIP_POINT"              },
        { D3D11_FILTER_MINIMUM_MIN_MAG_MIP_LINEAR,                  "MINIMUM_MIN_MAG_MIP_LINEAR"                    },
        { D3D11_FILTER_MINIMUM_ANISOTROPIC,                         "MINIMUM_ANISOTROPIC"                           },
        { D3D11_FILTER_MAXIMUM_MIN_MAG_MIP_POINT,                   "MAXIMUM_MIN_MAG_MIP_POINT"                     },
        { D3D11_FILTER_MAXIMUM_MIN_MAG_POINT_MIP_LINEAR,            "MAXIMUM_MIN_MAG_POINT_MIP_LINEAR"              },
        { D3D11_FILTER_MAXIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT,      "MAXIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT"        },
        { D3D11_FILTER_MAXIMUM_MIN_POINT_MAG_MIP_LINEAR,            "MAXIMUM_MIN_POINT_MAG_MIP_LINEAR"              },
        { D3D11_FILTER_MAXIMUM_MIN_LINEAR_MAG_MIP_POINT,            "MAXIMUM_MIN_LINEAR_MAG_MIP_POINT"              },
        { D3D11_FILTER_MAXIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR,     "MAXIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR"       },
        { D3D11_FILTER_MAXIMUM_MIN_MAG_LINEAR_MIP_POINT,            "MAXIMUM_MIN_MAG_LINEAR_MIP_POINT"              },
        { D3D11_FILTER_MAXIMUM_MIN_MAG_MIP_LINEAR,                  "MAXIMUM_MIN_MAG_MIP_LINEAR"                    },
        { D3D11_FILTER_MAXIMUM_ANISOTROPIC,                         "MAXIMUM_ANISOTROPIC"                           },
    };
    static std::map<D3D11_TEXTURE_ADDRESS_MODE, std::string> addressModeMap = {
        { D3D11_TEXTURE_ADDRESS_WRAP,        "WRAP"         },
        { D3D11_TEXTURE_ADDRESS_MIRROR,      "MIRROR"       },
        { D3D11_TEXTURE_ADDRESS_CLAMP,       "CLAMP"        },
        { D3D11_TEXTURE_ADDRESS_BORDER,      "BORDER"       },
        { D3D11_TEXTURE_ADDRESS_MIRROR_ONCE, "MIRROR_ONCE"  },
    };
    static std::map<D3D11_COMPARISON_FUNC, std::string> comparisonFuncMap = {
        { D3D11_COMPARISON_NEVER,           "NEVER"            },
        { D3D11_COMPARISON_LESS,            "LESS"             },
        { D3D11_COMPARISON_EQUAL,           "EQUAL"            },
        { D3D11_COMPARISON_LESS_EQUAL,      "LESS_EQUAL"       },
        { D3D11_COMPARISON_GREATER,         "GREATER"          },
        { D3D11_COMPARISON_NOT_EQUAL,       "NOT_EQUAL"        },
        { D3D11_COMPARISON_GREATER_EQUAL,   "GREATER_EQUAL"    },
        { D3D11_COMPARISON_ALWAYS,          "ALWAYS"           },
    };


    bool changed = false;

    CD3D11_SAMPLER_DESC newDesc(m_SamplerDesc);

    // Filter
    {
        const char* pSelectedFilter = filterMap[newDesc.Filter].c_str();

        if (ImGui::BeginCombo("Filter", pSelectedFilter))
        {
            for (auto& filter : filterMap)
            {
                bool selected = filter.first == newDesc.Filter;

                if (ImGui::Selectable(filter.second.c_str(), &selected))
                {
                    changed = true;
                    newDesc.Filter = filter.first;
                }

                if (selected)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }
    }

    // AddressMode
    auto selectAddressMode = [&](const char* label, D3D11_TEXTURE_ADDRESS_MODE& currentAddressMode)
    {
        const char* pSelectedAddressMode = addressModeMap[currentAddressMode].c_str();

        if (ImGui::BeginCombo(label, pSelectedAddressMode))
        {
            for (auto& addressMode : addressModeMap)
            {
                bool selected = addressMode.first == currentAddressMode;

                if (ImGui::Selectable(addressMode.second.c_str(), &selected))
                {
                    changed = true;
                    currentAddressMode = addressMode.first;
                }

                if (selected)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }
    };

    selectAddressMode("AddressModeU", newDesc.AddressU);
    selectAddressMode("AddressModeV", newDesc.AddressV);
    selectAddressMode("AddressModeW", newDesc.AddressW);

    // MipLODBias
    changed |= ImGui::DragFloat("MipLODBias", &newDesc.MipLODBias);

    // MaxAnisotropy
    {
        int value = static_cast<int>(newDesc.MaxAnisotropy);
        if (ImGui::DragInt("MaxAnisotropy", &value))
        {
            changed = true;
            newDesc.MaxAnisotropy = static_cast<UINT>(value);
        }
    }

    // ComparisonFunc
    {
        const char* pSelectedComparisonFunc = comparisonFuncMap[newDesc.ComparisonFunc].c_str();

        if (ImGui::BeginCombo("ComparisonFunc", pSelectedComparisonFunc))
        {
            for (auto& comparisonFunc : comparisonFuncMap)
            {
                bool selected = comparisonFunc.first == newDesc.ComparisonFunc;

                if (ImGui::Selectable(comparisonFunc.second.c_str(), &selected))
                {
                    changed = true;
                    newDesc.ComparisonFunc = comparisonFunc.first;
                }

                if (selected)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }
    }

    // BorderColor
    changed |= ImGui::ColorEdit4("BorderColor", newDesc.BorderColor);

    // MinLOD
    changed |= ImGui::DragFloat("MinLOD", &newDesc.MinLOD);

    // MaxLOD
    changed |= ImGui::DragFloat("MaxLOD", &newDesc.MaxLOD);

    if (changed)
    {
        Init(newDesc);
    }
}


bool Sampler::Init(const CD3D11_SAMPLER_DESC& newDesc)
{
    ComPtr<ID3D11SamplerState> sampler;

    ResultUtil result = m_Device->CreateSamplerState(
        &newDesc,
        &sampler
    );

    if (!result)
    {
        result.ShowMessageBox("device->CreateSamplerState");
        return false;
    }
    else
    {
        m_SamplerState = sampler;
        m_SamplerDesc = newDesc;
    }

    return true;
}


void Sampler::Set()
{
    ID3D11SamplerState* pSamplerStates[] = {
        m_SamplerState.Get()
    };
    m_Context->PSSetSamplers(0, _countof(pSamplerStates), pSamplerStates);
}
