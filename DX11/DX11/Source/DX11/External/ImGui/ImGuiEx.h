#pragma once


#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui_stdlib.h"


namespace ImGuiEx
{
    inline bool InputUInt64(const char* label, ImU64* v, ImU64 step = 1, ImU64 step_fast = 100, ImGuiInputTextFlags flags = 0)
    {
        // Hexadecimal input provided as a convenience but the flag name is awkward. Typically you'd use InputText() to parse your own data, if you want to handle prefixes.
        const char* format = (flags & ImGuiInputTextFlags_CharsHexadecimal) ? "%016I64X" : "%d";
        return ImGui::InputScalar(label, ImGuiDataType_U64, (void*)v, (void*)(step > 0 ? &step : NULL), (void*)(step_fast > 0 ? &step_fast : NULL), format, flags);
    }

    inline bool InputUInt(const char* label, unsigned int* v, unsigned int step = 1, unsigned int step_fast = 100, ImGuiInputTextFlags flags = 0)
    {
        // Hexadecimal input provided as a convenience but the flag name is awkward. Typically you'd use InputText() to parse your own data, if you want to handle prefixes.
        const char* format = (flags & ImGuiInputTextFlags_CharsHexadecimal) ? "%08X" : "%d";
        return ImGui::InputScalar(label, ImGuiDataType_U32, (void*)v, (void*)(step > 0 ? &step : NULL), (void*)(step_fast > 0 ? &step_fast : NULL), format, flags);
    }

    inline bool DragUInt(const char* label, unsigned int* v, float v_speed = 1.0f, unsigned int v_min = 0, unsigned int v_max = 0, const char* format = "%d")
    {
        return ImGui::DragScalar(label, ImGuiDataType_U32, v, v_speed, &v_min, &v_max, format);
    }
}

