#pragma once


#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui_stdlib.h"


class ImGui_DX11
{
public:
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

public:
    static bool Init(InitParams params)
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

        return true;
    }

    static void Shutdown()
    {
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
    }

    static void WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
        ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam);
    }

    static void SetDisplaySize(int width, int height)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize.x = static_cast<float>(width);
        io.DisplaySize.y = static_cast<float>(height);
    }

    static void NewFrame()
    {
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
    }

    static void Render()
    {
        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }
};

namespace ImGui {
    inline bool InputUInt64(const char* label, ImU64* v, ImU64 step = 1, ImU64 step_fast = 100, ImGuiInputTextFlags flags = 0)
    {
        // Hexadecimal input provided as a convenience but the flag name is awkward. Typically you'd use InputText() to parse your own data, if you want to handle prefixes.
        const char* format = (flags & ImGuiInputTextFlags_CharsHexadecimal) ? "%016I64X" : "%d";
        return InputScalar(label, ImGuiDataType_U64, (void*)v, (void*)(step > 0 ? &step : NULL), (void*)(step_fast > 0 ? &step_fast : NULL), format, flags);
    }

    inline bool InputUInt(const char* label, unsigned int* v, unsigned int step = 1, unsigned int step_fast = 100, ImGuiInputTextFlags flags = 0)
    {
        // Hexadecimal input provided as a convenience but the flag name is awkward. Typically you'd use InputText() to parse your own data, if you want to handle prefixes.
        const char* format = (flags & ImGuiInputTextFlags_CharsHexadecimal) ? "%08X" : "%d";
        return InputScalar(label, ImGuiDataType_U32, (void*)v, (void*)(step > 0 ? &step : NULL), (void*)(step_fast > 0 ? &step_fast : NULL), format, flags);
    }
}
