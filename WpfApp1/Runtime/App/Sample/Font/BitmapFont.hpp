#pragma once



struct FontPadding
{
    u16 up;
    u16 down;
    u16 right;
    u16 left;
};


struct FontSpacing
{
    u16 horizontal;
    u16 vertical;
};


// This tag holds information on how the font was generated.
struct FontInfo
{
    std::string face;       // This is the name of the true type font.
    u16         size;       // The size of the true type font.
    u8          bold;       // The font is bold.
    u8          italic;     // The font is italic.
    std::string charSet;    // The name of the OEM charset used (when not unicode).
    u8          unicode;    // Set to 1 if it is the unicode charset.
    u16         stretchH;   // The font height stretch in percentage. 100% means no stretch.
    u8          smooth;     // Set to 1 if smoothing was turned on.
    u8          aa;         // The supersampling level used. 1 means no supersampling was used.
    FontPadding padding;    // The padding for each character (up, right, down, left).
    FontSpacing spacing;    // The spacing for each character (horizontal, vertical).
    u16         outline;    // The outline thickness for the characters.
};


// This tag holds information common to all characters.
struct FontCommon
{
    u16 lineHeight;    // This is the distance in pixels between each line of text.
    u16 base;          // The number of pixels from the absolute top of the line to the base of the characters.
    u16 scaleW;        // The width of the texture, normally used to scale the x pos of the character image.
    u16 scaleH;        // The height of the texture, normally used to scale the y pos of the character image.
    u16 pages;         // The number of texture pages included in the font.
    u8  packed;        // Set to 1 if the monochrome characters have been packed into each of the texture channels. In this case alphaChnl describes what is stored in each channel.
    u8  alphaChnl;     // Set to 0 if the channel holds the glyph data, 1 if it holds the outline, 2 if it holds the glyph and the outline, 3 if its set to zero, and 4 if its set to one.
    u8  redChnl;       // Set to 0 if the channel holds the glyph data, 1 if it holds the outline, 2 if it holds the glyph and the outline, 3 if its set to zero, and 4 if its set to one.
    u8  greenChnl;     // Set to 0 if the channel holds the glyph data, 1 if it holds the outline, 2 if it holds the glyph and the outline, 3 if its set to zero, and 4 if its set to one.
    u8  blueChnl;      // Set to 0 if the channel holds the glyph data, 1 if it holds the outline, 2 if it holds the glyph and the outline, 3 if its set to zero, and 4 if its set to one.
};


// This tag gives the name of a texture file. There is one for each page in the font.
struct FontPage
{
    u16         id;     // The page id.
    std::string file;   // The texture file name.
};


// This tag describes on character in the font. There is one for each included character in the font.
struct FontCharacter
{
    u16 id;        // The character id.
    u16 x;         // The left position of the character image in the texture.
    u16 y;         // The top position of the character image in the texture.
    u16 width;     // The width of the character image in the texture.
    u16 height;    // The height of the character image in the texture.
    s16 xoffset;   // How much the current position should be offset when copying the image from the texture to the screen.
    s16 yoffset;   // How much the current position should be offset when copying the image from the texture to the screen.
    u16 xadvance;  // How much the current position should be advanced after drawing the character.
    u16 page;      // The texture page where the character image is found.
    u8  chnl;      // The texture channel where the character image is found (1 = blue, 2 = green, 4 = red, 8 = alpha, 15 = all channels).
};


// The kerning information is used to adjust the distance between certain characters,
// e.g. some characters should be placed closer to each other than others.
struct FontKerning
{
    u16 first;     // The first character id.
    u16 second;    // The second character id.
    s16 amount;    // How much the x position should be adjusted when drawing the second character immediately following the first.
};


enum
{
    FONT_DATA_CHARACTER_SIZE = 0x10000
};


struct FontData
{
    FontInfo                    info;
    FontCommon                  common;
    std::vector<FontPage>       pages;
    FontCharacter               chars[FONT_DATA_CHARACTER_SIZE];
    std::vector<FontKerning>    kernings;
};

enum ChannelFlag
{
    ChannelFlag_Blue    = (1 << 0),
    ChannelFlag_Green   = (1 << 1),
    ChannelFlag_Red     = (1 << 2),
    ChannelFlag_Alpha   = (1 << 3),
    ChannelFlag_All     = ChannelFlag_Blue | ChannelFlag_Green | ChannelFlag_Red | ChannelFlag_Alpha,
};


struct Vertex_BitmapFont
{
    glm::vec2 Position;
    u32    Color;
    glm::vec2 Texture;
    u32    Channel;
    u16    Page;

    static constexpr UINT PositionOffset = 0;
    static constexpr UINT PositionSize = sizeof(glm::vec2);

    static constexpr UINT ColorOffset = PositionOffset + PositionSize;
    static constexpr UINT ColorSize = sizeof(u32);

    static constexpr UINT TextureOffset = ColorOffset + ColorSize;
    static constexpr UINT TextureSize = sizeof(glm::vec2);

    static constexpr UINT ChannelOffset = TextureOffset + TextureSize;
    static constexpr UINT ChannelSize = sizeof(u16);

    static constexpr UINT PageOffset = ChannelOffset + ChannelSize;
    static constexpr UINT PageSize = sizeof(u16);

    static constexpr D3D11_INPUT_ELEMENT_DESC pInputElementDescs[] = {
        { "POSITION", 0,    DXGI_FORMAT_R32G32_FLOAT,   0,  0,                              D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",    0,    DXGI_FORMAT_R8G8B8A8_UNORM, 0,  D3D11_APPEND_ALIGNED_ELEMENT,   D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0,    DXGI_FORMAT_R32G32_FLOAT,   0,  D3D11_APPEND_ALIGNED_ELEMENT,   D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "CHANNEL",  0,    DXGI_FORMAT_R8G8B8A8_UNORM, 0,  D3D11_APPEND_ALIGNED_ELEMENT,   D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "PAGE",     0,    DXGI_FORMAT_R16_UINT,       0,  D3D11_APPEND_ALIGNED_ELEMENT,   D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    static constexpr UINT InputElementCount = _countof(pInputElementDescs);
};


class BitmapFont
{
public:
    BitmapFont(
        const ComPtr<ID3D11Device>& device,
        const ComPtr<ID3D11DeviceContext> context,
        const glm::mat4x4& projection
    );

    ~BitmapFont();

    void FileLoad(const std::string& fileName, FontData& out);

    void Initialize(const std::string& fileName);

    //glm::vec4 PutFormat(const glm::vec2& position, const char* pFormat, ...);

    //glm::vec4 PutFormat(const glm::vec2& position, const glm::vec4& color, const char* pFormat, ...);

    glm::vec4 Put(const glm::vec2& position, const char* pUtf8);

    glm::vec4 Put(const glm::vec2& position, const glm::vec4& color, const char* pUtf8);

    glm::vec4 Put(const glm::vec2& position, const wchar_t* pUtf16);

    glm::vec4 Put(const glm::vec2& position, const glm::vec4& color, const wchar_t* pUtf16);

    void Draw();

    void Clear();

    void Flush();

protected:
    void PutVertex(const FontCharacter* pCharPrev, const FontCharacter* pChar);

    FontKerning* GetKering(const FontCharacter* pCharLeft, const FontCharacter* pCharRight);

    u32 GetChannelColor(u8 channel) const;

    ComPtr<ID3D11Device>                m_Device;               // デバイス
    ComPtr<ID3D11DeviceContext>         m_Context;              // デバイスコンテキスト
    ComPtr<ID3D11ShaderResourceView>    m_ShaderResourceView;   // シェーダーリソース
    ComPtr<ID3D11Texture2D>             m_Texture2D;            // テクスチャ
    ComPtr<ID3D11Buffer>                m_VertexBuffer;         // 頂点バッファ
    ComPtr<ID3D11Buffer>                m_IndexBuffer;          // インデックスバッファ
    ComPtr<ID3D11Buffer>                m_ConstantBuffer;       // 定数バッファ
    std::unique_ptr<Vertex_BitmapFont>  m_VertexStream;         // CPU側頂点バッファ      m_BufferSize * 4 頂点
    ComPtr<ID3D11VertexShader>          m_VertexShader;         // 頂点シェーダー
    ComPtr<ID3D11PixelShader>           m_PixelShader;          // ピクセルシェーダー
    ComPtr<ID3D11InputLayout>           m_InputLayout;          // 入力レイアウト
    ComPtr<ID3D11SamplerState>          m_SamplerState;         // フォント描画用サンプラー
    ComPtr<ID3D11RasterizerState>       m_RasterizerState;      // ラスタライザーステート
    ComPtr<ID3D11BlendState>            m_BlendState;           // ブレンドステート

    Size2D      m_TextureSize;
    FontData    m_Data;
    f32         m_FontScale;
    bool        m_UpdateBufferRq;   // バッファー更新リクエスト
    u32         m_BufferSize;       // 描画最大文字数
    u32         m_Count;            // 表示文字数
    f32         m_CharSpacing;      // 横文字間
    f32         m_LineSpacing;      // 縦文字間
    glm::vec4   m_RectDrawArea;     // 描画領域
    glm::vec2   m_Location;         // 現在文字位置
    f32         m_FontHeight;       // フォントの縦描画サイズ
    glm::vec2   m_SizePerPix;       // 1ピクセルあたりのUV値
    glm::vec4   m_Color;            // 描画乗算色(全体)
    glm::vec4   m_CurrentColor;     // 現在の文字色
    glm::mat4x4 m_Projection;       // プロジェクション行列
    glm::vec4   m_RectMargin;       // 矩形マージン
};


