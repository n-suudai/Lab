#include "BitmapFont.h"
#include "DX11/Utils/DX11Util.h"
#include "DX11/Utils/StringUtil.h"
#include "DX11/Utils/MathUtil.h"
#include "DX11/Graphics/Sampler.h"
#include "DX11/Graphics/BlendState.h"
#include "DX11/Graphics/BufferResource.h"



BitmapFont::BitmapFont(
    const ComPtr<ID3D11Device>& device,
    const ComPtr<ID3D11DeviceContext> context,
    const glm::mat4x4& projection
)
    : m_Device(device)
    , m_Context(context)
    , m_Projection(projection)
    , m_ShaderCode("")
{
    m_Count = 0;
    m_CharSpacing = 0.0f;
    m_LineSpacing = 0;
    m_FontScale = 1.0f;
    m_RectMargin = glm::vec4(-2.0f, -2.0f, 2.0f, 2.0f);

    StringUtil::InitLocale();
}

BitmapFont::~BitmapFont()
{

}

void BitmapFont::FileLoad(const std::string& fontName, Font::FontData& out)
{
    std::string fileName = "Assets\\Font\\" + fontName + "\\" + fontName + ".fnt";

    LoadFont(fileName, out);
}

void BitmapFont::Init(const std::string& fontName)
{
    // ファイル読み込み
    FileLoad(fontName, m_Data);
    
    // テクスチャ作成
    {
        std::vector<std::string> files;
        for (auto& page : m_Data.pages)
        {
            files.push_back("Assets\\Font\\" + fontName + "\\" + page.file);
        }

        // テクスチャ配列として生成
        DX11Util::CreateTextureArrayFromFile(
            m_Device,
            files,
            m_Texture2D,
            m_ShaderResourceView,
            m_TextureSize
        );
    }

    // 頂点バッファ作成
    {
        m_BufferSize = 4096;
        m_VertexStream.reset(new Vertex_BitmapFont[m_BufferSize * 4]);

        ZeroMemory(m_VertexStream.get(), sizeof(Vertex_BitmapFont) * m_BufferSize * 4);

        DX11Util::CreateBuffer(
            m_Device,
            m_VertexStream.get(),
            sizeof(Vertex_BitmapFont) * m_BufferSize * 4,
            D3D11_BIND_VERTEX_BUFFER,
            sizeof(Vertex_BitmapFont),
            m_VertexBuffer
        );
    }

    // インデックスバッファ作成
    {
        std::unique_ptr<u16> indexStream(new u16[m_BufferSize * 6]);
        u16* pIndex = indexStream.get();
        for (u16 ii = 0; ii < m_BufferSize; ++ii)
        {
            pIndex[ii * 6 + 0] = ii * 4 + 0;
            pIndex[ii * 6 + 1] = ii * 4 + 1;
            pIndex[ii * 6 + 2] = ii * 4 + 2;
            pIndex[ii * 6 + 3] = ii * 4 + 1;
            pIndex[ii * 6 + 4] = ii * 4 + 3;
            pIndex[ii * 6 + 5] = ii * 4 + 2;
        }

        DX11Util::CreateBuffer(
            m_Device,
            pIndex,
            sizeof(u16) * m_BufferSize * 6,
            D3D11_BIND_INDEX_BUFFER,
            sizeof(u16),
            m_IndexBuffer
        );
    }

    // ラスタライザーステート作成
    {
        DX11Util::CreateRasterizerState(
            m_Device,
            D3D11_CULL_NONE,
            FALSE,
            m_RasterizerState
        );
    }

    m_ShaderCode = R"(
Texture2DArray<float4> diffuseTextures : register(t0);
SamplerState linearSampler : register(s0);

cbuffer CB0 : register(b0)
{
    row_major float4x4 Projection;
};

struct VS_INPUT
{
    float2 position : POSITION;
    float4 color    : COLOR;
    float2 tex      : TEXCOORD;
    float4 channel  : CHANNEL;
    uint   page     : PAGE;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float4 color    : COLOR;
    float2 tex      : TEXCOORD;
    float4 channel  : CHANNEL;
    uint   page     : PAGE;
};

VS_OUTPUT vs_main(VS_INPUT In)
{
    VS_OUTPUT Out = (VS_OUTPUT)0;

    Out.position = mul(float4(In.position.xy, 0.0, 1.0), Projection);
    Out.color = In.color;
    Out.tex = In.tex;
    Out.page = In.page;
    Out.channel = In.channel;

    return Out;
}

float4 ps_main(VS_OUTPUT In) : SV_TARGET
{
    // distance field texture
    //const float smoothing = 1.0 / 16.0;
    //float3 tex = float3(In.tex, In.page);
    //float distance = diffuseTextures.Sample(linearSampler, tex).a;
    //float alpha = smoothstep(0.5 - smoothing, 0.5 + smoothing, distance);
    //float4 color = float4(In.color.rgb, In.color.a * alpha);
    //return color;

    // simple texture
    float3 tex = float3(In.tex, In.page);
    float4 texColor = diffuseTextures.Sample(linearSampler, tex);

    if (dot(float4(1.0, 1.0, 1.0, 1.0), In.channel))
    {
        float value = dot(texColor, In.channel);

        texColor.rgb = value > 0.5 ? 2.0 * value - 1.0 : 0.0;
        texColor.a = value > 0.5 ? 2.0 * value - 1.0 : 0.0;
    }

    return In.color * texColor;
}
)";

    {
        ComPtr<ID3DBlob> code;
        bool result = DX11Util::CompileShader(
            m_ShaderCode.data(),
            m_ShaderCode.size(),
            "vs_5_0",
            "vs_main",
            code
        );

        if (result)
        {
            DX11Util::CreateVertexShaderAndInputLayout(
                m_Device,
                code,
                Vertex_BitmapFont::pInputElementDescs,
                Vertex_BitmapFont::InputElementCount,
                m_VertexShader,
                m_InputLayout
            );
        }
    }

    {
        ComPtr<ID3DBlob> code;
        bool result = DX11Util::CompileShader(
            m_ShaderCode.data(),
            m_ShaderCode.size(),
            "ps_5_0",
            "ps_main",
            code
        );

        if (result)
        {
            DX11Util::CreatePixelShader(
                m_Device,
                code,
                m_PixelShader
            );
        }
    }

    m_ConstantBuffer = std::make_unique<ConstantBuffer>(
        m_Device,
        m_Context,
        &m_Projection,
        sizeof(m_Projection)
        );

    m_Sampler = std::make_unique<Sampler>(
        m_Device,
        m_Context
        );

    m_BlendState = std::make_unique<BlendState>(
        m_Device,
        m_Context
        );

    m_SizePerPix = glm::vec2(1.0f / static_cast<f32>(m_TextureSize.width), 1.0f / static_cast<f32>(m_TextureSize.height));

    // フォント描画縦サイズを取得
    m_FontHeight = m_Data.chars[static_cast<int>('A')].height + 2.0f;

    m_Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    m_CurrentColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

glm::vec4 BitmapFont::Put(const glm::vec2& position, const char* pUtf8)
{
    std::wstring widen = StringUtil::Widen(pUtf8);
    return Put(position, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), widen.c_str());
}

glm::vec4 BitmapFont::Put(const glm::vec2& position, const glm::vec4& color, const char* pUtf8)
{
    std::wstring widen = StringUtil::Widen(pUtf8);
    return Put(position, color, widen.c_str());
}

glm::vec4 BitmapFont::Put(const glm::vec2& position, const wchar_t* pUtf16)
{
    return Put(position, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), pUtf16);
}

glm::vec4 BitmapFont::Put(const glm::vec2& position, const glm::vec4& color, const wchar_t* pUtf16)
{
    glm::vec4 rect(position.x, position.y, position.x, position.y);
    m_Location = position;
    m_CurrentColor = color;

    u32 iCharaCount = 0;
    wchar_t utf16Code = *pUtf16;
    wchar_t utf16CodePrev = *pUtf16;
    while (utf16Code != 0x0000)
    {
        const Font::FontCharacter* pChar = &m_Data.chars[utf16Code];
        switch (utf16Code)
        {
        default:// 通常文字
            if (utf16Code != 0xFFFF)
            {
                PutVertex(pChar, static_cast<u16>(utf16CodePrev), static_cast<u16>(utf16Code));
                m_Location.x += (pChar->xadvance * m_FontScale) + m_CharSpacing;
                if (rect[2] < m_Location.x) rect[2] = m_Location.x;
            }
            break;
        case '\n':// 改行
            m_Location.x = position.x;
            m_Location.y += (m_FontHeight * m_FontScale) + m_LineSpacing;
            if (rect[3] < m_Location.y) rect[3] = m_Location.y;
            break;
        }

        iCharaCount++;
        if (m_Count >= m_BufferSize)
        {
            break;// バッファーサイズオーバー
        }
        utf16CodePrev = utf16Code;
        utf16Code = pUtf16[iCharaCount];
    }
    f32 bottom = m_Location.y + (m_FontHeight * m_FontScale) + m_LineSpacing;
    if (rect[3] < bottom) rect[3] = bottom;
    rect += m_RectMargin;
    rect[2] -= rect[0];
    rect[3] -= rect[1];
    m_UpdateBufferRq = true;
    return rect;
}

void BitmapFont::Draw()
{
    // 頂点バッファの更新
    if (m_UpdateBufferRq)
    {
        m_Context->UpdateSubresource(m_VertexBuffer.Get(), 0, nullptr, m_VertexStream.get(), 0, 0);

        m_UpdateBufferRq = false;
    }

    // 入力レイアウトを設定
    m_Context->IASetInputLayout(m_InputLayout.Get());

    // 頂点バッファを設定
    {
        constexpr UINT stride = sizeof(Vertex_BitmapFont);
        UINT offset = 0;

        ID3D11Buffer* pVertexBuffers[] = {
            m_VertexBuffer.Get()
        };
        m_Context->IASetVertexBuffers(
            0,
            _countof(pVertexBuffers),
            pVertexBuffers,
            &stride,
            &offset
        );
    }

    // インデックスバッファを設定
    m_Context->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

    // プリミティブトポロジーの設定
    m_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // 頂点シェーダーの設定
    m_Context->VSSetShader(m_VertexShader.Get(), nullptr, 0);

    // 頂点シェーダーに定数バッファを設定
    m_ConstantBuffer->SetVS();

    // ピクセルシェーダーを設定
    m_Context->PSSetShader(m_PixelShader.Get(), nullptr, 0);

    // ピクセルシェーダーにサンプラーステートを設定
    m_Sampler->Set();

    // ピクセルシェーダーにシェーダーリソースビューを設定
    {
        ID3D11ShaderResourceView* pShaderResourceViews[] = {
            m_ShaderResourceView.Get()
        };
        m_Context->PSSetShaderResources(0, _countof(pShaderResourceViews), pShaderResourceViews);
    }

    // ラスタライザーステートを設定
    m_Context->RSSetState(m_RasterizerState.Get());

    // ブレンドステートを設定
    m_BlendState->Set();

    m_Context->DrawIndexed(static_cast<UINT>(m_Count) * 6, 0, 0);
}

void BitmapFont::Clear()
{
    m_Count = 0;
}

void BitmapFont::Flush()
{
    Draw();
    m_Count = 0;
    m_UpdateBufferRq = true;
}

void BitmapFont::PutVertex(
    const Font::FontCharacter* pChar,
    u16 prevCharId,
    u16 charId
)
{
    if (m_Count >= m_BufferSize) { return; }

    Vertex_BitmapFont * pVtx = m_VertexStream.get() + m_Count * 4;

    glm::f32 x = m_Location.x + (pChar->xoffset * m_FontScale);
    glm::f32 y = m_Location.y + (pChar->yoffset * m_FontScale);

    Font::FontKerning* pKerning = GetKering(prevCharId, charId);

    if (pKerning != nullptr)
    {
        x += pKerning->amount * m_FontScale;
    }

    //	-1.0f, 1.0f,0.0f,0.0f,// 左上
    //	-1.0f,-1.0f,0.0f,1.0f,// 左下
    //	 1.0f, 1.0f,1.0f,0.0f,// 右上
    //	 1.0f,-1.0f,1.0f,1.0f,// 右下
    // 	pVertex
    //  [0]左上
    //  [1]右上
    //	[2]左下
    //	[3]右下
    pVtx[0].Channel = GetChannelColor(pChar->chnl);
    pVtx[1].Channel = pVtx[0].Channel;
    pVtx[2].Channel = pVtx[0].Channel;
    pVtx[3].Channel = pVtx[0].Channel;
    pVtx[0].Position.x = x;
    pVtx[1].Position.x = x + (pChar->width * m_FontScale);
    pVtx[2].Position.x = x;
    pVtx[3].Position.x = x + (pChar->width * m_FontScale);
    pVtx[0].Position.y = y;
    pVtx[1].Position.y = y;
    pVtx[2].Position.y = y + (pChar->height * m_FontScale);
    pVtx[3].Position.y = y + (pChar->height * m_FontScale);
    pVtx[0].Texture.x = m_SizePerPix.x * pChar->x;
    pVtx[1].Texture.x = m_SizePerPix.x * (pChar->x + pChar->width);
    pVtx[2].Texture.x = pVtx[0].Texture.x;
    pVtx[3].Texture.x = pVtx[1].Texture.x;
    pVtx[0].Texture.y = m_SizePerPix.y * pChar->y;
    pVtx[1].Texture.y = pVtx[0].Texture.y;
    pVtx[2].Texture.y = m_SizePerPix.y * (pChar->y + pChar->height);
    pVtx[3].Texture.y = pVtx[2].Texture.y;
    pVtx[0].Color = MathUtil::ColorPack(m_CurrentColor);
    pVtx[1].Color = pVtx[0].Color;
    pVtx[2].Color = pVtx[0].Color;
    pVtx[3].Color = pVtx[0].Color;
    pVtx[0].Page = pChar->page;
    pVtx[1].Page = pVtx[0].Page;
    pVtx[2].Page = pVtx[0].Page;
    pVtx[3].Page = pVtx[0].Page;

    m_Count++;
}

Font::FontKerning* BitmapFont::GetKering(u16 prevCharId, u16 charId)
{
    if (prevCharId == charId) { return nullptr; }

    Font::FontKerning* pKerning = nullptr;

    for (auto& kerning : m_Data.kernings)
    {
        if (kerning.first == prevCharId && kerning.second == charId)
        {
            pKerning = &kerning;
            break;
        }
    }

    return pKerning;
}

u32 BitmapFont::GetChannelColor(u8 channel) const
{
    u32 color =
        (channel & Font::Alpha ? 255 : 0) << 24 |
        (channel & Font::Blue ? 255 : 0) << 16 |
        (channel & Font::Green ? 255 : 0) << 8 |
        (channel & Font::Red ? 255 : 0);
    return color;
}

