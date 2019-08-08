#include "ImGui_DX11.h"
#include <map>


bool ImGui_DX11::Init(const InitParams& params)
{
    ImGui::CreateContext();
    ImGui_ImplWin32_Init(params.hwnd);
    ImGui_ImplDX11_Init(params.device, params.context);

    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = params.iniFilename;
    io.LogFilename = params.logFilename;

    ImFontConfig config{};
    config.MergeMode = params.mergeMode;
    if (params.mergeMode) io.Fonts->AddFontDefault();
    if (params.font != nullptr)
    {
        static const ImWchar ranges[] = { 0x0020, 0xFFFF, 0 };
        io.Fonts->AddFontFromFileTTF(params.font, params.fontSize, &config, ranges);
    }

    ImGuiStyle& style = ImGui::GetStyle();
    style.Colors[ImGuiCol_WindowBg].w = 0.5f;

    return true;
}


void ImGui_DX11::Shutdown()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void ImGui_DX11::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam);
}

void ImGui_DX11::SetDisplaySize(int width, int height)
{
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize.x = static_cast<float>(width);
    io.DisplaySize.y = static_cast<float>(height);
}

void ImGui_DX11::NewFrame()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void ImGui_DX11::Render()
{
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}


//----------------------
// Enum
//----------------------

// D3D11_BLEND
bool ImGui_DX11::ComboEnum(const char* label, D3D11_BLEND* p_blend)
{
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

    assert(p_blend != nullptr);

    bool changed = false;
    const char* pSelectedBlend = blendMap[(*p_blend)].c_str();

    if (ImGui::BeginCombo(label, pSelectedBlend))
    {
        for (auto& blend : blendMap)
        {
            bool selected = blend.first == (*p_blend);

            if (ImGui::Selectable(blend.second.c_str(), &selected))
            {
                changed = true;
                (*p_blend) = blend.first;
            }

            if (selected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    return changed;
}

// D3D11_BLEND_OP
bool ImGui_DX11::ComboEnum(const char* label, D3D11_BLEND_OP* p_blend_op)
{
    static std::map<D3D11_BLEND_OP, std::string> blendOPMap = {
        { D3D11_BLEND_OP_ADD,           "ADD"            },
        { D3D11_BLEND_OP_SUBTRACT,      "SUBTRACT"       },
        { D3D11_BLEND_OP_REV_SUBTRACT,  "REV_SUBTRACT"   },
        { D3D11_BLEND_OP_MIN,           "MIN"            },
        { D3D11_BLEND_OP_MAX,           "MAX"            },
    };

    assert(p_blend_op != nullptr);

    bool changed = false;
    const char* pSelectedBlendOP = blendOPMap[(*p_blend_op)].c_str();

    if (ImGui::BeginCombo(label, pSelectedBlendOP))
    {
        for (auto& blendOP : blendOPMap)
        {
            bool selected = blendOP.first == (*p_blend_op);

            if (ImGui::Selectable(blendOP.second.c_str(), &selected))
            {
                changed = true;
                (*p_blend_op) = blendOP.first;
            }

            if (selected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    return changed;
}

// D3D11_FILTER
bool ImGui_DX11::ComboEnum(const char* label, D3D11_FILTER* p_filter)
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

    assert(p_filter != nullptr);

    bool changed = false;
    const char* pSelectedFilter = filterMap[(*p_filter)].c_str();

    if (ImGui::BeginCombo(label, pSelectedFilter))
    {
        for (auto& filter : filterMap)
        {
            bool selected = filter.first == (*p_filter);

            if (ImGui::Selectable(filter.second.c_str(), &selected))
            {
                changed = true;
                (*p_filter) = filter.first;
            }

            if (selected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    return changed;
}

// D3D11_TEXTURE_ADDRESS_MODE
bool ImGui_DX11::ComboEnum(const char* label, D3D11_TEXTURE_ADDRESS_MODE* p_texture_address_mode)
{
    static std::map<D3D11_TEXTURE_ADDRESS_MODE, std::string> addressModeMap = {
        { D3D11_TEXTURE_ADDRESS_WRAP,        "WRAP"         },
        { D3D11_TEXTURE_ADDRESS_MIRROR,      "MIRROR"       },
        { D3D11_TEXTURE_ADDRESS_CLAMP,       "CLAMP"        },
        { D3D11_TEXTURE_ADDRESS_BORDER,      "BORDER"       },
        { D3D11_TEXTURE_ADDRESS_MIRROR_ONCE, "MIRROR_ONCE"  },
    };

    assert(p_texture_address_mode != nullptr);

    bool changed = false;
    const char* pSelectedAddressMode = addressModeMap[(*p_texture_address_mode)].c_str();

    if (ImGui::BeginCombo(label, pSelectedAddressMode))
    {
        for (auto& addressMode : addressModeMap)
        {
            bool selected = addressMode.first == (*p_texture_address_mode);

            if (ImGui::Selectable(addressMode.second.c_str(), &selected))
            {
                changed = true;
                (*p_texture_address_mode) = addressMode.first;
            }

            if (selected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    return changed;
}

// D3D11_COMPARISON_FUNC
bool ImGui_DX11::ComboEnum(const char* label, D3D11_COMPARISON_FUNC* p_comparison_func)
{
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

    assert(p_comparison_func != nullptr);

    bool changed = false;
    const char* pSelectedComparisonFunc = comparisonFuncMap[(*p_comparison_func)].c_str();

    if (ImGui::BeginCombo(label, pSelectedComparisonFunc))
    {
        for (auto& comparisonFunc : comparisonFuncMap)
        {
            bool selected = comparisonFunc.first == (*p_comparison_func);

            if (ImGui::Selectable(comparisonFunc.second.c_str(), &selected))
            {
                changed = true;
                (*p_comparison_func) = comparisonFunc.first;
            }

            if (selected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    return changed;
}

// D3D11_FILL_MODE
bool ImGui_DX11::ComboEnum(const char* label, D3D11_FILL_MODE* p_fill_mode)
{
    static std::map<D3D11_FILL_MODE, std::string> fillModeMap = {
        { D3D11_FILL_WIREFRAME, "WIREFRAME" },
        { D3D11_FILL_SOLID,     "SOLID"     },
    };

    assert(p_fill_mode != nullptr);

    bool changed = false;
    const char* pSelectedFillMode = fillModeMap[(*p_fill_mode)].c_str();

    if (ImGui::BeginCombo(label, pSelectedFillMode))
    {
        for (auto& fillMode : fillModeMap)
        {
            bool selected = fillMode.first == (*p_fill_mode);

            if (ImGui::Selectable(fillMode.second.c_str(), &selected))
            {
                changed = true;
                (*p_fill_mode) = fillMode.first;
            }

            if (selected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    return changed;
}

// D3D11_CULL_MODE
bool ImGui_DX11::ComboEnum(const char* label, D3D11_CULL_MODE* p_cull_mode)
{
    static std::map<D3D11_CULL_MODE, std::string> cullModeMap = {
        { D3D11_CULL_NONE, "NONE"   },
        { D3D11_CULL_FRONT, "FRONT" },
        { D3D11_CULL_BACK,  "BACK"  },
    };

    assert(p_cull_mode != nullptr);

    bool changed = false;
    const char* pSelectedCullMode = cullModeMap[(*p_cull_mode)].c_str();

    if (ImGui::BeginCombo(label, pSelectedCullMode))
    {
        for (auto& cullMode : cullModeMap)
        {
            bool selected = cullMode.first == (*p_cull_mode);

            if (ImGui::Selectable(cullMode.second.c_str(), &selected))
            {
                changed = true;
                (*p_cull_mode) = cullMode.first;
            }

            if (selected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    return changed;
}


// D3D11_COLOR_WRITE_ENABLE
bool ImGui_DX11::CheckBox_ColorWriteEnable(const char* label, UINT8* p_flags)
{
    static std::map<D3D11_COLOR_WRITE_ENABLE, std::string> colorWriteEnableMap = {
        { D3D11_COLOR_WRITE_ENABLE_RED,     "RED"      },
        { D3D11_COLOR_WRITE_ENABLE_GREEN,   "GREEN"    },
        { D3D11_COLOR_WRITE_ENABLE_BLUE,    "BLUE"     },
        { D3D11_COLOR_WRITE_ENABLE_ALPHA,   "ALPHA"    },
    };

    assert(p_flags != nullptr);

    ImGui::Text(label);

    bool changed = false;

    for (auto& colorWriteEnable : colorWriteEnableMap)
    {
        bool enable = (*p_flags) & colorWriteEnable.first;

        if (ImGui::Checkbox(colorWriteEnable.second.c_str(), &enable))
        {
            changed = true;

            if (enable)
            {
                (*p_flags) |= colorWriteEnable.first;
            }
            else
            {
                (*p_flags) &= ~colorWriteEnable.first;
            }
        }
    }

    return changed;
}

