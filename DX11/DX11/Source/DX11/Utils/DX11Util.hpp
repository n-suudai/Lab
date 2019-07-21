
#pragma once

#include "../DX11Prerequisites.hpp"
#include <fstream>
#include <vector>
#include <iterator>
#include "ResultUtil.hpp"


namespace DX11Util
{

    // DXGI ピクセルフォーマットからビット数を返す
    UINT DXGIFormatBitsPerPixel(DXGI_FORMAT fmt);

    // DXGI ピクセルフォーマットからSysMemPitchを返す
    UINT DXGIFormatSystemPitch(DXGI_FORMAT fmt, int iWidth);

    // PNG 読み込み
    bool LoadPng(const std::string& fileName, std::vector<BYTE>& image, DXGI_FORMAT& format, Size2D& size);

    // ファイルの読み込み（バイナリ）
    bool ReadFile(const std::string& fileName, std::vector<BYTE>& out);

    // バイナリデータからテクスチャを作成
    bool CreateTextureFromData(
        const ComPtr<ID3D11Device>& device,
        DXGI_FORMAT format,
        const Size2D& size,
        const u8* image,
        u32 imageSize,
        ComPtr<ID3D11Texture2D>& outTexture2D,
        ComPtr<ID3D11ShaderResourceView>& outShaderResourceView,
        D3D11_USAGE usage = D3D11_USAGE_DEFAULT,
        UINT cpuAccessFlag = 0
    );

    // PNG ファイルからテクスチャを生成
    bool CreateTextureFromFile(
        const ComPtr<ID3D11Device>& device,
        const std::string& fileName,
        ComPtr<ID3D11Texture2D>& outTexture2D,
        ComPtr<ID3D11ShaderResourceView>& outShaderResourceView
    );

    // 複数の PNG ファイルからテクスチャ配列を作成
    bool CreateTextureArrayFromFile(
        const ComPtr<ID3D11Device>& device,
        std::vector<std::string> fileNames,
        ComPtr<ID3D11Texture2D>& outTexture2D,
        ComPtr<ID3D11ShaderResourceView>& outShaderResourceView,
        Size2D& outTextureSize
    );

    // 各種バッファの作成
    bool CreateBuffer(
        const ComPtr<ID3D11Device>& device,
        const void* pInitData,
        UINT byteWidth,
        D3D11_BIND_FLAG bindFlag,
        UINT stride,
        ComPtr<ID3D11Buffer>& outBuffer
    );

    // ラスタライザーステートを作成
    bool CreateRasterizerState(
        const ComPtr<ID3D11Device>& device,
        D3D11_CULL_MODE cullMode,
        BOOL depthClipEnable,
        ComPtr<ID3D11RasterizerState>& outRasterizerState
    );

    // サンプラーステートを作成
    bool CreateSamplerState(
        const ComPtr<ID3D11Device>& device,
        D3D11_FILTER filter,
        D3D11_TEXTURE_ADDRESS_MODE addressU,
        D3D11_TEXTURE_ADDRESS_MODE addressV,
        D3D11_TEXTURE_ADDRESS_MODE addressW,
        ComPtr<ID3D11SamplerState>& outSamplerState
    );

    // 頂点シェーダーと入力レイアウトを作成
    bool CreateVertexShaderAndInputLayout(
        const ComPtr<ID3D11Device>& device,
        const std::string& fileName,
        const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs,
        UINT inputElementCount,
        ComPtr<ID3D11VertexShader>& outVertexShader,
        ComPtr<ID3D11InputLayout>& outInputLayout
    );

    // 頂点シェーダーと入力レイアウトを作成
    bool CreateVertexShaderAndInputLayout(
        const ComPtr<ID3D11Device>& device,
        const ComPtr<ID3DBlob>& byteCode,
        const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs,
        UINT inputElementCount,
        ComPtr<ID3D11VertexShader>& outVertexShader,
        ComPtr<ID3D11InputLayout>& outInputLayout
    );

    // ピクセルシェーダーを作成
    bool CreatePixelShader(
        const ComPtr<ID3D11Device>& device,
        const std::string& fileName,
        ComPtr<ID3D11PixelShader>& outPixelShader
    );

    // ピクセルシェーダーを作成
    bool CreatePixelShader(
        const ComPtr<ID3D11Device>& device,
        const ComPtr<ID3DBlob>& byteCode,
        ComPtr<ID3D11PixelShader>& outPixelShader
    );

    // ジオメトリシェーダーを作成
    bool CreateGeometryShader(
        const ComPtr<ID3D11Device>& device,
        const std::string& fileName,
        ComPtr<ID3D11GeometryShader>& outGeometryShader
    );


    // ブレンドステートを作成
    bool CreateBlendState(
        const ComPtr<ID3D11Device>& device,
        ComPtr<ID3D11BlendState>& outBlendState
    );


    // シェーダーをコンパイル
    bool CompileShader(
        const char* src,
        size_t srcSize,
        const char* shaderModel,
        ComPtr<ID3DBlob>& outCode
    );
















//inline u32 ColorPack(const glm::vec4& color)
//{
//    glm::vec4 saturatedColor = glm::saturation(1.0f, color);
//    return (u32)(saturatedColor.a * 255) << 24 |
//        (u32)(saturatedColor.b * 255) << 16 |
//        (u32)(saturatedColor.g * 255) << 8 |
//        (u32)(saturatedColor.r * 255);
//}


} // namespace DX11Util

