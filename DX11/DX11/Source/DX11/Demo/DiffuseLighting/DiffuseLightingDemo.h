#pragma once

#include "DX11/Demo/Demo.h"
#include "DX11/DX11Prerequisites.h"

class Sampler;
class Texture;
class BlendState;
class ConstantBuffer;
class BitmapFont;
class DiffuselightingDemo : public Demo
{
public:
    DiffuselightingDemo(
        const ComPtr<ID3D11Device>& device,
        const ComPtr<ID3D11DeviceContext>& context
    );

    virtual ~DiffuselightingDemo();

    virtual void Update() override;

    virtual void Render() override;

    virtual void OnResizedBuffer(const Size2D& size) override;

private:
    ComPtr<ID3D11Device>        m_Device;
    ComPtr<ID3D11DeviceContext> m_Context;
};

