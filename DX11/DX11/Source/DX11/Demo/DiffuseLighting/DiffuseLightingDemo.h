#pragma once

#include "DX11/Demo/Demo.h"
#include "DX11/DX11Prerequisites.h"
#include "DX11/Geometry/Camera.h"
#include "DX11/Geometry/Transform.h"


struct ModelResource;
class Model;
class DiffuselightingDemo : public Demo
{
public:
    DiffuselightingDemo(
        const ComPtr<ID3D11Device>& device,
        const ComPtr<ID3D11DeviceContext>& context,
        const Size2D& clientSize
    );

    virtual ~DiffuselightingDemo();

    virtual void Update(float deltaTime) override;

    virtual void Render() override;

    virtual void OnResizedBuffer(const Size2D& size) override;

private:
    enum ModelType
    {
        ModelType_Cube,
        ModelType_Sphere,
        ModelType_Torus,
        ModelType_Cylinder,
        ModelType_File,
        ModelType_Num,
    };

    ComPtr<ID3D11Device>        m_Device;
    ComPtr<ID3D11DeviceContext> m_Context;

    std::shared_ptr<ModelResource>  m_Resource;
    std::unique_ptr<Model>          m_Model;
    Camera                          m_Camera;
    Transform                       m_Transform;
    glm::vec4                       m_LightDirection; // 逆行列変換前

    ModelType                       m_ModelType;
    bool                            m_EnableRotateAnimation;

    bool                            m_ForceUpdateModel;
};

