#include "Camera.h"
#include "../External/ImGui/ImGui_DX11.h"


Camera::Camera()
    : viewMatrix(1.0f)
    , perspectiveMatrix(1.0f)
    , orthoGraphicMatrix(1.0f)
    , eye(0.0f)
    , center(0.0f)
    , up(0.0f, 1.0f, 0.0f)
    , screenSize(1280.0f, 720.0f)
    , nearClip(0.1f)
    , farClip(10.0f)
    , fov(glm::radians(90.0f))
{

}


void Camera::UpdateMatrix()
{
    viewMatrix = glm::lookAtLH(eye, center, up);
    perspectiveMatrix = glm::perspectiveFovLH(
        fov, screenSize.x, screenSize.y, nearClip, farClip
    );
    orthoGraphicMatrix = glm::orthoLH(
        0.0f,
        screenSize.x,
        screenSize.y,
        0.0f,
        nearClip,
        farClip
    );
}


bool Camera::UpdateImGui()
{
    bool changed = false;

    if (ImGui::TreeNode("Camera"))
    {
        changed |= ImGuiEx::DragVec3("eye", &eye);
        changed |= ImGuiEx::DragVec3("center", &center);

        changed |= ImGui::DragFloat("near", &nearClip, 0.1f, 0.1f, farClip);
        changed |= ImGui::DragFloat("far", &farClip, 1.0f, farClip, 10000.0f);

        changed |= ImGui::DragFloat("fov", &fov, 0.1f, 0.0f, glm::radians(180.0f));

        ImGui::TreePop();
    }

    return changed;
}

