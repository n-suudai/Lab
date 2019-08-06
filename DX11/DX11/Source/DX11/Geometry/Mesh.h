#pragma once

#include <string>
#include "DX11/External/glm/glm_Prerequisites.h"
#include "DX11/DX11Prerequisites.h"



enum class VertexFormat
{
    Position,
    Color,
    Normal,
    Tangent,
    TexCoord1,
    TexCoord2,
    TexCoord3,
};

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
    ComPtr<ID3D11Device>        m_Device;
    ComPtr<ID3D11DeviceContext> m_Context;

    std::string m_FileName;


};


class ConstantBuffer;
class Shader;
class Texture;
class Material
{
public:
    Material(
        const ComPtr<ID3D11Device>& device,
        const ComPtr<ID3D11DeviceContext>& context
    );
    virtual ~Material();

protected:
    ComPtr<ID3D11Device>        m_Device;
    ComPtr<ID3D11DeviceContext> m_Context;

    std::unique_ptr<Shader>         m_Shader;
    std::unique_ptr<ConstantBuffer> m_ConstantBuffer;
    std::unique_ptr<Texture>        m_Texture;
};





