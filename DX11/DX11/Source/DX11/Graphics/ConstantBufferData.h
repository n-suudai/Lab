#pragma once

#include "DX11/External/glm/glm_Prerequisites.h"


struct ConstantBufferData
{
    glm::mat4x4 MVPMatrix;
    glm::vec4   Color;
    glm::vec2   UVOffset;
    float       dummy[2];

    ConstantBufferData();

    bool UpdateImGui();
};

