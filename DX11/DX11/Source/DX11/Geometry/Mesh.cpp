#include "Mesh.h"
#include "DX11/Utils/DX11Util.h"
#include "DX11/Graphics/BufferResource.h"
#include "DX11/Graphics/Texture.h"
#include "DX11/Graphics/Shader.h"
#include "DX11/Graphics/Sampler.h"
#include "DX11/Graphics/RasterizerState.h"
#include "DX11/Graphics/BlendState.h"
#include "DX11/Utils/PathUtil.h"
#include "DX11/External/ImGui/ImGui_DX11.h"



bool ModelResource::UpdateImGui()
{
    bool changed = false;

    ImGui::Text("Materials");
    {
        MaterialMap::iterator it = Materials.begin();
        while (it != Materials.end())
        {
            ImGui::PushID(it->first.data());

            if (ImGui::TreeNode(it->first.c_str()))
            {
                //it->second->UpdateImGui();

                ImGui::TreePop();
            }

            ImGui::PopID();

            it++;
        }
    }

    ImGui::Separator();

    ImGui::Text("Textures");
    {
        TextureMap::iterator it = Textures.begin();
        while (it != Textures.end())
        {
            ImGui::PushID(it->first.data());

            if (ImGui::TreeNode(it->first.c_str()))
            {
                //it->second->UpdateImGui();

                ImGui::TreePop();
            }

            ImGui::PopID();

            it++;
        }
    }

    return changed;
}


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


void Model::Update()
{
    for (int i = 0; i < m_MeshList.size(); i++)
    {
        m_MeshList[i]->Update();
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
            else
            {
                it = resource->Materials.find("Default");
                if (it != resource->Materials.end())
                {
                    m_Material = (*it).second;
                }
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


void Mesh::Update()
{
    if (m_Material)
    {
        m_Material->Update();
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


static const char* g_shaderCode = R"(
//Texture2D diffuseTexture : register(t0);
//SamplerState normalSampler : register(s0);

cbuffer CB0 : register(b0)
{
    float4x4 MVPMatrix;
    float4 Color;
    float2 UVOffset;
    float2 dummy_CB0;
};

cbuffer CB1 : register(b1)
{
    float4 LightDirection;
    float4 EyeDirection;

    float4 DiffuseColor;
    float4 AmbientColor;
    float4 SpecularColor; // w : power
    float4 EmissiveColor;
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

    //float4 diffuseColor  = diffuseTexture.Sample(linearSampler, In.texcoord);
    //float4 ambientColor  = float4(1.0, 1.0, 1.0, 1.0);
    //float4 specularColor = specularTexture.Sample(linearSampler, In.texcoord);
    //float4 emissiveColor = float4(1.0, 1.0, 1.0, 1.0);

    float4 diffuseColor  = float4(DiffuseColor.xyz, 1.0);
    float4 ambientColor  = float4(AmbientColor.xyz, 1.0);
    float4 specularColor = float4(SpecularColor.xyz, 1.0);
    float4 emissiveColor = float4(EmissiveColor.xyz, 1.0);

    float3 light  = normalize(LightDirection.xyz);
    float3 eye    = normalize(EyeDirection.xyz);
    float diffuse = clamp(dot(In.normal.xyz, light), 0.1, 1.0);

    float3 halfLE  = normalize(light + eye);
    float specular = pow(clamp(dot(In.normal.xyz, halfLE), 0.0, 1.0), SpecularColor.w);

    float4 color = In.color * diffuseColor * float4(diffuse, diffuse, diffuse, 1.0);
    color += ambientColor;
    color += float4(specular * specularColor.x, specular * specularColor.y, specular * specularColor.z, 1.0);
    color += emissiveColor;

    return color;
}
)";


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

    m_Shader = std::make_unique<Shader>(
        m_Device,
        m_Context
        );
    bool result = m_Shader->Init(
        g_shaderCode,
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

    auto make_cbuffer = [&](void* initData, size_t size)
    {
        std::unique_ptr<ConstantBuffer> cbuffer = std::make_unique<ConstantBuffer>(
            m_Device,
            m_Context,
            initData,
            size
            );
        m_ConstantBuffers.push_back(std::move(cbuffer));
    };

    make_cbuffer(&m_CB0, sizeof(ConstantBufferData));
    make_cbuffer(&m_CB1, sizeof(ConstantBufferData_DiffuseLighting));

    SetDiffuseColor(glmEx::to_vec3(material.diffuse));
    SetAmbientColor(glmEx::to_vec3(material.ambient));
    SetSpecularColor(glmEx::to_vec3(material.specular));
    SetEmissiveColor(glmEx::to_vec3(material.emission));
    SetSpecularPower(material.shininess);

    return true;
}


bool Material::InitDefault()
{
    if (m_Resource.expired())
    {
        return false;
    }

    {
        std::shared_ptr<ModelResource> resource = m_Resource.lock();
        ModelResource::TextureMap::iterator it = resource->Textures.find("diffuse_default");

        if (it != resource->Textures.end())
        {
            m_Texture = (*it).second;
        }
    }

    m_Shader = std::make_unique<Shader>(
        m_Device,
        m_Context
        );
    bool result = m_Shader->Init(
        g_shaderCode,
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

    auto make_cbuffer = [&](void* initData, size_t size)
    {
        std::unique_ptr<ConstantBuffer> cbuffer = std::make_unique<ConstantBuffer>(
            m_Device,
            m_Context,
            initData,
            size
            );
        m_ConstantBuffers.push_back(std::move(cbuffer));
    };

    make_cbuffer(&m_CB0, sizeof(ConstantBufferData));
    make_cbuffer(&m_CB1, sizeof(ConstantBufferData_DiffuseLighting));

    SetDiffuseColor(glm::vec3(0.3f, 0.3f, 0.3f));
    SetAmbientColor(glm::vec3(0.0f, 0.0f, 0.0f));
    SetSpecularColor(glm::vec3(1.0f, 1.0f, 1.0f));
    SetEmissiveColor(glm::vec3(0.0f, 0.0f, 0.0f));
    SetSpecularPower(5.0f);

    return true;
}


void Material::Update()
{
    if (m_ConstantBufferChanged)
    {
        m_ConstantBuffers[0]->Update(&m_CB0);
        m_ConstantBuffers[1]->Update(&m_CB1);

        m_ConstantBufferChanged = false;
    }
}


void Material::SetMVPMatrix(const glm::mat4x4& mvpMatrix)
{
    m_CB0.MVPMatrix = mvpMatrix;
    m_ConstantBufferChanged = true;
}


void Material::SetEyeDirection(const glm::vec4& eyeDirection)
{
    m_CB1.EyeDirection = eyeDirection;
    m_ConstantBufferChanged = true;
}


void Material::SetLightDirection(const glm::vec4& lightDirection)
{
    m_CB1.LightDirection = lightDirection;
    m_ConstantBufferChanged = true;
}


void Material::SetDiffuseColor(const glm::vec3& diffuseColor)
{
    m_CB1.DiffuseColor.r = diffuseColor.r;
    m_CB1.DiffuseColor.g = diffuseColor.g;
    m_CB1.DiffuseColor.b = diffuseColor.b;
    m_ConstantBufferChanged = true;
}


void Material::SetAmbientColor(const glm::vec3& ambientColor)
{
    m_CB1.AmbientColor.r = ambientColor.r;
    m_CB1.AmbientColor.g = ambientColor.g;
    m_CB1.AmbientColor.b = ambientColor.b;
    m_ConstantBufferChanged = true;
}


void Material::SetSpecularColor(const glm::vec3& specularColor)
{
    m_CB1.SpecularColor.r = specularColor.r;
    m_CB1.SpecularColor.g = specularColor.g;
    m_CB1.SpecularColor.b = specularColor.b;
    m_ConstantBufferChanged = true;
}


void Material::SetEmissiveColor(const glm::vec3& emissiveColor)
{
    m_CB1.EmissiveColor.r = emissiveColor.r;
    m_CB1.EmissiveColor.g = emissiveColor.g;
    m_CB1.EmissiveColor.b = emissiveColor.b;
    m_ConstantBufferChanged = true;
}


void Material::SetSpecularPower(float power)
{
    m_CB1.SpecularColor.w = power;
    m_ConstantBufferChanged = true;
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

    for (size_t i = 0; i < m_ConstantBuffers.size(); i++)
    {
        if (m_ConstantBuffers[i])
        {
            m_ConstantBuffers[i]->SetVS(static_cast<u32>(i));
            m_ConstantBuffers[i]->SetPS(static_cast<u32>(i));
        }
    }

}

