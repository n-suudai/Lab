#pragma once

#include <string>
#include "DX11/External/glm/glm_Prerequisites.h"
#include "DX11/DX11Prerequisites.h"


class Mesh
{
public:
    struct Vertex
    {
        glm::vec4 Position;
        glm::vec4 Color;
        glm::vec4 Normal;
        glm::vec2 TexCoord;
    };

    Mesh(
        const ComPtr<ID3D11Device>& device,
        const ComPtr<ID3D11DeviceContext>& context
    );
    virtual ~Mesh();

protected:
    std::string m_FileName;


};

