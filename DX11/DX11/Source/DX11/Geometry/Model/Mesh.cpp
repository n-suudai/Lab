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



bool ModelResource::InitDefault(
    const ComPtr<ID3D11Device>& device,
    const ComPtr<ID3D11DeviceContext>& context
)
{
    std::string defaultTextureNames[] = {
        "Assets\\Image\\default_diffuse.png",
        "Assets\\Image\\default_specular.png",
    };

    for (int i = 0; i < _countof(defaultTextureNames); i++)
    {
        std::shared_ptr<Texture> texture = std::make_shared<Texture>(
            device,
            context
            );
        texture->Init(PathUtil::GetFullPath(defaultTextureNames[i]));

        Textures.insert(
            {
                PathUtil::GetFileNameWithoutExtension(defaultTextureNames[i]),
                texture
            }
        );
    }

    std::shared_ptr<Material> defaultMaterial = std::make_shared<Material>(
        device,
        context,
        shared_from_this()
        );

    defaultMaterial->InitDefault();
    Materials.insert(
        {
            "Default",
            defaultMaterial
        }
    );

    return true;
}


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
                changed |= it->second->UpdateImGui();

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


bool Model::InitAsTorus(u16 row, u16 column, f32 irad, f32 orad, const glm::vec4* color)
{
    if (m_Resource.expired())
    {
        return false;
    }

    std::shared_ptr<ModelResource> resource = m_Resource.lock();

    m_MeshList.resize(1);

    m_MeshList[0] = std::make_unique<Mesh>(
        m_Device,
        m_Context,
        resource
        );

    bool result = m_MeshList[0]->InitAsTorus(
        row,
        column,
        irad,
        orad,
        color
    );

    if (!result)
    {
        m_MeshList.clear();
    }

    return result;
}


bool Model::InitAsSphere(u16 x, u16 y, f32 rad, const glm::vec4* color)
{
    if (m_Resource.expired())
    {
        return false;
    }

    std::shared_ptr<ModelResource> resource = m_Resource.lock();

    m_MeshList.resize(1);

    m_MeshList[0] = std::make_unique<Mesh>(
        m_Device,
        m_Context,
        resource
        );

    bool result = m_MeshList[0]->InitAsSphere(
        x,
        y,
        rad,
        color
    );

    if (!result)
    {
        m_MeshList.clear();
    }

    return result;
}


bool Model::InitAsCube(f32 scale, const glm::vec4* color)
{
    if (m_Resource.expired())
    {
        return false;
    }

    std::shared_ptr<ModelResource> resource = m_Resource.lock();

    m_MeshList.resize(1);

    m_MeshList[0] = std::make_unique<Mesh>(
        m_Device,
        m_Context,
        resource
        );

    bool result = m_MeshList[0]->InitAsCube(
        scale,
        color
    );

    if (!result)
    {
        m_MeshList.clear();
    }

    return result;
}


bool Model::InitAsCylinder(u16 x, u16 y, f32 rad, f32 height, const glm::vec4* color)
{
    if (m_Resource.expired())
    {
        return false;
    }

    std::shared_ptr<ModelResource> resource = m_Resource.lock();

    m_MeshList.resize(1);

    m_MeshList[0] = std::make_unique<Mesh>(
        m_Device,
        m_Context,
        resource
        );

    bool result = m_MeshList[0]->InitAsCylinder(
        x,
        y,
        rad,
        height,
        color
    );

    if (!result)
    {
        m_MeshList.clear();
    }

    return result;
}


void Model::Update(float deltaTime)
{
    for (int i = 0; i < m_MeshList.size(); i++)
    {
        m_MeshList[i]->Update(deltaTime);
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

    if (!m_Material)
    {
        ModelResource::MaterialMap::iterator it = resource->Materials.find("Default");
        if (it != resource->Materials.end())
        {
            m_Material = (*it).second;
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


// トーラスとして初期化
bool Mesh::InitAsTorus(u16 row, u16 column, f32 irad, f32 orad, const glm::vec4* colorIn)
{
    if (m_Resource.expired())
    {
        return false;
    }

    std::shared_ptr<ModelResource> resource = m_Resource.lock();

    if (!m_Material)
    {
        ModelResource::MaterialMap::iterator it = resource->Materials.find("Default");
        if (it != resource->Materials.end())
        {
            m_Material = (*it).second;
        }
        else
        {
            return false;
        }
    }

    std::vector<Vertex_PositionColorNormalTexture> vertices;   // 頂点配列
    std::vector<u16> indices;    // インデックス配列

    constexpr float PI = glm::pi<float>();

    // 頂点配列を作成
    for (u16 ix = 0; ix <= row; ++ix)
    {
        f32 r = PI * 2.0f / row * ix;
        f32 rr = std::cosf(r);
        f32 ry = std::sinf(r);

        for (u16 iy = 0; iy <= column; ++iy)
        {
            f32 tr = PI * 2.0f / column * iy;

            Vertex_PositionColorNormalTexture v;
            // 頂点
            v.Position.x = (rr * irad + orad) * std::cosf(tr);
            v.Position.y = ry * irad;
            v.Position.z = (rr * irad + orad) * std::sinf(tr);
            v.Position.w = 1.0f;

            // 頂点色
            if (colorIn != nullptr)
            {
                v.Color.x = colorIn->x;
                v.Color.y = colorIn->y;
                v.Color.z = colorIn->z;
                v.Color.w = colorIn->w;
            }
            else
            {
                glm::vec3 color = glm::rgbColor(glm::vec3(360.0f / column * iy, 1.0f, 1.0f));
                v.Color.x = color.x;
                v.Color.y = color.y;
                v.Color.z = color.z;
                v.Color.w = 1.0f;
            }

            // 法線
            v.Normal.x = rr * std::cosf(tr);
            v.Normal.y = ry;
            v.Normal.z = rr * std::sinf(tr);
            v.Normal.w = 1.0f;

            // テクスチャ座標
            v.Texture.x = 1.0f / column * iy;
            v.Texture.y = 1.0f / row * ix + 0.5f;
            if (v.Texture.y > 1.0f)
            {
                v.Texture.y -= 1.0f;
            }
            v.Texture.y = 1.0f - v.Texture.y;

            vertices.push_back(v);
        }
    }

    // インデックス配列を作成
    for (u16 ix = 0; ix < row; ++ix)
    {
        for (u16 iy = 0; iy < column; ++iy)
        {
            u16 r = (column + 1) * ix + iy;

            indices.push_back(r);
            indices.push_back(r + column + 1);
            indices.push_back(r + 1);

            indices.push_back(r + column + 1);
            indices.push_back(r + column + 2);
            indices.push_back(r + 1);
        }
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


bool Mesh::InitAsSphere(u16 x, u16 y, f32 rad, const glm::vec4* colorIn)
{
    if (m_Resource.expired())
    {
        return false;
    }

    std::shared_ptr<ModelResource> resource = m_Resource.lock();

    if (!m_Material)
    {
        ModelResource::MaterialMap::iterator it = resource->Materials.find("Default");
        if (it != resource->Materials.end())
        {
            m_Material = (*it).second;
        }
        else
        {
            return false;
        }
    }

    std::vector<Vertex_PositionColorNormalTexture> vertices;   // 頂点配列
    std::vector<u16> indices;    // インデックス配列

    // 頂点配列を作成
    for (u16 iy = 0; iy <= y; iy++)
    {
        for (u16 ix = 0; ix <= x; ix++)
        {
            f32 r = glm::radians(180.0f / y * iy);
            f32 ry = std::cosf(r);
            f32 rr = std::sinf(r);

            Vertex_PositionColorNormalTexture v;
            f32 tr = glm::radians(360.0f / x * ix);

            // 頂点
            v.Position.x = rr * rad * std::cosf(tr);
            v.Position.y = ry * rad;
            v.Position.z = rr * rad * std::sinf(tr);
            v.Position.w = 1.0f;

            // 頂点色
            if (colorIn != nullptr)
            {
                v.Color.x = colorIn->x;
                v.Color.y = colorIn->y;
                v.Color.z = colorIn->z;
                v.Color.w = colorIn->w;
            }
            else
            {
                glm::vec3 color = glm::rgbColor(glm::vec3(360.0f / x * ix, 1.0f, 1.0f));
                v.Color.x = color.x;
                v.Color.y = color.y;
                v.Color.z = color.z;
                v.Color.w = 1.0f;
            }

            // 法線
            v.Normal.x = rr * std::cosf(tr);
            v.Normal.y = ry;
            v.Normal.z = rr * std::sinf(tr);
            v.Normal.w = 1.0f;

            // テクスチャ座標
            v.Texture.x = 1.0f / x * ix;
            v.Texture.y = 1.0f / y * iy;

            vertices.push_back(v);
        }
    }

    // インデックス
    for (u16 iy = 0; iy <= y; ++iy)
    {
        for (u16 ix = 0; ix < x; ++ix)
        {
            u16 i0 = ix + x * iy;
            u16 i1 = i0 + 1;
            u16 i2 = i0 + x + 1;
            u16 i3 = i2 + 1;

            indices.push_back(i0);
            indices.push_back(i1);
            indices.push_back(i2);

            indices.push_back(i1);
            indices.push_back(i3);
            indices.push_back(i2);
        }
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


bool Mesh::InitAsCube(f32 scale, const glm::vec4* colorIn)
{
    if (m_Resource.expired())
    {
        return false;
    }

    std::shared_ptr<ModelResource> resource = m_Resource.lock();

    if (!m_Material)
    {
        ModelResource::MaterialMap::iterator it = resource->Materials.find("Default");
        if (it != resource->Materials.end())
        {
            m_Material = (*it).second;
        }
        else
        {
            return false;
        }
    }

    const f32 hs = scale * 0.5f;

    // 頂点配列
    std::vector<Vertex_PositionColorNormalTexture> vertices = {
        //          Position                          Color                                Normal                                   Texture
        { glm::vec4(-hs, -hs,  hs, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(-1.0f, -1.0f,  1.0f, 1.0f),  glm::vec2(0.0f, 0.0f) },
        { glm::vec4(hs, -hs,  hs, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f, -1.0f,  1.0f, 1.0f),  glm::vec2(1.0f, 0.0f) },
        { glm::vec4(hs,  hs,  hs, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f,  1.0f,  1.0f, 1.0f),  glm::vec2(1.0f, 1.0f) },
        { glm::vec4(-hs,  hs,  hs, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(-1.0f,  1.0f,  1.0f, 1.0f),  glm::vec2(0.0f, 1.0f) },

        { glm::vec4(-hs, -hs, -hs, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f),  glm::vec2(0.0f, 0.0f) },
        { glm::vec4(-hs,  hs, -hs, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(-1.0f,  1.0f, -1.0f, 1.0f),  glm::vec2(1.0f, 0.0f) },
        { glm::vec4(hs,  hs, -hs, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f,  1.0f, -1.0f, 1.0f),  glm::vec2(1.0f, 1.0f) },
        { glm::vec4(hs, -hs, -hs, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f, -1.0f, -1.0f, 1.0f),  glm::vec2(0.0f, 1.0f) },

        { glm::vec4(-hs,  hs, -hs, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(-1.0f,  1.0f, -1.0f, 1.0f),  glm::vec2(0.0f, 0.0f) },
        { glm::vec4(-hs,  hs,  hs, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(-1.0f,  1.0f,  1.0f, 1.0f),  glm::vec2(1.0f, 0.0f) },
        { glm::vec4(hs,  hs,  hs, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f,  1.0f,  1.0f, 1.0f),  glm::vec2(1.0f, 1.0f) },
        { glm::vec4(hs,  hs, -hs, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f,  1.0f, -1.0f, 1.0f),  glm::vec2(0.0f, 1.0f) },

        { glm::vec4(-hs, -hs, -hs, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f),  glm::vec2(0.0f, 0.0f) },
        { glm::vec4(hs, -hs, -hs, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f, -1.0f, -1.0f, 1.0f),  glm::vec2(1.0f, 0.0f) },
        { glm::vec4(hs, -hs,  hs, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f, -1.0f,  1.0f, 1.0f),  glm::vec2(1.0f, 1.0f) },
        { glm::vec4(-hs, -hs,  hs, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(-1.0f, -1.0f,  1.0f, 1.0f),  glm::vec2(0.0f, 1.0f) },

        { glm::vec4(hs, -hs, -hs, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f, -1.0f, -1.0f, 1.0f),  glm::vec2(0.0f, 0.0f) },
        { glm::vec4(hs,  hs, -hs, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f,  1.0f, -1.0f, 1.0f),  glm::vec2(1.0f, 0.0f) },
        { glm::vec4(hs,  hs,  hs, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f,  1.0f,  1.0f, 1.0f),  glm::vec2(1.0f, 1.0f) },
        { glm::vec4(hs, -hs,  hs, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f, -1.0f,  1.0f, 1.0f),  glm::vec2(0.0f, 1.0f) },

        { glm::vec4(-hs, -hs, -hs, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f),  glm::vec2(1.0f, 0.0f) },
        { glm::vec4(-hs, -hs,  hs, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(-1.0f, -1.0f,  1.0f, 1.0f),  glm::vec2(1.0f, 1.0f) },
        { glm::vec4(-hs,  hs,  hs, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(-1.0f,  1.0f,  1.0f, 1.0f),  glm::vec2(0.0f, 1.0f) },
        { glm::vec4(-hs,  hs, -hs, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(-1.0f,  1.0f, -1.0f, 1.0f),  glm::vec2(0.0f, 0.0f) },
    };

    // インデックス配列
    std::vector<u16> indices = {
        0,  1,  2,  0,  2,  3,
        4,  5,  6,  4,  6,  7,
        8,  9,  10, 8,  10, 11,
        12, 13, 14, 12, 14, 15,
        16, 17, 18, 16, 18, 19,
        20, 21, 22, 20, 22, 23,
    };

    for (size_t i = 0; i < vertices.size(); i++)
    {
        if (colorIn != nullptr)
        {
            vertices[i].Color.x = colorIn->x;
            vertices[i].Color.y = colorIn->y;
            vertices[i].Color.z = colorIn->z;
            vertices[i].Color.w = colorIn->w;
        }
        else
        {
            glm::vec3 color = glm::rgbColor(glm::vec3(360.0f / vertices.size() * i, 1.0f, 1.0f));
            vertices[i].Color.x = color.x;
            vertices[i].Color.y = color.y;
            vertices[i].Color.z = color.z;
            vertices[i].Color.w = 1.0f;
        }
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


bool Mesh::InitAsCylinder(u16 x, u16 y, f32 rad, f32 height, const glm::vec4* colorIn)
{
    if (m_Resource.expired())
    {
        return false;
    }

    std::shared_ptr<ModelResource> resource = m_Resource.lock();

    if (!m_Material)
    {
        ModelResource::MaterialMap::iterator it = resource->Materials.find("Default");
        if (it != resource->Materials.end())
        {
            m_Material = (*it).second;
        }
        else
        {
            return false;
        }
    }

    if (x <= 2 || y <= 2) { return false; }

    std::vector<Vertex_PositionColorNormalTexture> vertices;   // 頂点配列
    std::vector<u16> indices;    // インデックス配列

    for (u16 iy = 0; iy <= y; ++iy)
    {
        for (u16 ix = 0; ix <= x; ++ix)
        {
            Vertex_PositionColorNormalTexture v;
            v.Position.w = 1.0f;

            // 頂点座標
            if (iy == 0 || iy == y)
            {
                v.Position.x = 0.0f;
                v.Position.z = 0.0f;
                v.Position.y = height * 0.5f;
                if (iy == y)
                {
                    v.Position.y = -height * 0.5f;
                }
            }
            else
            {
                const f32 r = glm::radians(360.0f / x * ix);
                v.Position.x = rad * std::cosf(r);
                v.Position.z = rad * std::sinf(r);

                const f32 h = 0.5f - (1.0f / (y - 2) * (iy - 1));
                v.Position.y = height * h;
            }

            // 頂点色
            if (colorIn != nullptr)
            {
                v.Color.x = colorIn->x;
                v.Color.y = colorIn->y;
                v.Color.z = colorIn->z;
                v.Color.w = colorIn->w;
            }
            else
            {
                glm::vec3 color = glm::rgbColor(glm::vec3(360.0f / x * ix, 1.0f, 1.0f));
                v.Color.x = color.x;
                v.Color.y = color.y;
                v.Color.z = color.z;
                v.Color.w = 1.0f;
            }

            // 法線
            v.Normal.w = 1.0f;
            if (iy == 0 || iy == y)
            {
                v.Normal.x = 0.0f;
                v.Normal.z = 0.0f;
                v.Normal.y = 1.0f;
                if (iy == y)
                {
                    v.Normal.y = -1.0f;
                }
            }
            else
            {
                const f32 r = glm::radians(360.0f / x * ix);
                v.Normal.x = rad * std::cosf(r);
                v.Normal.z = rad * std::sinf(r);
                v.Normal.y = 0.0f;
            }

            // UV座標
            v.Texture.x = (1.0f / x) * ix;
            v.Texture.y = (1.0f / y) * iy;

            vertices.push_back(v);
        }
    }

    // インデックス
    for (u16 iy = 0; iy <= y; ++iy)
    {
        for (u16 ix = 0; ix < x; ++ix)
        {
            u16 i0 = ix + x * iy;
            u16 i1 = i0 + 1;
            u16 i2 = i0 + x + 1;
            u16 i3 = i2 + 1;

            indices.push_back(i0);
            indices.push_back(i1);
            indices.push_back(i2);

            indices.push_back(i1);
            indices.push_back(i3);
            indices.push_back(i2);
        }
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


void Mesh::Update(float deltaTime)
{
    if (m_Material)
    {
        m_Material->Update(deltaTime);
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
Texture2D diffuseTexture : register(t0);
SamplerState normalSampler : register(s0);

cbuffer CB0 : register(b0)
{
    float4x4 MVPMatrix;
    float4x4 ModelMatrix;
    float4 Color;
    float2 UVOffset;
    float Time;
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
    float2 texcoord : TEXCOORD0;
    float4 worldPosition   : POSITION;
};

VS_OUTPUT vs_main(VS_INPUT In) {
    VS_OUTPUT Out = (VS_OUTPUT)0;
    Out.worldPosition = mul(In.position, ModelMatrix);
    Out.position = mul(In.position, MVPMatrix);
    Out.color = Color;
    Out.texcoord = In.texcoord + UVOffset;
    Out.normal = In.normal;
    return Out;
}


float rand(float co) {
    return frac(sin(dot(co, 12.9898)) * 43758.5453);
}

float rand2(float2 p) {
    return frac(sin(dot(p, float2(12.9898, 78.233))) * 43758.5453);
}

float graph(float x, float noise_size, float alpha) {
    return pow(noise_size * abs(sin(10.0 * x) * (-sin(x * 2.0) + 1.0)) * 0.5, 2.0) * alpha;
}

float noise(float2 coord) {
    return rand2(coord + floor(Time * 10.0));
}

float scanline(float y, float lineAmount) {
    return frac(y * lineAmount - Time);
}

float overlay_element(float c, float o) {
    if (c < 0.5) {
        c = 2.0 * c * o;
    }
    else {
        c = 1.0 - 2.0 * (1.0 - c) * (1.0 - o);
    }
    return c;
}

float4 overlay(float4 color, float4 overlayColor) {
    color.r = overlay_element(color.r, overlayColor.r);
    color.g = overlay_element(color.g, overlayColor.g);
    color.b = overlay_element(color.b, overlayColor.b);
    return color;
}

float4 ps_main(VS_OUTPUT In) : SV_TARGET {

    float4 diffuseColor  = diffuseTexture.Sample(normalSampler, In.texcoord);
    //float4 ambientColor  = float4(1.0, 1.0, 1.0, 1.0);
    //float4 specularColor = specularTexture.Sample(normalSampler, In.texcoord);
    //float4 emissiveColor = float4(1.0, 1.0, 1.0, 1.0);

    diffuseColor  = diffuseColor * float4(DiffuseColor.xyz, 1.0);
    float4 ambientColor  = float4(AmbientColor.xyz, 1.0);
    float4 specularColor = float4(SpecularColor.xyz, 1.0);
    float4 emissiveColor = float4(EmissiveColor.xyz, 1.0);

    float3 light  = normalize(LightDirection.xyz);
    float3 eye    = normalize(EyeDirection.xyz);
    float diffuse = clamp(dot(In.normal.xyz, light), 0.1, 1.0);

    float3 halfLE  = normalize(light + eye);
    float specular = pow(clamp(dot(In.normal.xyz, halfLE), 0.0, 1.0), SpecularColor.w);

    float4 color = diffuseColor;
    //float4 color = In.color * diffuseColor * float4(diffuse, diffuse, diffuse, 1.0);
    //color.rgb += ambientColor.rgb;
    //color.rgb += float3(specular * specularColor.x, specular * specularColor.y, specular * specularColor.z);
    //color.rgb += emissiveColor.rgb;

    //color -= noise(In.position.xy) * 0.5;

    float fScanLineX = scanline(In.worldPosition.x, 15.0) - 0.5; // -0.5 ~ 0.5
    float fScanLineY = scanline(In.worldPosition.y, 15.0) - 0.5; // -0.5 ~ 0.5

    fScanLineX = step(abs(fScanLineX), 0.4); // 両端 0.1 分を 0.0 へ
    fScanLineY = step(abs(fScanLineY), 0.4); // 両端 0.1 分を 0.0 へ

    color.a = 1.0 - (fScanLineX * fScanLineY); // 端の部分 0.1 分を 1.0 へ

    color.rgb *= overlay(color, float4(0.5, 0, 0, 1)).rgb;

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
    , m_PrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
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
        ModelResource::TextureMap::iterator it = resource->Textures.find("default_diffuse");

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

    SetDiffuseColor(glm::vec3(1.0f, 1.0f, 1.0f));
    SetAmbientColor(glm::vec3(0.0f, 0.0f, 0.0f));
    SetSpecularColor(glm::vec3(1.0f, 1.0f, 1.0f));
    SetEmissiveColor(glm::vec3(0.0f, 0.0f, 0.0f));
    SetSpecularPower(5.0f);

    return true;
}


bool Material::UpdateImGui()
{
    bool changed = false;

    m_Sampler->UpdateImGui();

    m_BlendState->UpdateImGui();

    m_RasterizerState->UpdateImGui();

    ImGui::Text("ConstantBuffers");
    if (ImGui::TreeNode("CB0"))
    {
        changed |= m_CB0.UpdateImGui();
        m_ConstantBufferChanged = true;
        ImGui::TreePop();
    }
    
    if (ImGui::TreeNode("CB1"))
    {
        changed |= m_CB1.UpdateImGui();
        m_ConstantBufferChanged = true;
        ImGui::TreePop();
    }

    m_Shader->UpdateImGui();

    return changed;
}

float wrap(float x, float low, float high)
{
    assert(low < high);
    const float n = std::fmod(x - low, high - low);
    return (n >= 0) ? (n + low) : (n + high);
}


void Material::Update(float deltaTime)
{
    // CB0は時間を含むので毎回更新
    m_CB0.Time = wrap(m_CB0.Time + deltaTime, 0.0f, 1000.0f);
    m_ConstantBuffers[0]->Update(&m_CB0);

    if (m_ConstantBufferChanged)
    {
        m_ConstantBuffers[1]->Update(&m_CB1);

        m_ConstantBufferChanged = false;
    }
}


void Material::SetMVPMatrix(const glm::mat4x4& mvpMatrix)
{
    m_CB0.MVPMatrix = mvpMatrix;
    m_ConstantBufferChanged = true;
}


void Material::SetModelMatrix(const glm::mat4x4& modelMatrix)
{
    m_CB0.ModelMatrix = modelMatrix;
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


void Material::SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY primitiveTopology)
{
    m_PrimitiveTopology = primitiveTopology;
}


void Material::Set()
{
    m_Context->IASetPrimitiveTopology(m_PrimitiveTopology);

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

