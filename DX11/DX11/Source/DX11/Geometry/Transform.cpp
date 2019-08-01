#include "Transform.hpp"
#include "../External/ImGui/ImGui_DX11.h"


Transform::Transform()
    : matrix(1.0f)
    , position(0.0f)
    , rotate(0.0f)
    , scale(1.0f)
{
    UpdateMatrix();
}


void Transform::UpdateMatrix()
{
    matrix = glm::mat4x4(1.0f);
    matrix = glm::translate(matrix, position);
    matrix = glm::rotate(matrix, rotate.x, glm::vec3(1.0f, 0.0f, 0.0f));
    matrix = glm::rotate(matrix, rotate.y, glm::vec3(0.0f, 1.0f, 0.0f));
    matrix = glm::rotate(matrix, rotate.z, glm::vec3(0.0f, 0.0f, 1.0f));
    matrix = glm::scale(matrix, scale);
}


bool Transform::UpdateImGui(const char* label)
{
    bool changed = false;

    ImGui::PushID(this);

    if (ImGui::TreeNode(label))
    {
        changed |= ImGuiEx::DragVec3("position", &position, 0.1f);
        changed |= ImGuiEx::DragVec3("rotate", &rotate, 0.1f);
        changed |= ImGuiEx::DragVec3("scale", &scale, 0.1f);

        ImGui::TreePop();
    }

    ImGui::PopID();

    return changed;
}
