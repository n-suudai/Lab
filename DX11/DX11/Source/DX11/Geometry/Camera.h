#pragma once

#include "DX11/External/glm/glm_Prerequisites.h"


struct Camera
{
    glm::mat4x4 viewMatrix;
    glm::mat4x4 perspectiveMatrix;
    glm::mat4x4 orthoGraphicMatrix;

    glm::vec3 eye;
    glm::vec3 center;
    glm::vec3 up;
    
    glm::vec2 screenSize;

    float nearClip;
    float farClip;
    float fov;

    Camera();

    void UpdateMatrix();

    bool UpdateImGui();
};

