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

