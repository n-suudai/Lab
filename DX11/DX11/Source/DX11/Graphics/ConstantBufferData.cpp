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

