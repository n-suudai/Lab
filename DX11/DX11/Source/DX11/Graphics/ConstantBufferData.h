#pragma once

#include "DX11/External/glm/glm_Prerequisites.h"


struct ConstantBufferData
{
    glm::mat4x4 MVPMatrix;
    glm::mat4x4 ModelMatrix;
    glm::vec4   Color;
    glm::vec2   UVOffset;
    float       Time;
    float       dummy;

    ConstantBufferData();

    bool UpdateImGui();
};


struct ConstantBufferData_DiffuseLighting
{
    glm::vec4 LightDirection;   // 平行光      ※逆行列で変換済み
    glm::vec4 EyeDirection;     // 視線ベクトル ※逆行列で変換済み

    glm::vec4 DiffuseColor;     // ディフューズ
    glm::vec4 AmbientColor;     // アンビエント
    glm::vec4 SpecularColor;    // スペキュラ  w : power
    glm::vec4 EmissiveColor;    // エミッシブ

    ConstantBufferData_DiffuseLighting();

    bool UpdateImGui();
};

