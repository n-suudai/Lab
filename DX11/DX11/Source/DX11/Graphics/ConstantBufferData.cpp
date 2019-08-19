#include "ConstantBufferData.h"
#include "DX11/External/ImGui/ImGui_DX11.h"


ConstantBufferData::ConstantBufferData()
    : MVPMatrix(1.0f)
    , Color(1.0f)
    , UVOffset(0.0f)
{

}


bool ConstantBufferData::UpdateImGui()
{
    bool changed = false;

    changed |= ImGuiEx::DragVec4("Color", &Color, 0.01f);
    changed |= ImGuiEx::DragVec2("UVOffset", &UVOffset, 0.01f);

    return changed;
}



ConstantBufferData_DiffuseLighting::ConstantBufferData_DiffuseLighting()
    : LightDirection(0.0f)
    , EyeDirection(0.0f)
    , DiffuseColor(1.0f)
    , AmbientColor(0.0f)
    , SpecularColor(1.0f, 1.0f, 1.0f, 5.0f)
    , EmissiveColor(0.0f)
{

}


bool ConstantBufferData_DiffuseLighting::UpdateImGui()
{
    bool changed = false;

    //changed |= ImGuiEx::DragVec4("LightDirection", &LightDirection, 0.01f);
    //changed |= ImGuiEx::DragVec4("EyeDirection", &EyeDirection, 0.01f);
    changed |= ImGuiEx::DragVec4("DiffuseColor", &DiffuseColor, 0.01f);
    changed |= ImGuiEx::DragVec4("AmbientColor", &AmbientColor, 0.01f);
    changed |= ImGuiEx::DragVec4("SpecularColor", &SpecularColor, 0.01f);
    changed |= ImGuiEx::DragVec4("EmissiveColor", &EmissiveColor, 0.01f);

    return changed;
}

