#include "ImGuiEx.h"



bool ImGuiEx::InputS8(const char* label, ImS8* v, ImS8 step, ImS8 step_fast, ImGuiInputTextFlags flags)
{
    // Hexadecimal input provided as a convenience but the flag name is awkward. Typically you'd use InputText() to parse your own data, if you want to handle prefixes.
    const char* format = (flags & ImGuiInputTextFlags_CharsHexadecimal) ? "%08X" : "%d";
    return ImGui::InputScalar(label, ImGuiDataType_S8, (void*)v, (void*)(step > 0 ? &step : NULL), (void*)(step_fast > 0 ? &step_fast : NULL), format, flags);
}

bool ImGuiEx::InputU8(const char* label, ImU8* v, ImU8 step, ImU8 step_fast, ImGuiInputTextFlags flags)
{
    // Hexadecimal input provided as a convenience but the flag name is awkward. Typically you'd use InputText() to parse your own data, if you want to handle prefixes.
    const char* format = (flags & ImGuiInputTextFlags_CharsHexadecimal) ? "%08X" : "%u";
    return ImGui::InputScalar(label, ImGuiDataType_U8, (void*)v, (void*)(step > 0 ? &step : NULL), (void*)(step_fast > 0 ? &step_fast : NULL), format, flags);
}

bool ImGuiEx::InputS16(const char* label, ImS16* v, ImS16 step, ImS16 step_fast, ImGuiInputTextFlags flags)
{
    // Hexadecimal input provided as a convenience but the flag name is awkward. Typically you'd use InputText() to parse your own data, if you want to handle prefixes.
    const char* format = (flags & ImGuiInputTextFlags_CharsHexadecimal) ? "%08X" : "%d";
    return ImGui::InputScalar(label, ImGuiDataType_S16, (void*)v, (void*)(step > 0 ? &step : NULL), (void*)(step_fast > 0 ? &step_fast : NULL), format, flags);
}

bool ImGuiEx::InputU16(const char* label, ImU16* v, ImU16 step, ImU16 step_fast, ImGuiInputTextFlags flags)
{
    // Hexadecimal input provided as a convenience but the flag name is awkward. Typically you'd use InputText() to parse your own data, if you want to handle prefixes.
    const char* format = (flags & ImGuiInputTextFlags_CharsHexadecimal) ? "%08X" : "%u";
    return ImGui::InputScalar(label, ImGuiDataType_U16, (void*)v, (void*)(step > 0 ? &step : NULL), (void*)(step_fast > 0 ? &step_fast : NULL), format, flags);
}

bool ImGuiEx::InputS32(const char* label, ImS32* v, ImS32 step, ImS32 step_fast, ImGuiInputTextFlags flags)
{
    // Hexadecimal input provided as a convenience but the flag name is awkward. Typically you'd use InputText() to parse your own data, if you want to handle prefixes.
    const char* format = (flags & ImGuiInputTextFlags_CharsHexadecimal) ? "%08X" : "%d";
    return ImGui::InputScalar(label, ImGuiDataType_S32, (void*)v, (void*)(step > 0 ? &step : NULL), (void*)(step_fast > 0 ? &step_fast : NULL), format, flags);
}

bool ImGuiEx::InputU32(const char* label, ImU32* v, ImU32 step, ImU32 step_fast, ImGuiInputTextFlags flags)
{
    // Hexadecimal input provided as a convenience but the flag name is awkward. Typically you'd use InputText() to parse your own data, if you want to handle prefixes.
    const char* format = (flags & ImGuiInputTextFlags_CharsHexadecimal) ? "%08X" : "%u";
    return ImGui::InputScalar(label, ImGuiDataType_U32, (void*)v, (void*)(step > 0 ? &step : NULL), (void*)(step_fast > 0 ? &step_fast : NULL), format, flags);
}

bool ImGuiEx::InputS64(const char* label, ImS64* v, ImS64 step, ImS64 step_fast, ImGuiInputTextFlags flags)
{
    // Hexadecimal input provided as a convenience but the flag name is awkward. Typically you'd use InputText() to parse your own data, if you want to handle prefixes.
    const char* format = (flags & ImGuiInputTextFlags_CharsHexadecimal) ? "%016I64X" : "%I64d";
    return ImGui::InputScalar(label, ImGuiDataType_S64, (void*)v, (void*)(step > 0 ? &step : NULL), (void*)(step_fast > 0 ? &step_fast : NULL), format, flags);
}

bool ImGuiEx::InputU64(const char* label, ImU64* v, ImU64 step, ImU64 step_fast, ImGuiInputTextFlags flags)
{
    // Hexadecimal input provided as a convenience but the flag name is awkward. Typically you'd use InputText() to parse your own data, if you want to handle prefixes.
    const char* format = (flags & ImGuiInputTextFlags_CharsHexadecimal) ? "%016I64X" : "%I64u";
    return ImGui::InputScalar(label, ImGuiDataType_U64, (void*)v, (void*)(step > 0 ? &step : NULL), (void*)(step_fast > 0 ? &step_fast : NULL), format, flags);
}


bool ImGuiEx::DragS8(const char* label, ImS8* v, float v_speed, ImS8 v_min, ImS8 v_max, const char* format)
{
    return ImGui::DragScalar(label, ImGuiDataType_S8, v, v_speed, &v_min, &v_max, format);
}

bool ImGuiEx::DragU8(const char* label, ImU8* v, float v_speed, ImU8 v_min, ImU8 v_max, const char* format)
{
    return ImGui::DragScalar(label, ImGuiDataType_U8, v, v_speed, &v_min, &v_max, format);
}

bool ImGuiEx::DragS16(const char* label, ImS16* v, float v_speed, ImS16 v_min, ImS16 v_max, const char* format)
{
    return ImGui::DragScalar(label, ImGuiDataType_S16, v, v_speed, &v_min, &v_max, format);
}

bool ImGuiEx::DragU16(const char* label, ImU16* v, float v_speed, ImU16 v_min, ImU16 v_max, const char* format)
{
    return ImGui::DragScalar(label, ImGuiDataType_U16, v, v_speed, &v_min, &v_max, format);
}

bool ImGuiEx::DragS32(const char* label, ImS32* v, float v_speed, ImS32 v_min, ImS32 v_max, const char* format)
{
    return ImGui::DragScalar(label, ImGuiDataType_S32, v, v_speed, &v_min, &v_max, format);
}

bool ImGuiEx::DragU32(const char* label, ImU32* v, float v_speed, ImU32 v_min, ImU32 v_max, const char* format)
{
    return ImGui::DragScalar(label, ImGuiDataType_U32, v, v_speed, &v_min, &v_max, format);
}

bool ImGuiEx::DragS64(const char* label, ImS64* v, float v_speed, ImS64 v_min, ImS64 v_max, const char* format)
{
    return ImGui::DragScalar(label, ImGuiDataType_S64, v, v_speed, &v_min, &v_max, format);
}

bool ImGuiEx::DragU64(const char* label, ImU64* v, float v_speed, ImU64 v_min, ImU64 v_max, const char* format)
{
    return ImGui::DragScalar(label, ImGuiDataType_U64, v, v_speed, &v_min, &v_max, format);
}


bool ImGuiEx::DragVec2(const char* label, glm::vec2* v, float v_speed, float v_min, float v_max, const char* format, float power)
{
    return ImGui::DragScalarN(label, ImGuiDataType_Float, reinterpret_cast<float*>(v), 2, v_speed, &v_min, &v_max, format, power);
}

bool ImGuiEx::DragVec3(const char* label, glm::vec3* v, float v_speed, float v_min, float v_max, const char* format, float power)
{
    return ImGui::DragScalarN(label, ImGuiDataType_Float, reinterpret_cast<float*>(v), 3, v_speed, &v_min, &v_max, format, power);
}

bool ImGuiEx::DragVec4(const char* label, glm::vec4* v, float v_speed, float v_min, float v_max, const char* format, float power)
{
    return ImGui::DragScalarN(label, ImGuiDataType_Float, reinterpret_cast<float*>(v), 4, v_speed, &v_min, &v_max, format, power);
}

