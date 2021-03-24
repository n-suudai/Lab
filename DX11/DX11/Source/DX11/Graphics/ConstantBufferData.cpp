#include "ConstantBufferData.h"
#include "DX11/External/ImGui/ImGui_DX11.h"


ConstantBufferData::ConstantBufferData()
    : MVPMatrix(1.0f)
    , ModelMatrix(1.0f)
    , Color(1.0f)
    , UVOffset(0.0f)
    , Time(0.0f)
{

}


bool ConstantBufferData::UpdateImGui()
{
    bool changed = false;

    changed |= ImGuiEx::ColorEdit4("Color", &Color);
    changed |= ImGuiEx::DragVec2("UVOffset", &UVOffset, 0.01f);
    ImGui::Text("Time : %f", Time);

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
    changed |= ImGuiEx::ColorEdit4("DiffuseColor", &DiffuseColor);
    changed |= ImGuiEx::ColorEdit4("AmbientColor", &AmbientColor);
    changed |= ImGuiEx::ColorEdit3("SpecularColor", &SpecularColor);
    changed |= ImGuiEx::ColorEdit4("EmissiveColor", &EmissiveColor);
    changed |= ImGui::DragFloat("SpecularPower", &SpecularColor.w, 1.0f, 0.0f, 100.0f);

    return changed;
}

