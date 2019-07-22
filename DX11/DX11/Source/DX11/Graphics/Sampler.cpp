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
    , m_Filter(D3D11_FILTER_ANISOTROPIC)
    , m_AddressU(D3D11_TEXTURE_ADDRESS_WRAP)
    , m_AddressV(D3D11_TEXTURE_ADDRESS_WRAP)
    , m_AddressW(D3D11_TEXTURE_ADDRESS_WRAP)
{
    Init(
        m_Filter,
        m_AddressU,
        m_AddressV,
        m_AddressW
    );
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

    bool changed = false;

    {
        const char* pSelectedFilter = filterMap[m_Filter].c_str();

        if (ImGui::BeginCombo("Filter", pSelectedFilter))
        {
            for (auto& filter : filterMap)
            {
                bool selected = filter.first == m_Filter;

                if (ImGui::Selectable(filter.second.c_str(), &selected))
                {
                    changed = true;
                    m_Filter = filter.first;
                }

                if (selected)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }
    }

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

    selectAddressMode("AddressModeU", m_AddressU);
    selectAddressMode("AddressModeV", m_AddressV);
    selectAddressMode("AddressModeW", m_AddressW);

    if (changed)
    {
        Init(
            m_Filter,
            m_AddressU,
            m_AddressV,
            m_AddressW
        );
    }
}


bool Sampler::Init(
    D3D11_FILTER filter,
    D3D11_TEXTURE_ADDRESS_MODE addressU,
    D3D11_TEXTURE_ADDRESS_MODE addressV,
    D3D11_TEXTURE_ADDRESS_MODE addressW
)
{
    ComPtr<ID3D11SamplerState> sampler;

    bool result = DX11Util::CreateSamplerState(
        m_Device,
        filter,
        addressU,
        addressV,
        addressW,
        sampler
    );

    if (result)
    {
        m_SamplerState = sampler;
        m_Filter = filter;
        m_AddressU = addressU;
        m_AddressV = addressV;
        m_AddressW = addressW;
    }

    return result;
}


void Sampler::Set()
{
    ID3D11SamplerState* pSamplerStates[] = {
        m_SamplerState.Get()
    };
    m_Context->PSSetSamplers(0, _countof(pSamplerStates), pSamplerStates);
}
