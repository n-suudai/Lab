#pragma once
#include "DX11/External/glm/glm_Prerequisites.h"
#include "DX11/DX11Prerequisites.h"


struct Vertex_Position
{
    glm::vec4 Position;

    static constexpr D3D11_INPUT_ELEMENT_DESC InputElements[] = {
        { "POSITION",   0, DXGI_FORMAT_R32G32B32A32_FLOAT,  0, 0,                            D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    static constexpr UINT ElementsCount = _countof(InputElements);
};

struct Vertex_PositionColor
{
    glm::vec4 Position;
    glm::vec4 Color;

    static constexpr D3D11_INPUT_ELEMENT_DESC InputElements[] = {
        { "POSITION",   0, DXGI_FORMAT_R32G32B32A32_FLOAT,  0, 0,                            D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",      0, DXGI_FORMAT_R32G32B32A32_FLOAT,  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    static constexpr UINT ElementsCount = _countof(InputElements);
};

struct Vertex_PositionNormal
{
    glm::vec4 Position;
    glm::vec4 Normal;

    static constexpr D3D11_INPUT_ELEMENT_DESC InputElements[] = {
        { "POSITION",   0, DXGI_FORMAT_R32G32B32A32_FLOAT,  0, 0,                            D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL",     0, DXGI_FORMAT_R32G32B32A32_FLOAT,  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    static constexpr UINT ElementsCount = _countof(InputElements);
};

struct Vertex_PositionTexture
{
    glm::vec4 Position;
    glm::vec2 Texture;

    static constexpr D3D11_INPUT_ELEMENT_DESC InputElements[] = {
        { "POSITION",   0, DXGI_FORMAT_R32G32B32A32_FLOAT,  0, 0,                            D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD",   0, DXGI_FORMAT_R32G32_FLOAT,        0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    static constexpr UINT ElementsCount = _countof(InputElements);
};

struct Vertex_PositionColorNormal
{
    glm::vec4 Position;
    glm::vec4 Color;
    glm::vec4 Normal;

    static constexpr D3D11_INPUT_ELEMENT_DESC InputElements[] = {
        { "POSITION",   0, DXGI_FORMAT_R32G32B32A32_FLOAT,  0, 0,                            D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",      0, DXGI_FORMAT_R32G32B32A32_FLOAT,  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL",     0, DXGI_FORMAT_R32G32B32A32_FLOAT,  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    static constexpr UINT ElementsCount = _countof(InputElements);
};

struct Vertex_PositionColorTexture
{
    glm::vec4 Position;
    glm::vec4 Color;
    glm::vec2 Texture;

    static constexpr D3D11_INPUT_ELEMENT_DESC InputElements[] = {
        { "POSITION",   0, DXGI_FORMAT_R32G32B32A32_FLOAT,  0, 0,                            D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",      0, DXGI_FORMAT_R32G32B32A32_FLOAT,  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD",   0, DXGI_FORMAT_R32G32_FLOAT,        0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    static constexpr UINT ElementsCount = _countof(InputElements);
};

struct Vertex_PositionNormalTexture
{
    glm::vec4 Position;
    glm::vec4 Normal;
    glm::vec2 Texture;

    static constexpr D3D11_INPUT_ELEMENT_DESC InputElements[] = {
        { "POSITION",   0, DXGI_FORMAT_R32G32B32A32_FLOAT,  0, 0,                            D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL",     0, DXGI_FORMAT_R32G32B32A32_FLOAT,  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD",   0, DXGI_FORMAT_R32G32_FLOAT,        0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    static constexpr UINT ElementsCount = _countof(InputElements);
};

struct Vertex_PositionColorNormalTexture
{
    glm::vec4 Position;
    glm::vec4 Color;
    glm::vec4 Normal;
    glm::vec2 Texture;

    static constexpr D3D11_INPUT_ELEMENT_DESC InputElements[] = {
        { "POSITION",   0, DXGI_FORMAT_R32G32B32A32_FLOAT,  0, 0,                            D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",      0, DXGI_FORMAT_R32G32B32A32_FLOAT,  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL",     0, DXGI_FORMAT_R32G32B32A32_FLOAT,  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD",   0, DXGI_FORMAT_R32G32_FLOAT,        0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    static constexpr UINT ElementsCount = _countof(InputElements);
};

struct Vertex_PositionColorNormalTangentTexture
{
    glm::vec4 Position;
    glm::vec4 Color;
    glm::vec4 Normal;
    glm::vec4 Tangent;
    glm::vec2 Texture;

    static constexpr D3D11_INPUT_ELEMENT_DESC InputElements[] = {
        { "POSITION",   0, DXGI_FORMAT_R32G32B32A32_FLOAT,  0, 0,                            D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",      0, DXGI_FORMAT_R32G32B32A32_FLOAT,  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL",     0, DXGI_FORMAT_R32G32B32A32_FLOAT,  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TANGENT",    0, DXGI_FORMAT_R32G32B32A32_FLOAT,  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD",   0, DXGI_FORMAT_R32G32_FLOAT,        0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    static constexpr UINT ElementsCount = _countof(InputElements);
};

