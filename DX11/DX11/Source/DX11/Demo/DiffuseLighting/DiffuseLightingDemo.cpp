#include "DiffuseLightingDemo.h"
#include "DX11/Geometry/Model/Mesh.h"
#include "DX11/External/ImGui/ImGuiEx.h"



DiffuselightingDemo::DiffuselightingDemo(
    const ComPtr<ID3D11Device>& device,
    const ComPtr<ID3D11DeviceContext>& context,
    const Size2D& clientSize
)
    : m_Device(device)
    , m_Context(context)
    , m_ModelType(ModelType_Cube)
    , m_EnableRotateAnimation(true)
    , m_ForceUpdateModel(false)
{
    m_Resource = std::make_shared<ModelResource>();
    m_Resource->InitDefault(
        m_Device,
        m_Context
    );

    glm::vec4 color(1.0f, 1.0f, 1.0f, 1.0f);
    m_Model = std::make_unique<Model>(
        m_Device,
        m_Context,
        m_Resource
        );
    m_Model->InitAsCube(2.0f, &color);

    m_Camera.eye = glm::vec3(0.0f, 2.8f, 6.0f);
    m_Camera.center = glm::vec3(0.0f, 0.0f, 0.0f);
    m_Camera.screenSize.x = static_cast<float>(clientSize.width);
    m_Camera.screenSize.y = static_cast<float>(clientSize.height);
    m_Camera.UpdateMatrix();

    m_Transform.UpdateMatrix();

    m_LightDirection = glm::vec4(8.0f, 2.8f, 6.0f, 1.0f);

    m_ForceUpdateModel = true;
}


DiffuselightingDemo::~DiffuselightingDemo()
{

}


void DiffuselightingDemo::Update(float deltaTime)
{
    bool changed = m_ForceUpdateModel;

    ImGui::Checkbox("EnableRotateAnim", &m_EnableRotateAnimation);

    if (m_EnableRotateAnimation)
    {
        static f32 delta = 0.0f;
        delta += 0.003f;

        m_Transform.rotate.y = delta;
        //m_Transform.rotate.z = delta;
        m_Transform.UpdateMatrix();
        changed = true;
    }

    {
        static const char* models[ModelType_Num] = {
            "Cube",
            "Sphere",
            "Torus",
            "Cylinder",
            "Monkey",
        };

        int modelType = static_cast<int>(m_ModelType);
        static const char* current = models[modelType];

        if (ImGui::Combo("ModelType", &modelType, models, _countof(models)))
        {
            glm::vec4 color(1.0f, 1.0f, 1.0f, 1.0f);

            switch (modelType)
            {
            case ModelType_Cube:
                m_Model->InitAsCube(2.0f, &color);
                break;

            case ModelType_Sphere:
                m_Model->InitAsSphere(32, 32, 1.0f, &color);
                break;

            case ModelType_Torus:
                m_Model->InitAsTorus(32, 32, 0.5f, 1.5f, &color);
                break;

            case ModelType_Cylinder:
                m_Model->InitAsCylinder(32, 32, 1.0f, 2.0f, &color);
                break;

            case ModelType_File:
                m_Model->Init("Assets\\Model\\monkey\\monkey.obj");
                break;
            }
            changed = true;
        }
    }

    ImGui::Separator();

    changed |= m_Resource->UpdateImGui();

    ImGui::Separator();

    changed |= ImGuiEx::DragVec4("LightDirection", &m_LightDirection, 0.01f);

    if (m_Camera.UpdateImGui())
    {
        m_Camera.UpdateMatrix();
        changed = true;
    }

    if (m_Transform.UpdateImGui("ModelTransform"))
    {
        m_Transform.UpdateMatrix();
        changed = true;
    }

    // 本当はモデルごとに設定する必要があるが、とりあえずここで直接定数バッファを更新する
    if (changed)
    {
        glm::mat4x4 inverseModelMatrix = glm::inverse(m_Transform.matrix);

        glm::vec4 eyeDirection = inverseModelMatrix * glm::vec4(m_Camera.eye - m_Camera.center, 1.0f);
        glm::normalize(eyeDirection);

        glm::vec4 lightDirection = inverseModelMatrix * m_LightDirection;
        glm::normalize(lightDirection);

        glm::mat4x4 mvpMatrix = glm::transpose
        (
            m_Camera.perspectiveMatrix * m_Camera.viewMatrix * m_Transform.matrix
        );

        ModelResource::MaterialMap::iterator material_it = m_Resource->Materials.begin();
        while(material_it != m_Resource->Materials.end())
        {
            material_it->second->SetMVPMatrix(mvpMatrix);
            material_it->second->SetModelMatrix(glm::transpose(m_Transform.matrix));
            material_it->second->SetEyeDirection(eyeDirection);
            material_it->second->SetLightDirection(lightDirection);
            material_it++;
        }

        m_Model->Update(deltaTime);
    }

    m_ForceUpdateModel = false;
}


void DiffuselightingDemo::Render()
{
    m_Model->Draw();
}


void DiffuselightingDemo::OnResizedBuffer(const Size2D& size)
{
    m_Camera.screenSize.x = static_cast<float>(size.width);
    m_Camera.screenSize.y = static_cast<float>(size.height);
    m_Camera.UpdateMatrix();
    m_ForceUpdateModel = true;
}

