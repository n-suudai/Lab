#include "DX11Util.hpp"
#include "../External/lodepng/lodepng.h"
#include <d3dcompiler.h>


namespace DX11Util
{

    // DXGI ピクセルフォーマットからビット数を返す
    UINT DXGIFormatBitsPerPixel(DXGI_FORMAT format)
    {
        switch (static_cast<int>(format))
        {
        case DXGI_FORMAT_R32G32B32A32_TYPELESS:
        case DXGI_FORMAT_R32G32B32A32_FLOAT:
        case DXGI_FORMAT_R32G32B32A32_UINT:
        case DXGI_FORMAT_R32G32B32A32_SINT:
            return 128;

        case DXGI_FORMAT_R32G32B32_TYPELESS:
        case DXGI_FORMAT_R32G32B32_FLOAT:
        case DXGI_FORMAT_R32G32B32_UINT:
        case DXGI_FORMAT_R32G32B32_SINT:
            return 96;

        case DXGI_FORMAT_R16G16B16A16_TYPELESS:
        case DXGI_FORMAT_R16G16B16A16_FLOAT:
        case DXGI_FORMAT_R16G16B16A16_UNORM:
        case DXGI_FORMAT_R16G16B16A16_UINT:
        case DXGI_FORMAT_R16G16B16A16_SNORM:
        case DXGI_FORMAT_R16G16B16A16_SINT:
        case DXGI_FORMAT_R32G32_TYPELESS:
        case DXGI_FORMAT_R32G32_FLOAT:
        case DXGI_FORMAT_R32G32_UINT:
        case DXGI_FORMAT_R32G32_SINT:
        case DXGI_FORMAT_R32G8X24_TYPELESS:
        case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
        case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
        case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
        case DXGI_FORMAT_Y416:
        case DXGI_FORMAT_Y210:
        case DXGI_FORMAT_Y216:
            return 64;

        case DXGI_FORMAT_R10G10B10A2_TYPELESS:
        case DXGI_FORMAT_R10G10B10A2_UNORM:
        case DXGI_FORMAT_R10G10B10A2_UINT:
        case DXGI_FORMAT_R11G11B10_FLOAT:
        case DXGI_FORMAT_R8G8B8A8_TYPELESS:
        case DXGI_FORMAT_R8G8B8A8_UNORM:
        case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
        case DXGI_FORMAT_R8G8B8A8_UINT:
        case DXGI_FORMAT_R8G8B8A8_SNORM:
        case DXGI_FORMAT_R8G8B8A8_SINT:
        case DXGI_FORMAT_R16G16_TYPELESS:
        case DXGI_FORMAT_R16G16_FLOAT:
        case DXGI_FORMAT_R16G16_UNORM:
        case DXGI_FORMAT_R16G16_UINT:
        case DXGI_FORMAT_R16G16_SNORM:
        case DXGI_FORMAT_R16G16_SINT:
        case DXGI_FORMAT_R32_TYPELESS:
        case DXGI_FORMAT_D32_FLOAT:
        case DXGI_FORMAT_R32_FLOAT:
        case DXGI_FORMAT_R32_UINT:
        case DXGI_FORMAT_R32_SINT:
        case DXGI_FORMAT_R24G8_TYPELESS:
        case DXGI_FORMAT_D24_UNORM_S8_UINT:
        case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
        case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
        case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
        case DXGI_FORMAT_R8G8_B8G8_UNORM:
        case DXGI_FORMAT_G8R8_G8B8_UNORM:
        case DXGI_FORMAT_B8G8R8A8_UNORM:
        case DXGI_FORMAT_B8G8R8X8_UNORM:
        case DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:
        case DXGI_FORMAT_B8G8R8A8_TYPELESS:
        case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
        case DXGI_FORMAT_B8G8R8X8_TYPELESS:
        case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
        case DXGI_FORMAT_AYUV:
        case DXGI_FORMAT_Y410:
        case DXGI_FORMAT_YUY2:
            return 32;

        case DXGI_FORMAT_P010:
        case DXGI_FORMAT_P016:
            return 24;

        case DXGI_FORMAT_R8G8_TYPELESS:
        case DXGI_FORMAT_R8G8_UNORM:
        case DXGI_FORMAT_R8G8_UINT:
        case DXGI_FORMAT_R8G8_SNORM:
        case DXGI_FORMAT_R8G8_SINT:
        case DXGI_FORMAT_R16_TYPELESS:
        case DXGI_FORMAT_R16_FLOAT:
        case DXGI_FORMAT_D16_UNORM:
        case DXGI_FORMAT_R16_UNORM:
        case DXGI_FORMAT_R16_UINT:
        case DXGI_FORMAT_R16_SNORM:
        case DXGI_FORMAT_R16_SINT:
        case DXGI_FORMAT_B5G6R5_UNORM:
        case DXGI_FORMAT_B5G5R5A1_UNORM:
        case DXGI_FORMAT_A8P8:
        case DXGI_FORMAT_B4G4R4A4_UNORM:
            return 16;

        case DXGI_FORMAT_NV12:
        case DXGI_FORMAT_420_OPAQUE:
        case DXGI_FORMAT_NV11:
            return 12;

        case DXGI_FORMAT_R8_TYPELESS:
        case DXGI_FORMAT_R8_UNORM:
        case DXGI_FORMAT_R8_UINT:
        case DXGI_FORMAT_R8_SNORM:
        case DXGI_FORMAT_R8_SINT:
        case DXGI_FORMAT_A8_UNORM:
        case DXGI_FORMAT_AI44:
        case DXGI_FORMAT_IA44:
        case DXGI_FORMAT_P8:
            return 8;

        case DXGI_FORMAT_R1_UNORM:
            return 1;

        case DXGI_FORMAT_BC1_TYPELESS:
        case DXGI_FORMAT_BC1_UNORM:
        case DXGI_FORMAT_BC1_UNORM_SRGB:
        case DXGI_FORMAT_BC4_TYPELESS:
        case DXGI_FORMAT_BC4_UNORM:
        case DXGI_FORMAT_BC4_SNORM:
            return 4;

        case DXGI_FORMAT_BC2_TYPELESS:
        case DXGI_FORMAT_BC2_UNORM:
        case DXGI_FORMAT_BC2_UNORM_SRGB:
        case DXGI_FORMAT_BC3_TYPELESS:
        case DXGI_FORMAT_BC3_UNORM:
        case DXGI_FORMAT_BC3_UNORM_SRGB:
        case DXGI_FORMAT_BC5_TYPELESS:
        case DXGI_FORMAT_BC5_UNORM:
        case DXGI_FORMAT_BC5_SNORM:
        case DXGI_FORMAT_BC6H_TYPELESS:
        case DXGI_FORMAT_BC6H_UF16:
        case DXGI_FORMAT_BC6H_SF16:
        case DXGI_FORMAT_BC7_TYPELESS:
        case DXGI_FORMAT_BC7_UNORM:
        case DXGI_FORMAT_BC7_UNORM_SRGB:
            return 8;

        default:
            return 0;
        }
    }


    // DXGI ピクセルフォーマットからSysMemPitchを返す
    UINT DXGIFormatSystemPitch(DXGI_FORMAT format, int iWidth)
    {
        switch (format)
        {
        case DXGI_FORMAT_BC1_TYPELESS:
        case DXGI_FORMAT_BC1_UNORM:
        case DXGI_FORMAT_BC1_UNORM_SRGB:
        case DXGI_FORMAT_BC4_TYPELESS:
        case DXGI_FORMAT_BC4_UNORM:
        case DXGI_FORMAT_BC4_SNORM:
        case DXGI_FORMAT_BC2_TYPELESS:
        case DXGI_FORMAT_BC2_UNORM:
        case DXGI_FORMAT_BC2_UNORM_SRGB:
        case DXGI_FORMAT_BC3_TYPELESS:
        case DXGI_FORMAT_BC3_UNORM:
        case DXGI_FORMAT_BC3_UNORM_SRGB:
        case DXGI_FORMAT_BC5_TYPELESS:
        case DXGI_FORMAT_BC5_UNORM:
        case DXGI_FORMAT_BC5_SNORM:
        case DXGI_FORMAT_BC6H_TYPELESS:
        case DXGI_FORMAT_BC6H_UF16:
        case DXGI_FORMAT_BC6H_SF16:
        case DXGI_FORMAT_BC7_TYPELESS:
        case DXGI_FORMAT_BC7_UNORM:
        case DXGI_FORMAT_BC7_UNORM_SRGB:
            return static_cast<UINT>(DXGIFormatBitsPerPixel(format) * 2 * (iWidth / 4));
        default:
            return static_cast<UINT>(DXGIFormatBitsPerPixel(format) / 8 * iWidth);
        }
    }


    // PNG 読み込み
    bool LoadPng(const std::string& fileName, std::vector<BYTE>& image, DXGI_FORMAT& format, Size2D& size)
    {
        ResultUtil result;

        std::vector<BYTE> png;

        result = ReadFile(fileName, png);
        if (!result)
        {
            result.ShowMessageBox("!file.fail()");
            return false;
        }

        LodePNGColorType cType = (LodePNGColorType)png[25];
        LodePNGColorType cOutType = LCT_RGBA;

        format = DXGI_FORMAT_R8G8B8A8_UNORM;

        switch (cType)
        {
        case LCT_GREY:
            format = DXGI_FORMAT_R8_UNORM;
            cOutType = cType;
            break;
        case LCT_GREY_ALPHA:
            format = DXGI_FORMAT_A8_UNORM;
            cOutType = cType;
            break;
        case LCT_RGB:
            format = DXGI_FORMAT_R8G8B8A8_UNORM;
            cOutType = LCT_RGBA;
            break;
        default:
            break;
        }

        unsigned int error = lodepng::decode(image, size.width, size.height, png, cOutType);
        result = ResultUtil(error == 0, lodepng_error_text(error));
        if (!result)
        {
            result.ShowMessageBox("lodepng::decode");
            return false;
        }

        return true;
    }


    // ファイルの読み込み（バイナリ）
    bool ReadFile(const std::string& fileName, std::vector<BYTE>& out)
    {
        out.clear();

        std::ifstream file(fileName, std::ios::binary);


        ResultUtil result = ResultUtil(!file.fail(), "open " + fileName + " is failed.");

        if (!result)
        {
            result.ShowMessageBox("!file.fail()");
            return false;
        }

        file.unsetf(std::ios::skipws);

        std::streampos fileSize;

        file.seekg(0, std::ios::end);
        fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        out.reserve(fileSize);

        out.insert(
            out.begin(),
            std::istream_iterator<BYTE>(file),
            std::istream_iterator<BYTE>()
        );

        return true;
    }


    // バイナリデータからテクスチャを作成
    bool CreateTextureFromData(
        const ComPtr<ID3D11Device>& device,
        DXGI_FORMAT format,
        const Size2D& size,
        const u8* image,
        u32 imageSize,
        ComPtr<ID3D11Texture2D>& outTexture2D,
        ComPtr<ID3D11ShaderResourceView>& outShaderResourceView,
        D3D11_USAGE usage,
        UINT cpuAccessFlag
    )
    {
        const void* pImage = image;
        CD3D11_TEXTURE2D_DESC texture2DDesc(
            format,
            static_cast<UINT>(size.width),
            static_cast<UINT>(size.height),
            1,
            1,
            D3D11_BIND_SHADER_RESOURCE,
            usage,
            cpuAccessFlag
        );

        D3D11_SUBRESOURCE_DATA subresourceData;
        ZeroMemory(&subresourceData, sizeof(subresourceData));

        subresourceData.pSysMem = pImage;
        subresourceData.SysMemPitch = DXGIFormatSystemPitch(format, static_cast<UINT>(size.width));
        subresourceData.SysMemSlicePitch = static_cast<UINT>(imageSize);

        // テクスチャを生成
        ResultUtil result = device->CreateTexture2D(
            &texture2DDesc,
            &subresourceData,
            &outTexture2D
        );
        if (!result)
        {
            result.ShowMessageBox("device->CreateTexture2D");
            return false;
        }

        // シェーダーリソースビューを生成
        {
            CD3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc(
                D3D11_SRV_DIMENSION_TEXTURE2D,
                format,
                0,
                texture2DDesc.MipLevels
            );

            // シェーダーリソースビューを生成
            result = device->CreateShaderResourceView(
                outTexture2D.Get(),
                &shaderResourceViewDesc,
                &outShaderResourceView
            );
            if (!result)
            {
                result.ShowMessageBox("device->CreateShaderResourceView");
                return false;
            }
        }
        return true;
    }


    // PNG ファイルからテクスチャを生成
    bool CreateTextureFromFile(
        const ComPtr<ID3D11Device>& device,
        const std::string& fileName,
        ComPtr<ID3D11Texture2D>& outTexture2D,
        ComPtr<ID3D11ShaderResourceView>& outShaderResourceView
    )
    {
        std::vector<unsigned char> image;
        DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;
        Size2D size = { 0 };

        // PNG 読み込み
        if (!LoadPng(fileName, image, format, size))
        {
            return false;
        }

        void* pImage = &image[0];
        CD3D11_TEXTURE2D_DESC texture2DDesc(
            format,
            static_cast<UINT>(size.width),
            static_cast<UINT>(size.height),
            1,
            1,
            D3D11_BIND_SHADER_RESOURCE
        );

        D3D11_SUBRESOURCE_DATA subresourceData;
        ZeroMemory(&subresourceData, sizeof(subresourceData));

        subresourceData.pSysMem = pImage;
        subresourceData.SysMemPitch = DXGIFormatSystemPitch(format, static_cast<UINT>(size.width));
        subresourceData.SysMemSlicePitch = static_cast<UINT>(image.size());

        // テクスチャを生成
        ResultUtil result = device->CreateTexture2D(
            &texture2DDesc,
            &subresourceData,
            &outTexture2D
        );
        if (!result)
        {
            result.ShowMessageBox("device->CreateTexture2D");
            return false;
        }

        // シェーダーリソースビューを生成
        {
            CD3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc(
                D3D11_SRV_DIMENSION_TEXTURE2D,
                format,
                0,
                texture2DDesc.MipLevels
            );

            // シェーダーリソースビューを生成
            result = device->CreateShaderResourceView(
                outTexture2D.Get(),
                &shaderResourceViewDesc,
                &outShaderResourceView
            );
            if (!result)
            {
                result.ShowMessageBox("device->CreateShaderResourceView");
                return false;
            }
        }
        return true;
    }


    // 複数の PNG ファイルからテクスチャ配列を作成
    bool CreateTextureArrayFromFile(
        const ComPtr<ID3D11Device>& device,
        std::vector<std::string> fileNames,
        ComPtr<ID3D11Texture2D>& outTexture2D,
        ComPtr<ID3D11ShaderResourceView>& outShaderResourceView,
        Size2D& outTextureSize
    )
    {
        std::vector<std::vector<unsigned char>> images;
        DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;

        for (auto& fileName : fileNames)
        {
            std::vector<unsigned char> image;

            // PNG 読み込み
            if (!LoadPng(fileName, image, format, outTextureSize))
            {
                return false;
            }
            images.push_back(std::move(image));
        }

        CD3D11_TEXTURE2D_DESC texture2DDesc(
            format,
            static_cast<UINT>(outTextureSize.width),
            static_cast<UINT>(outTextureSize.height),
            static_cast<UINT>(images.size()),
            1,
            D3D11_BIND_SHADER_RESOURCE
        );

        std::vector<D3D11_SUBRESOURCE_DATA> subresourceDataArray;

        for (auto& image : images)
        {
            void* pImage = &image[0];
            D3D11_SUBRESOURCE_DATA subresourceData;
            ZeroMemory(&subresourceData, sizeof(subresourceData));

            subresourceData.pSysMem = pImage;
            subresourceData.SysMemPitch = DXGIFormatSystemPitch(format, static_cast<UINT>(outTextureSize.width));
            subresourceData.SysMemSlicePitch = static_cast<UINT>(image.size());

            subresourceDataArray.push_back(subresourceData);
        }

        // テクスチャを生成
        ResultUtil result = device->CreateTexture2D(
            &texture2DDesc,
            &subresourceDataArray[0],
            &outTexture2D
        );
        if (!result)
        {
            result.ShowMessageBox("device->CreateTexture2D");
            return false;
        }

        // シェーダーリソースビューを生成
        {
            CD3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc(
                D3D11_SRV_DIMENSION_TEXTURE2DARRAY,
                format,
                0,
                texture2DDesc.MipLevels
            );

            // シェーダーリソースビューを生成
            result = device->CreateShaderResourceView(
                outTexture2D.Get(),
                &shaderResourceViewDesc,
                &outShaderResourceView
            );
            if (!result)
            {
                result.ShowMessageBox("device->CreateShaderResourceView");
                return false;
            }
        }
        return true;
    }


    // 各種バッファの作成
    bool CreateBuffer(
        const ComPtr<ID3D11Device>& device,
        const void* pInitData,
        UINT byteWidth,
        D3D11_BIND_FLAG bindFlag,
        UINT stride,
        ComPtr<ID3D11Buffer>& outBuffer
    )
    {
        // バッファの設定
        CD3D11_BUFFER_DESC bufferDesc(
            byteWidth,
            bindFlag,
            D3D11_USAGE_DEFAULT,
            0,
            0,
            stride
        );

        // サブリソースの設定
        D3D11_SUBRESOURCE_DATA subresourceData;
        ZeroMemory(&subresourceData, sizeof(subresourceData));

        subresourceData.pSysMem = pInitData;
        subresourceData.SysMemPitch = 0;
        subresourceData.SysMemSlicePitch = 0;

        ResultUtil result = device->CreateBuffer(
            &bufferDesc,
            &subresourceData,
            &outBuffer
        );
        if (!result)
        {
            result.ShowMessageBox("device->CreateBuffer");
            return false;
        }
        return true;
    }


    // ラスタライザーステートを作成
    bool CreateRasterizerState(
        const ComPtr<ID3D11Device>& device,
        D3D11_CULL_MODE cullMode,
        BOOL depthClipEnable,
        ComPtr<ID3D11RasterizerState>& outRasterizerState
    )
    {
        CD3D11_RASTERIZER_DESC rasterizerDesc(D3D11_DEFAULT);
        rasterizerDesc.CullMode = cullMode;
        rasterizerDesc.DepthClipEnable = depthClipEnable;

        ResultUtil result = device->CreateRasterizerState(
            &rasterizerDesc,
            &outRasterizerState
        );
        if (!result)
        {
            result.ShowMessageBox("device->CreateRasterizerState");
            return false;
        }
        return true;
    }


    // サンプラーステートを作成
    bool CreateSamplerState(
        const ComPtr<ID3D11Device>& device,
        D3D11_FILTER filter,
        D3D11_TEXTURE_ADDRESS_MODE addressU,
        D3D11_TEXTURE_ADDRESS_MODE addressV,
        D3D11_TEXTURE_ADDRESS_MODE addressW,
        ComPtr<ID3D11SamplerState>& outSamplerState
    )
    {
        FLOAT borderColor[4] = { 0.0f };
        CD3D11_SAMPLER_DESC samplerDesc(
            filter,       // サンプリング時に使用するフィルタ。ここでは異方性フィルターを使用する(D3D11_FILTER_ANISOTROPIC)
            addressU,     // 0 ～ 1 の範囲外にある u テクスチャー座標の描画方法
            addressV,     // 0 ～ 1 の範囲外にある v テクスチャー座標
            addressW,     // 0 ～ 1 の範囲外にある w テクスチャー座標
            0,                              // 計算されたミップマップ レベルからのバイアス
            16,                             // サンプリングに異方性補間を使用している場合の限界値。有効な値は 1 ～ 16 
            D3D11_COMPARISON_ALWAYS,        // 比較オプション
            borderColor,                    // 境界色
            0,                              // アクセス可能なミップマップの下限値
            D3D11_FLOAT32_MAX               // アクセス可能なミップマップの上限値
        );

        ResultUtil result = device->CreateSamplerState(
            &samplerDesc,
            &outSamplerState
        );
        if (!result)
        {
            result.ShowMessageBox("device->CreateSamplerState");
            return false;
        }
        return true;
    }


    // 頂点シェーダーと入力レイアウトを作成
    bool CreateVertexShaderAndInputLayout(
        const ComPtr<ID3D11Device>& device,
        const std::string& fileName,
        const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs,
        UINT inputElementCount,
        ComPtr<ID3D11VertexShader>& outVertexShader,
        ComPtr<ID3D11InputLayout>& outInputLayout
    )
    {
        std::vector<BYTE> vertexShaderData;

        if (!ReadFile(fileName, vertexShaderData))
        {
            return false;
        }

        ResultUtil result = device->CreateVertexShader(
            vertexShaderData.data(),
            vertexShaderData.size(),
            nullptr,
            &outVertexShader
        );
        if (!result)
        {
            result.ShowMessageBox("device->CreateVertexShader");
            return false;
        }

        // InputLayout
        {
            result = device->CreateInputLayout(
                pInputElementDescs,
                inputElementCount,
                vertexShaderData.data(),
                vertexShaderData.size(),
                &outInputLayout
            );
            if (!result)
            {
                result.ShowMessageBox("device->CreateInputLayout");
                return false;
            }
        }

        return true;
    }


    // 頂点シェーダーと入力レイアウトを作成
    bool CreateVertexShaderAndInputLayout(
        const ComPtr<ID3D11Device>& device,
        const ComPtr<ID3DBlob>& byteCode,
        const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs,
        UINT inputElementCount,
        ComPtr<ID3D11VertexShader>& outVertexShader,
        ComPtr<ID3D11InputLayout>& outInputLayout
    )
    {
        ResultUtil result = device->CreateVertexShader(
            byteCode->GetBufferPointer(),
            byteCode->GetBufferSize(),
            nullptr,
            &outVertexShader
        );
        if (!result)
        {
            result.ShowMessageBox("device->CreateVertexShader");
            return false;
        }

        // InputLayout
        {
            result = device->CreateInputLayout(
                pInputElementDescs,
                inputElementCount,
                byteCode->GetBufferPointer(),
                byteCode->GetBufferSize(),
                &outInputLayout
            );
            if (!result)
            {
                result.ShowMessageBox("device->CreateInputLayout");
                return false;
            }
        }

        return true;
    }


    // ピクセルシェーダーを作成
    bool CreatePixelShader(
        const ComPtr<ID3D11Device>& device,
        const std::string& fileName,
        ComPtr<ID3D11PixelShader>& outPixelShader
    )
    {
        std::vector<BYTE> pixelShaderData;

        if (!ReadFile(fileName, pixelShaderData))
        {
            return false;
        }

        ResultUtil result = device->CreatePixelShader(
            pixelShaderData.data(),
            pixelShaderData.size(),
            nullptr,
            &outPixelShader
        );
        if (!result)
        {
            result.ShowMessageBox("device->CreatePixelShader");
            return false;
        }

        return true;
    }


    // ピクセルシェーダーを作成
    bool CreatePixelShader(
        const ComPtr<ID3D11Device>& device,
        const ComPtr<ID3DBlob>& byteCode,
        ComPtr<ID3D11PixelShader>& outPixelShader
    )
    {
        ResultUtil result = device->CreatePixelShader(
            byteCode->GetBufferPointer(),
            byteCode->GetBufferSize(),
            nullptr,
            &outPixelShader
        );
        if (!result)
        {
            result.ShowMessageBox("device->CreatePixelShader");
            return false;
        }

        return true;
    }


    // ジオメトリシェーダーを作成
    bool CreateGeometryShader(
        const ComPtr<ID3D11Device>& device,
        const std::string& fileName,
        ComPtr<ID3D11GeometryShader>& outGeometryShader
    )
    {
        std::vector<BYTE> geometryShaderData;

        if (!ReadFile(fileName, geometryShaderData))
        {
            return false;
        }

        ResultUtil result = device->CreateGeometryShader(
            geometryShaderData.data(),
            geometryShaderData.size(),
            nullptr,
            &outGeometryShader
        );
        if (!result)
        {
            result.ShowMessageBox("device->CreatePixelShader");
            return false;
        }

        return true;
    }


    // ブレンドステートを作成
    bool CreateBlendState(
        const ComPtr<ID3D11Device>& device,
        ComPtr<ID3D11BlendState>& outBlendState
    )
    {
        // アルファブレンド 加算
        CD3D11_BLEND_DESC blendDesc(D3D11_DEFAULT);
        blendDesc.RenderTarget[0].BlendEnable = TRUE;
        blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
        blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
        blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
        blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
        blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
        blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

        ResultUtil result = device->CreateBlendState(
            &blendDesc,
            &outBlendState
        );
        if (!result)
        {
            result.ShowMessageBox("device->CreateBlendState");
            return false;
        }
        return true;
    }


    // シェーダーをコンパイル
    bool CompileShader(
        const char* src,
        size_t srcSize,
        const char* shaderModel,
        const char* entryPoint,
        ComPtr<ID3DBlob>& outCode
    )
    {
        ComPtr<ID3DBlob> errorMessage;
        UINT shaderFlag = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined(DEBUG) || defined(_DEBUG)
        shaderFlag |= D3DCOMPILE_DEBUG;
#endif

        ResultUtil result =  D3DCompile2(
            reinterpret_cast<LPCVOID>(src),
            srcSize,
            "",
            nullptr,
            nullptr,
            entryPoint,
            shaderModel,
            shaderFlag,
            0,
            0,
            nullptr,
            0,
            outCode.GetAddressOf(),
            errorMessage.GetAddressOf()
        );

        if (!result)
        {
            if (errorMessage)
            {
                result.ShowMessageBox(
                    (char*)errorMessage->GetBufferPointer()
                );
            }
            else
            {
                result.ShowMessageBox(
                    "ShaderCompile Failed."
                );
            }

            return false;
        }

        return true;
    }
}

