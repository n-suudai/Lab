#include "Transform.hpp"
#include "../External/ImGui/ImGui_DX11.h"


Transform::Transform()
    : matrix(1.0f)
    , position(0.0f)
    , rotate(0.0f)
    , scale(1.0f)
{

}


void Transform::UpdateMatrix()
{
    glm::translate(matrix, position);
    glm::rotate(matrix, rotate.x, glm::vec3(1.0f, 0.0f, 0.0f));
    glm::rotate(matrix, rotate.y, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::rotate(matrix, rotate.z, glm::vec3(0.0f, 0.0f, 1.0f));
    glm::scale(matrix, scale);
}


bool Transform::UpdateImGui(const char* label)
{
    bool changed = false;

    ImGui::PushID(this);

    if (ImGui::TreeNode(label))
    {
        changed |= ImGuiEx::DragVec3("position", &position);
        changed |= ImGuiEx::DragVec3("rotate", &rotate);
        changed |= ImGuiEx::DragVec3("scale", &scale);

        ImGui::TreePop();
    }

    ImGui::PopID();

    return changed;
}
