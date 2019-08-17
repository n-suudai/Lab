#include "Mesh.h"
#include "DX11/Utils/DX11Util.h"
#include "DX11/Graphics/BufferResource.h"
#include "DX11/Graphics/Texture.h"
#include "DX11/Graphics/Shader.h"
#include "DX11/Graphics/Sampler.h"
#include "DX11/Graphics/RasterizerState.h"
#include "DX11/Graphics/BlendState.h"
#include "DX11/Utils/PathUtil.h"


Model::Model(
    const ComPtr<ID3D11Device>& device,
    const ComPtr<ID3D11DeviceContext>& context,
    const std::shared_ptr<ModelResource>& resource
)
    : m_Device(device)
    , m_Context(context)
    , m_Resource(resource)
{

}


Model::~Model()
{

}


bool Model::Init(const std::string& filename)
{
    std::string warning;
    std::string error;
    tinyobj::attrib_t attribute;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string objFullPath = PathUtil::GetFullPath(filename);
    std::string mtlBaseDir = PathUtil::GetDirectoryName(objFullPath);

    ResultUtil result;
    if (!tinyobj::LoadObj(&attribute, &shapes, &materials, &warning, &error, objFullPath.c_str(), mtlBaseDir.c_str()))
    {
        std::string message;
        if (warning != "")
        {
            message += "Warning : " + warning + "\n";
        }
        if (error != "")
        {
            message += "Error : " + error + "\n";
        }
        result = ResultUtil(false, message);
        result.ShowMessageBox("OBJ Load failed.");
        return false;
    }

    if (m_Resource.expired())
    {
        return false;
    }

    // Material, Texture
    std::shared_ptr<ModelResource> resource = m_Resource.lock();

    ModelResource::MaterialMap& materialList = resource->Materials;
    ModelResource::TextureMap& textureList = resource->Textures;

    for (int i = 0; i < materials.size(); i++)
    {
        tinyobj::material_t* material = &materials[i];

        if (materialList.find(material->name) == materialList.end())
        {
            if (material->diffuse_texname != "")
            {
                if (textureList.find(material->diffuse_texname) == textureList.end())
                {
                    std::shared_ptr<Texture> t = std::make_shared<Texture>(
                        m_Device,
                        m_Context
                        );
                    t->Init(PathUtil::GetFullPath(material->diffuse_texname));

                    textureList.insert(
                        {
                            PathUtil::GetFileNameWithoutExtension(material->diffuse_texname),
                            t
                        }
                    );
                }
            }

            std::shared_ptr<Material> m = std::make_shared<Material>(
                m_Device,
                m_Context,
                resource
                );
            m->Init((*material));

            materialList.insert(
                { material->name, m }
            );
        }
    }

    m_MeshList.resize(shapes.size());
    for (int i = 0; i < shapes.size(); i++)
    {
        m_MeshList[i] = std::make_unique<Mesh>(
            m_Device,
            m_Context,
            resource
            );
        m_MeshList[i]->Init(
            attribute,
            shapes[i],
            materials
        );
    }

    return true;
}


void Model::Update(const ConstantBufferData& data)
{
    for (int i = 0; i < m_MeshList.size(); i++)
    {
        m_MeshList[i]->Update(data);
    }
}


void Model::Draw()
{
    for (int i = 0; i < m_MeshList.size(); i++)
    {
        m_MeshList[i]->Draw();
    }
}


Mesh::Mesh(
    const ComPtr<ID3D11Device>& device,
    const ComPtr<ID3D11DeviceContext>& context,
    const std::shared_ptr<ModelResource>& resource
)
    : m_Device(device)
    , m_Context(context)
    , m_VertexCount(0)
    , m_IndexCount(0)
    , m_Resource(resource)
{

}


Mesh::~Mesh()
{

}


bool Mesh::Init(
    const tinyobj::attrib_t& attribute,
    const tinyobj::shape_t& shape,
    const std::vector<tinyobj::material_t>& materials
)
{
    const auto v = [&](int index) -> Vertex_PositionColorNormalTexture {
        const auto i = shape.mesh.indices[index];
        
        Vertex_PositionColorNormalTexture vertex = {};

        if (attribute.vertices.size() > (3 * i.vertex_index + 2))
        {
            vertex.Position = glm::vec4(
                attribute.vertices[3 * i.vertex_index + 0],
                attribute.vertices[3 * i.vertex_index + 1],
                attribute.vertices[3 * i.vertex_index + 2],
                1.0f
            );
        }

        if (attribute.colors.size() > (3 * i.vertex_index + 2))
        {
            vertex.Color = glm::vec4(
                attribute.colors[3 * i.vertex_index + 0],
                attribute.colors[3 * i.vertex_index + 1],
                attribute.colors[3 * i.vertex_index + 2],
                1.0f
            );
        }

        if (attribute.normals.size() > (3 * i.normal_index + 2))
        {
            vertex.Normal = glm::normalize(
                glm::vec4(
                    attribute.normals[3 * i.normal_index + 0],
                    attribute.normals[3 * i.normal_index + 1],
                    attribute.normals[3 * i.normal_index + 2],
                    1.0f
                )
            );
        }

        if (attribute.texcoords.size() > (3 * i.texcoord_index + 1))
        {
            vertex.Texture = glm::vec2(
                attribute.texcoords[3 * i.texcoord_index + 0],
                attribute.texcoords[3 * i.texcoord_index + 1]
            );
        }

        return vertex;
    };

    if (m_Resource.expired())
    {
        return false;
    }

    std::shared_ptr<ModelResource> resource = m_Resource.lock();

    std::vector<Vertex_PositionColorNormalTexture> vertices;
    std::vector<u16> indices;
    vertices.resize(shape.mesh.num_face_vertices.size() * 3);
    indices.resize(shape.mesh.num_face_vertices.size() * 3);

    if (shape.mesh.material_ids.size() > 0)
    {
        if (materials.size() > shape.mesh.material_ids[0] && shape.mesh.material_ids[0] >= 0)
        {
            const tinyobj::material_t* material = &materials[shape.mesh.material_ids[0]];

            ModelResource::MaterialMap::iterator it = resource->Materials.find(material->name);

            if (it != resource->Materials.end())
            {
                m_Material = (*it).second;
            }
        }
    }

    for (int face_index = 0; face_index < (int)shape.mesh.num_face_vertices.size(); face_index++)
    {
        vertices[3 * face_index + 0] = v(3 * face_index + 0);
        vertices[3 * face_index + 1] = v(3 * face_index + 1);
        vertices[3 * face_index + 2] = v(3 * face_index + 2);

        indices[3 * face_index + 0] = static_cast<u16>(3 * face_index + 0);
        indices[3 * face_index + 1] = static_cast<u16>(3 * face_index + 1);
        indices[3 * face_index + 2] = static_cast<u16>(3 * face_index + 2);
    }

    m_VertexCount = static_cast<UINT>(vertices.size());

    m_IndexCount = static_cast<UINT>(indices.size());

    m_VertexBuffer = std::make_unique<VertexBuffer>(
        m_Device,
        m_Context,
        vertices.data(),
        sizeof(Vertex_PositionColorNormalTexture) * vertices.size()
        );

    m_IndexBuffer = std::make_unique<IndexBuffer>(
        m_Device,
        m_Context,
        indices
        );

    return true;
}


void Mesh::Update(const ConstantBufferData& data)
{
    if (m_Material)
    {
        m_Material->Update(data);
    }
}


void Mesh::Draw()
{
    if (m_Material)
    {
        m_Material->Set();
    }

    m_VertexBuffer->Set(0, sizeof(Vertex_PositionColorNormalTexture), 0);

    m_IndexBuffer->Set();
    
    //m_Context->Draw(m_VertexCount, 0);
    m_Context->DrawIndexed(m_IndexCount, 0, 0);
}


Material::Material(
    const ComPtr<ID3D11Device>& device,
    const ComPtr<ID3D11DeviceContext>& context,
    const std::shared_ptr<ModelResource>& resource
)
    : m_Device(device)
    , m_Context(context)
    , m_Resource(resource)
{

}


Material::~Material()
{

}


bool Material::Init(const tinyobj::material_t& material)
{
    if (m_Resource.expired())
    {
        return false;
    }

    {
        std::shared_ptr<ModelResource> resource = m_Resource.lock();
        ModelResource::TextureMap::iterator it = resource->Textures.find(
            PathUtil::GetFileNameWithoutExtension(material.diffuse_texname)
        );

        if (it != resource->Textures.end())
        {
            m_Texture = (*it).second;
        }
    }

    std::string shaderCode = R"(
//Texture2D diffuseTexture : register(t0);
//SamplerState normalSampler : register(s0);

cbuffer CB0 : register(b0)
{
    float4x4 MVPMatrix;
    float4 Color;
    float2 UVOffset;
    float2 dummy;
};

struct VS_INPUT {
    float4 position : POSITION;
    float4 color    : COLOR;
    float4 normal   : NORMAL;
    float2 texcoord : TEXCOORD;
};

struct VS_OUTPUT {
    float4 position : SV_POSITION;
    float4 color    : COLOR;
    float4 normal   : NORMAL;
    float2 texcoord : TEXCOORD;
};

VS_OUTPUT vs_main(VS_INPUT In) {
    VS_OUTPUT Out = (VS_OUTPUT)0;
    Out.position = mul(In.position, MVPMatrix);
    Out.color = Color;
    Out.texcoord = In.texcoord + UVOffset;
    Out.normal = In.normal;
    return Out;
}

float4 ps_main(VS_OUTPUT In) : SV_TARGET {
    //return diffuseTexture.Sample(normalSampler, In.texcoord) * In.color;
    float3 n = normalize(In.normal.xyz);
    n = saturate(n);
    return float4(n.x, n.y, n.z, 1.0f);
}
)";

    m_Shader = std::make_unique<Shader>(
        m_Device,
        m_Context
        );
    bool result = m_Shader->Init(
        shaderCode,
        SHADER_USE_FLAG_VS | SHADER_USE_FLAG_PS,
        Vertex_PositionColorNormalTexture::InputElements,
        Vertex_PositionColorNormalTexture::ElementsCount
    );
    if (!result)
    {
        return false;
    }

    m_Sampler = std::make_unique<Sampler>(
        m_Device,
        m_Context
        );

    m_BlendState = std::make_unique<BlendState>(
        m_Device,
        m_Context
        );

    m_RasterizerState = std::make_unique<RasterizerState>(
        m_Device,
        m_Context
        );

    m_ConstantBuffer = std::make_unique<ConstantBuffer>(
        m_Device,
        m_Context,
        &m_ConstantBufferData,
        sizeof(ConstantBufferData)
        );

    return true;
}


void Material::Update(const ConstantBufferData& data)
{
    m_ConstantBufferData = data;

    if (m_ConstantBuffer)
    {
        m_ConstantBuffer->Update(&m_ConstantBufferData);
    }
}


void Material::Set()
{
    m_Context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    if (m_Texture)
    {
        m_Texture->Bind();
    }

    if (m_Sampler)
    {
        m_Sampler->Set();
    }

    if (m_BlendState)
    {
        m_BlendState->Set();
    }

    if (m_RasterizerState)
    {
        m_RasterizerState->Set();
    }

    if (m_Shader)
    {
        m_Shader->ResetAll(); // 一応
        m_Shader->SetAll();
    }

    if (m_ConstantBuffer)
    {
        m_ConstantBuffer->SetVS();
        m_ConstantBuffer->SetPS();
    }
}

