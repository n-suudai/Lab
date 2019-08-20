#pragma once

#pragma warning(push)
#pragma warning(disable:4201) 
#pragma warning(disable:4127) 
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/color_space.hpp>
#pragma warning(pop)


namespace glmEx
{
    inline glm::vec2 to_vec2(const float v[2])
    {
        return glm::vec2(v[0], v[1]);
    }

    inline glm::vec3 to_vec3(const float v[3])
    {
        return glm::vec3(v[0], v[1], v[2]);
    }

    inline glm::vec4 to_vec4(const float v[4])
    {
        return glm::vec4(v[0], v[1], v[2], v[3]);
    }
}
