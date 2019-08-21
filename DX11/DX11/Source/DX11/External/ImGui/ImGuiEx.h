#pragma once


#include <imgui.h>
#include <examples/imgui_impl_win32.h>
#include <examples/imgui_impl_dx11.h>
#include <misc/cpp/imgui_stdlib.h>
#include <imgui_internal.h>
#include "DX11/External/glm/glm_Prerequisites.h"


namespace ImGuiEx
{
    //----------------------
    // Input
    //----------------------

    bool InputS8(const char* label, ImS8* v, ImS8 step = 1, ImS8 step_fast = 100, ImGuiInputTextFlags flags = 0);

    bool InputU8(const char* label, ImU8* v, ImU8 step = 1, ImU8 step_fast = 100, ImGuiInputTextFlags flags = 0);

    bool InputS16(const char* label, ImS16* v, ImS16 step = 1, ImS16 step_fast = 100, ImGuiInputTextFlags flags = 0);

    bool InputU16(const char* label, ImU16* v, ImU16 step = 1, ImU16 step_fast = 100, ImGuiInputTextFlags flags = 0);

    bool InputS32(const char* label, ImS32* v, ImS32 step = 1, ImS32 step_fast = 100, ImGuiInputTextFlags flags = 0);

    bool InputU32(const char* label, ImU32* v, ImU32 step = 1, ImU32 step_fast = 100, ImGuiInputTextFlags flags = 0);

    bool InputS64(const char* label, ImS64* v, ImS64 step = 1, ImS64 step_fast = 100, ImGuiInputTextFlags flags = 0);

    bool InputU64(const char* label, ImU64* v, ImU64 step = 1, ImU64 step_fast = 100, ImGuiInputTextFlags flags = 0);


    //----------------------
    // Drag
    //----------------------

    bool DragS8(const char* label, ImS8* v, float v_speed = 1.0f, ImS8 v_min = 0, ImS8 v_max = 0, const char* format = "%d");

    bool DragU8(const char* label, ImU8* v, float v_speed = 1.0f, ImU8 v_min = 0, ImU8 v_max = 0, const char* format = "%u");

    bool DragS16(const char* label, ImS16* v, float v_speed = 1.0f, ImS16 v_min = 0, ImS16 v_max = 0, const char* format = "%d");

    bool DragU16(const char* label, ImU16* v, float v_speed = 1.0f, ImU16 v_min = 0, ImU16 v_max = 0, const char* format = "%u");

    bool DragS32(const char* label, ImS32* v, float v_speed = 1.0f, ImS32 v_min = 0, ImS32 v_max = 0, const char* format = "%d");

    bool DragU32(const char* label, ImU32* v, float v_speed = 1.0f, ImU32 v_min = 0, ImU32 v_max = 0, const char* format = "%u");

    bool DragS64(const char* label, ImS64* v, float v_speed = 1.0f, ImS64 v_min = 0, ImS64 v_max = 0, const char* format = "%I64d");

    bool DragU64(const char* label, ImU64* v, float v_speed = 1.0f, ImU64 v_min = 0, ImU64 v_max = 0, const char* format = "%I64u");


    //----------------------
    // GLM
    //----------------------
    bool DragVec2(const char* label, glm::vec2* v, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", float power = 1.0f);
    
    bool DragVec3(const char* label, glm::vec3* v, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", float power = 1.0f);
    
    bool DragVec4(const char* label, glm::vec4* v, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", float power = 1.0f);


    //----------------------
    // ColorEdit
    //----------------------
    bool ColorEdit4(const char* label, glm::vec4* v, ImGuiColorEditFlags flags = 0);

    bool ColorEdit3(const char* label, glm::vec4* v, ImGuiColorEditFlags flags = 0);
}

