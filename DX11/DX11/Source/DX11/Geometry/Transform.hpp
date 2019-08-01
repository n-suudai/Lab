#pragma once


#include "DX11/External/glm/glm_Prerequisites.h"


struct Transform
{
    glm::mat4x4 matrix;

    glm::vec3 position;
    glm::vec3 rotate;
    glm::vec3 scale;

    Transform();

    void UpdateMatrix();

    bool UpdateImGui(const char* label);
};

