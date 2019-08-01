#pragma once


#include "DX11/DX11Prerequisites.hpp"
#include "DX11/External/glm/glm_Prerequisites.h"
#include "FontData.hpp"





struct Vertex_BitmapFont
{
    glm::vec2   Position;
    glm::u32    Color;
    glm::vec2   Texture;
    glm::u32    Channel;
    glm::u16    Page;

    //static constexpr UINT PositionOffset = 0;
    //static constexpr UINT PositionSize = sizeof(glm::vec2);

    //static constexpr UINT ColorOffset = PositionOffset + PositionSize;
    //static constexpr UINT ColorSize = sizeof(glm::u32);

    //static constexpr UINT TextureOffset = ColorOffset + ColorSize;
    //static constexpr UINT TextureSize = sizeof(glm::vec2);

    //static constexpr UINT ChannelOffset = TextureOffset + TextureSize;
    //static constexpr UINT ChannelSize = sizeof(glm::u16);

    //static constexpr UINT PageOffset = ChannelOffset + ChannelSize;
    //static constexpr UINT PageSize = sizeof(glm::u16);

    static constexpr D3D11_INPUT_ELEMENT_DESC pInputElementDescs[] = {
        { "POSITION", 0,    DXGI_FORMAT_R32G32_FLOAT,   0,  0,                              D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",    0,    DXGI_FORMAT_R8G8B8A8_UNORM, 0,  D3D11_APPEND_ALIGNED_ELEMENT,   D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0,    DXGI_FORMAT_R32G32_FLOAT,   0,  D3D11_APPEND_ALIGNED_ELEMENT,   D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "CHANNEL",  0,    DXGI_FORMAT_R8G8B8A8_UNORM, 0,  D3D11_APPEND_ALIGNED_ELEMENT,   D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "PAGE",     0,    DXGI_FORMAT_R16_UINT,       0,  D3D11_APPEND_ALIGNED_ELEMENT,   D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    static constexpr UINT InputElementCount = _countof(pInputElementDescs);
};


class Sampler;
class BlendState;
class ConstantBuffer;
class BitmapFont
{
public:
    BitmapFont(
        const ComPtr<ID3D11Device>& device,
        const ComPtr<ID3D11DeviceContext> context,
        const glm::mat4x4& projection
    );

    ~BitmapFont();

    void FileLoad(const std::string& fileName, Font::FontData& out);

    void Initialize(const std::string& fileName);

    glm::vec4 Put(const glm::vec2& position, const char* pUtf8);

    glm::vec4 Put(const glm::vec2& position, const glm::vec4& color, const char* pUtf8);

    glm::vec4 Put(const glm::vec2& position, const wchar_t* pUtf16);

    glm::vec4 Put(const glm::vec2& position, const glm::vec4& color, const wchar_t* pUtf16);

    void Draw();

    void Clear();

    void Flush();

protected:
    void PutVertex(
        const Font::FontCharacter* pChar,
        u16 prevCharId,
        u16 charId
    );

    Font::FontKerning* GetKering(u16 prevCharId, u16 charId);

    glm::u32 GetChannelColor(glm::u8 channel) const;

    ComPtr<ID3D11Device>                m_Device;               // デバイス
    ComPtr<ID3D11DeviceContext>         m_Context;              // デバイスコンテキスト
    ComPtr<ID3D11ShaderResourceView>    m_ShaderResourceView;   // シェーダーリソース
    ComPtr<ID3D11Texture2D>             m_Texture2D;            // テクスチャ
    ComPtr<ID3D11Buffer>                m_VertexBuffer;         // 頂点バッファ
    ComPtr<ID3D11Buffer>                m_IndexBuffer;          // インデックスバッファ
    std::unique_ptr<Vertex_BitmapFont>  m_VertexStream;         // CPU側頂点バッファ      m_BufferSize * 4 頂点
    ComPtr<ID3D11VertexShader>          m_VertexShader;         // 頂点シェーダー
    ComPtr<ID3D11PixelShader>           m_PixelShader;          // ピクセルシェーダー
    ComPtr<ID3D11InputLayout>           m_InputLayout;          // 入力レイアウト
    ComPtr<ID3D11RasterizerState>       m_RasterizerState;      // ラスタライザーステート

    std::string                         m_ShaderCode;

    std::unique_ptr<Sampler>            m_Sampler;
    std::unique_ptr<BlendState>         m_BlendState;
    std::unique_ptr<ConstantBuffer>     m_ConstantBuffer;

    Size2D          m_TextureSize;
    Font::FontData  m_Data;
    glm::f32        m_FontScale;
    bool            m_UpdateBufferRq;   // バッファー更新リクエスト
    glm::u32        m_BufferSize;       // 描画最大文字数
    glm::u32        m_Count;            // 表示文字数
    glm::f32        m_CharSpacing;      // 横文字間
    glm::f32        m_LineSpacing;      // 縦文字間
    glm::vec4       m_RectDrawArea;     // 描画領域
    glm::vec2       m_Location;         // 現在文字位置
    glm::f32        m_FontHeight;       // フォントの縦描画サイズ
    glm::vec2       m_SizePerPix;       // 1ピクセルあたりのUV値
    glm::vec4       m_Color;            // 描画乗算色(全体)
    glm::vec4       m_CurrentColor;     // 現在の文字色
    glm::mat4x4     m_Projection;       // プロジェクション行列
    glm::vec4       m_RectMargin;       // 矩形マージン
};


