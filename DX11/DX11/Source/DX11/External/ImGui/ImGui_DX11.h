#pragma once

#include "DX11/DX11Prerequisites.h"
#include "ImGuiEx.h"


namespace ImGui_DX11
{
    struct InitParams {
        HWND                    hwnd;
        ID3D11Device*           device;
        ID3D11DeviceContext*    context;
        const char*             font;
        float                   fontSize;
        const char*             iniFilename;
        const char*             logFilename;
        bool                    mergeMode; // tureならASCIIはデフォルトフォントで描画される
    };

    bool Init(const InitParams& params);

    void Shutdown();

    void WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    void SetDisplaySize(int width, int height);

    void NewFrame();

    void Render();


    //----------------------
    // Enum
    //----------------------

    // D3D11_BLEND
    bool ComboEnum(const char* label, D3D11_BLEND* p_blend);

    // D3D11_BLEND_OP
    bool ComboEnum(const char* label, D3D11_BLEND_OP* p_blend_op);

    // D3D11_FILTER
    bool ComboEnum(const char* label, D3D11_FILTER* p_filter);

    // D3D11_TEXTURE_ADDRESS_MODE
    bool ComboEnum(const char* label, D3D11_TEXTURE_ADDRESS_MODE* p_texture_address_mode);

    // D3D11_COMPARISON_FUNC
    bool ComboEnum(const char* label, D3D11_COMPARISON_FUNC* p_comparison_func);

    // D3D11_FILL_MODE
    bool ComboEnum(const char* label, D3D11_FILL_MODE* p_fill_mode);

    // D3D11_CULL_MODE
    bool ComboEnum(const char* label, D3D11_CULL_MODE* p_cull_mode);


    //----------------------
    // Flags
    //----------------------

    // D3D11_COLOR_WRITE_ENABLE
    bool CheckBox_ColorWriteEnable(const char* label, UINT8* p_flags);

};
