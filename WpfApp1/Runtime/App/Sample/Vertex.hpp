
#pragma once


struct Vertex_PositionColor
{
    glm::vec4 Position;
    glm::vec4 Color;

    static constexpr UINT PositionOffset = 0;
    static constexpr UINT PositionSize = sizeof(glm::vec4);

    static constexpr UINT ColorOffset = PositionOffset + PositionSize;
    static constexpr UINT ColorSize = sizeof(glm::vec4);

    static constexpr D3D11_INPUT_ELEMENT_DESC pInputElementDescs[] = {
        { "POSITION", 0,    DXGI_FORMAT_R32G32B32A32_FLOAT, 0,  PositionOffset, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",    0,    DXGI_FORMAT_R32G32B32A32_FLOAT, 0,  ColorOffset,    D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    static constexpr UINT InputElementCount = _countof(pInputElementDescs);
};


struct Vertex_PositionColorTexture
{
    glm::vec4 Position;
    glm::vec4 Color;
    glm::vec2 Texture;

    static constexpr UINT PositionOffset = 0;
    static constexpr UINT PositionSize = sizeof(glm::vec4);

    static constexpr UINT ColorOffset = PositionOffset + PositionSize;
    static constexpr UINT ColorSize = sizeof(glm::vec4);

    static constexpr UINT TextureOffset = ColorOffset + ColorSize;
    static constexpr UINT TextureSize = sizeof(glm::vec2);

    static constexpr D3D11_INPUT_ELEMENT_DESC pInputElementDescs[] = {
        { "POSITION", 0,    DXGI_FORMAT_R32G32B32A32_FLOAT, 0,  PositionOffset, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",    0,    DXGI_FORMAT_R32G32B32A32_FLOAT, 0,  ColorOffset,    D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0,    DXGI_FORMAT_R32G32_FLOAT,       0,  TextureOffset,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    static constexpr UINT InputElementCount = _countof(pInputElementDescs);
};


struct Vertex_PositionColorNormalTexture
{
    glm::vec4 Position;
    glm::vec4 Color;
    glm::vec4 Normal;
    glm::vec2 Texture;

    static constexpr UINT PositionOffset = 0;
    static constexpr UINT PositionSize = sizeof(glm::vec4);

    static constexpr UINT ColorOffset = PositionOffset + PositionSize;
    static constexpr UINT ColorSize = sizeof(glm::vec4);

    static constexpr UINT NormalOffset = ColorOffset + ColorSize;
    static constexpr UINT NormalSize = sizeof(glm::vec4);

    static constexpr UINT TextureOffset = NormalOffset + NormalSize;
    static constexpr UINT TextureSize = sizeof(glm::vec2);

    static constexpr D3D11_INPUT_ELEMENT_DESC pInputElementDescs[] = {
        { "POSITION", 0,    DXGI_FORMAT_R32G32B32A32_FLOAT, 0,  PositionOffset, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",    0,    DXGI_FORMAT_R32G32B32A32_FLOAT, 0,  ColorOffset,    D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL",   0,    DXGI_FORMAT_R32G32B32A32_FLOAT, 0,  NormalOffset,   D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0,    DXGI_FORMAT_R32G32_FLOAT,       0,  TextureOffset,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    static constexpr UINT InputElementCount = _countof(pInputElementDescs);
};


struct Vertex_PositionColorNormal
{
    glm::vec4 Position;
    glm::vec4 Color;
    glm::vec4 Normal;

    static constexpr UINT PositionOffset = 0;
    static constexpr UINT PositionSize = sizeof(glm::vec4);

    static constexpr UINT ColorOffset = PositionOffset + PositionSize;
    static constexpr UINT ColorSize = sizeof(glm::vec4);

    static constexpr UINT NormalOffset = ColorOffset + ColorSize;
    static constexpr UINT NormalSize = sizeof(glm::vec4);

    static constexpr D3D11_INPUT_ELEMENT_DESC pInputElementDescs[] = {
        { "POSITION", 0,    DXGI_FORMAT_R32G32B32A32_FLOAT, 0,  PositionOffset, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",    0,    DXGI_FORMAT_R32G32B32A32_FLOAT, 0,  ColorOffset,    D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL",   0,    DXGI_FORMAT_R32G32B32A32_FLOAT, 0,  NormalOffset,   D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    static constexpr UINT InputElementCount = _countof(pInputElementDescs);
};

