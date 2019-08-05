#pragma once

#include "DX11/External/glm/glm_Prerequisites.h"


namespace MathUtil
{
    inline glm::u32 ColorPack(const glm::vec4& color)
    {
        glm::vec4 saturatedColor = glm::saturation(1.0f, color);
        return (glm::u32)(saturatedColor.a * 255) << 24 |
            (glm::u32)(saturatedColor.b * 255) << 16 |
            (glm::u32)(saturatedColor.g * 255) << 8 |
            (glm::u32)(saturatedColor.r * 255);
    }
}

