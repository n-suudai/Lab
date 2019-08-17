#include "DiffuseLightingDemo.h"
#include "DX11/Geometry/Mesh.h"



DiffuselightingDemo::DiffuselightingDemo(
    const ComPtr<ID3D11Device>& device,
    const ComPtr<ID3D11DeviceContext>& context,
    const Size2D& clientSize
)
    : m_Device(device)
    , m_Context(context)
    , m_ForceUpdateModel(false)
{
    m_Resource = std::make_shared<ModelResource>();

    m_Model = std::make_unique<Model>(
        m_Device,
        m_Context,
        m_Resource
        );
    m_Model->Init("Assets\\Model\\cat\\cat.obj");

    m_Camera.eye = glm::vec3(0.0f, 2.8f, -6.0f);
    m_Camera.center = glm::vec3(0.0f, 0.0f, 0.0f);
    m_Camera.screenSize.x = static_cast<float>(clientSize.width);
    m_Camera.screenSize.y = static_cast<float>(clientSize.height);
    m_Camera.UpdateMatrix();

    m_Transform.UpdateMatrix();

    m_ForceUpdateModel = true;
}


DiffuselightingDemo::~DiffuselightingDemo()
{

}


void DiffuselightingDemo::Update()
{
    bool changed = m_ForceUpdateModel;

    if (m_Transform.UpdateImGui("ModelTransform"))
    {
        m_Transform.UpdateMatrix();
        changed = true;
    }

    if (m_Camera.UpdateImGui())
    {
        m_Camera.UpdateMatrix();
        changed = true;
    }

    if (changed)
    {
        ConstantBufferData data;
        data.MVPMatrix = glm::transpose
        (
            m_Camera.perspectiveMatrix * m_Camera.viewMatrix * m_Transform.matrix
        );

        m_Model->Update(data);
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

