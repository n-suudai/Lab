#pragma once

#include <string>
#include "DX11/Graphics/Vertex.h"
#include "DX11/Geometry/Transform.h"
#include "DX11/Graphics/ConstantBufferData.h"
#include <tiny_obj_loader.h>
#include <unordered_map>
#include <vector>
#include <memory>


class Material;
class Texture;
struct ModelResource : public std::enable_shared_from_this<ModelResource>
{
    typedef std::unordered_map<std::string, std::shared_ptr<Material>>  MaterialMap;
    typedef std::unordered_map<std::string, std::shared_ptr<Texture>>   TextureMap;

    MaterialMap Materials;
    TextureMap  Textures;

    bool InitDefault(
        const ComPtr<ID3D11Device>& device,
        const ComPtr<ID3D11DeviceContext>& context
    );

    bool UpdateImGui();
};


class Mesh;
class Model
{
public:
    Model(
        const ComPtr<ID3D11Device>& device,
        const ComPtr<ID3D11DeviceContext>& context,
        const std::shared_ptr<ModelResource>& resource
    );
    virtual ~Model();

    bool Init(const std::string& filename);

    bool InitAsTorus(u16 row, u16 column, f32 irad, f32 orad, const glm::vec4* color = nullptr);

    bool InitAsSphere(u16 x, u16 y, f32 rad, const glm::vec4* color = nullptr);

    bool InitAsCube(f32 scale, const glm::vec4* color = nullptr);

    bool InitAsCylinder(u16 x, u16 y, f32 rad, f32 height, const glm::vec4* color = nullptr);

    void Update();

    void Draw();

protected:
    ComPtr<ID3D11Device>        m_Device;
    ComPtr<ID3D11DeviceContext> m_Context;

    std::vector<std::unique_ptr<Mesh>> m_MeshList;
    std::weak_ptr<ModelResource>  m_Resource;
};


class VertexBuffer;
class IndexBuffer;
class Mesh
{
public:
    Mesh(
        const ComPtr<ID3D11Device>& device,
        const ComPtr<ID3D11DeviceContext>& context,
        const std::shared_ptr<ModelResource>& resource
    );
    virtual ~Mesh();

    bool Init(
        const tinyobj::attrib_t& attribute,
        const tinyobj::shape_t& shape,
        const std::vector<tinyobj::material_t>& materials
    );

    bool InitAsTorus(u16 row, u16 column, f32 irad, f32 orad, const glm::vec4* color);

    bool InitAsSphere(u16 x, u16 y, f32 rad, const glm::vec4* color);

    bool InitAsCube(f32 scale, const glm::vec4* color);

    bool InitAsCylinder(u16 x, u16 y, f32 rad, f32 height, const glm::vec4* color);

    void Update();

    void Draw();

protected:
    ComPtr<ID3D11Device>        m_Device;
    ComPtr<ID3D11DeviceContext> m_Context;

    UINT                            m_VertexCount;
    UINT                            m_IndexCount;
    std::unique_ptr<VertexBuffer>   m_VertexBuffer;
    std::unique_ptr<IndexBuffer>    m_IndexBuffer;
    std::shared_ptr<Material>       m_Material;

    std::weak_ptr<ModelResource>  m_Resource;
};


class Sampler;
class BlendState;
class ConstantBuffer;
class Shader;
class RasterizerState;
class Material : public std::enable_shared_from_this<Material>
{
public:
    Material(
        const ComPtr<ID3D11Device>& device,
        const ComPtr<ID3D11DeviceContext>& context,
        const std::shared_ptr<ModelResource>& resource
    );
    virtual ~Material();

    bool Init(const tinyobj::material_t& material);
    bool InitDefault();

    bool UpdateImGui();
    void Update();

    void SetMVPMatrix(const glm::mat4x4& mvpMatrix);
    void SetEyeDirection(const glm::vec4& eyeDirection);
    void SetLightDirection(const glm::vec4& lightDirection);

    void SetDiffuseColor(const glm::vec3& diffuseColor);
    void SetAmbientColor(const glm::vec3& ambientColor);
    void SetSpecularColor(const glm::vec3& specularColor);
    void SetEmissiveColor(const glm::vec3& emissiveColor);

    void SetSpecularPower(float power);

    void SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY primitiveTopology);

    void Set();

protected:
    ComPtr<ID3D11Device>        m_Device;
    ComPtr<ID3D11DeviceContext> m_Context;

    std::shared_ptr<Texture>            m_Texture;
    std::unique_ptr<Shader>             m_Shader;
    std::unique_ptr<Sampler>            m_Sampler;
    std::unique_ptr<BlendState>         m_BlendState;
    std::unique_ptr<RasterizerState>    m_RasterizerState;

    ConstantBufferData                  m_CB0;
    ConstantBufferData_DiffuseLighting  m_CB1;

    D3D_PRIMITIVE_TOPOLOGY              m_PrimitiveTopology;

    bool m_ConstantBufferChanged;

    std::vector<std::unique_ptr<ConstantBuffer>> m_ConstantBuffers;
    std::weak_ptr<ModelResource> m_Resource;
};

