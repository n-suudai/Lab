#include "Sampler.hpp"
#include "../Utils/ResultUtil.hpp"
#include "../External/ImGui/ImGui_DX11.h"
#include <map>


Sampler::Sampler(
    const ComPtr<ID3D11Device>& device,
    const ComPtr<ID3D11DeviceContext>& context
)
    : m_Device(device)
    , m_Context(context)
    , m_SamplerDesc(D3D11_DEFAULT)
{
    FLOAT borderColor[4] = { 0.0f };
    m_SamplerDesc = CD3D11_SAMPLER_DESC(
        D3D11_FILTER_ANISOTROPIC,       // サンプリング時に使用するフィルタ。ここでは異方性フィルターを使用する(D3D11_FILTER_ANISOTROPIC)
        D3D11_TEXTURE_ADDRESS_WRAP,     // 0 ～ 1 の範囲外にある u テクスチャー座標の描画方法
        D3D11_TEXTURE_ADDRESS_WRAP,     // 0 ～ 1 の範囲外にある v テクスチャー座標
        D3D11_TEXTURE_ADDRESS_WRAP,     // 0 ～ 1 の範囲外にある w テクスチャー座標
        0,                              // 計算されたミップマップ レベルからのバイアス
        16,                             // サンプリングに異方性補間を使用している場合の限界値。有効な値は 1 ～ 16 
        D3D11_COMPARISON_ALWAYS,        // 比較オプション
        borderColor,                    // 境界色
        0,                              // アクセス可能なミップマップの下限値
        D3D11_FLOAT32_MAX               // アクセス可能なミップマップの上限値
    );

    Init(m_SamplerDesc);
}


Sampler::~Sampler()
{

}


void Sampler::UpdateImGui()
{
    bool changed = false;

    CD3D11_SAMPLER_DESC newDesc(m_SamplerDesc);

    // Filter
    changed |= ImGui_DX11::ComboEnum("Filter", &newDesc.Filter);

    // TextureAddressMode
    changed |= ImGui_DX11::ComboEnum("AddressModeU", &newDesc.AddressU);
    changed |= ImGui_DX11::ComboEnum("AddressModeV", &newDesc.AddressV);
    changed |= ImGui_DX11::ComboEnum("AddressModeW", &newDesc.AddressW);

    // MipLODBias
    changed |= ImGui::DragFloat("MipLODBias", &newDesc.MipLODBias);

    // MaxAnisotropy
    changed |= ImGuiEx::DragUInt("MaxAnisotropy", &newDesc.MaxAnisotropy);

    // ComparisonFunc
    changed |= ImGui_DX11::ComboEnum("ComparisonFunc", &newDesc.ComparisonFunc);

    // BorderColor
    changed |= ImGui::ColorEdit4("BorderColor", newDesc.BorderColor);

    // MinLOD
    changed |= ImGui::DragFloat("MinLOD", &newDesc.MinLOD);

    // MaxLOD
    changed |= ImGui::DragFloat("MaxLOD", &newDesc.MaxLOD);

    if (changed)
    {
        Init(newDesc);
    }
}


bool Sampler::Init(const CD3D11_SAMPLER_DESC& newDesc)
{
    ComPtr<ID3D11SamplerState> sampler;

    ResultUtil result = m_Device->CreateSamplerState(
        &newDesc,
        &sampler
    );

    if (!result)
    {
        result.ShowMessageBox("device->CreateSamplerState");
        return false;
    }
    else
    {
        m_SamplerState = sampler;
        m_SamplerDesc = newDesc;
    }

    return true;
}


void Sampler::Set()
{
    ID3D11SamplerState* pSamplerStates[] = {
        m_SamplerState.Get()
    };
    m_Context->PSSetSamplers(0, _countof(pSamplerStates), pSamplerStates);
}
