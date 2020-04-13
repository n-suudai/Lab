

std::string GetValueStr(const std::string& line, const std::string& name)
{
    size_t pos = line.find(name);
    size_t size = name.size();

    // 見つからなかった
    if (pos == std::string::npos) { return ""; }

    size_t valueFirst = line.find('=', pos + size);
    valueFirst += 1;
    size_t valueEnd = line.find(' ', valueFirst);

    std::string result = "";

    if (valueFirst < valueEnd)
    {
        result = line.substr(valueFirst, valueEnd - valueFirst);
    }

    // ダブルクォーテーションを取り除く
    for (size_t c = result.find_first_of('\"'); c != std::string::npos; c = result.find_first_of('\"'))
    {
        result.erase(c, 1);
    }

    return result;
}

template<class T>
T ToValue(const std::string& valueStr, T notFound = T())
{
    if (valueStr == "") { return notFound; }

    std::istringstream iss;

    iss.str(valueStr);

    T value;

    iss >> value;

    return value;
}

template<class T>
T GetValue(const std::string& line, const std::string& name, T notFound = T())
{
    return ToValue<T>(GetValueStr(line, name), notFound);
}


FontPadding GetPadding(const std::string& line, const std::string& name)
{
    std::string valueStr = GetValueStr(line, name);

    FontPadding padding = { 0 };

    size_t first = 0;
    size_t last = 0;

    // up
    last = valueStr.find_first_of(',', first);
    padding.up = ToValue<u16>(valueStr.substr(first, last - first), 0);
    first = last + 1;

    // down
    last = valueStr.find_first_of(',', first);
    padding.down = ToValue<u16>(valueStr.substr(first, last - first), 0);
    first = last + 1;

    // right
    last = valueStr.find_first_of(',', first);
    padding.right = ToValue<u16>(valueStr.substr(first, last - first), 0);
    first = last + 1;

    // left
    padding.left = ToValue<u16>(valueStr.substr(first, valueStr.size() - first), 0);

    return padding;
}


FontSpacing GetSpacing(const std::string& line, const std::string& name)
{
    std::string valueStr = GetValueStr(line, name);

    FontSpacing spacing = { 0 };

    size_t first = 0;
    size_t last = 0;

    // horizontal
    last = valueStr.find_first_of(',', first);
    spacing.horizontal = ToValue<u16>(valueStr.substr(first, last - first), 0);
    first = last + 1;

    // vertical
    spacing.vertical = ToValue<u16>(valueStr.substr(first, valueStr.size() - first), 0);

    return spacing;
}


BitmapFont::BitmapFont(
    const ComPtr<ID3D11Device>& device,
    const ComPtr<ID3D11DeviceContext> context,
    const glm::mat4x4& projection
)
    : m_Device(device)
    , m_Context(context)
    , m_Projection(projection)
{
    m_Count = 0;
    m_CharSpacing = -3.0f;
    m_LineSpacing = 0;
    m_FontScale = 1.0f;
    m_RectMargin = glm::vec4(-2.0f, -2.0f, 2.0f, 2.0f);
}

BitmapFont::~BitmapFont()
{

}

void BitmapFont::FileLoad(const std::string& fontName, FontData& out)
{
    std::string fileName = "..\\Assets\\Font\\" + fontName + "\\" + fontName + ".fnt";

    std::ifstream file(fileName);

    file.unsetf(std::ios::skipws);  // スペースを読み飛ばさない
    file.seekg(std::ios::beg);  // 先頭にセット

    std::string lineStr = "";

    // info
    {
        std::getline(file, lineStr);
        out.info.face = GetValue<std::string>(lineStr, "face", "");
        out.info.size = GetValue<u16>(lineStr, "size", 0);
        out.info.bold = GetValue<u8>(lineStr, "bold", 0);
        out.info.italic = GetValue<u8>(lineStr, "italic", 0);
        out.info.charSet = GetValue<std::string>(lineStr, "charSet", "");
        out.info.unicode = GetValue<u8>(lineStr, "unicode", 0);
        out.info.stretchH = GetValue<u16>(lineStr, "streachH", 0);
        out.info.smooth = GetValue<u8>(lineStr, "smooth", 0);
        out.info.aa = GetValue<u8>(lineStr, "aa", 0);
        out.info.padding = GetPadding(lineStr, "padding");
        out.info.spacing = GetSpacing(lineStr, "spacing");
        out.info.outline = GetValue<u16>(lineStr, "outline", 0);
    }

    // common
    {
        lineStr = "";
        std::getline(file, lineStr);
        out.common.lineHeight = GetValue<u16>(lineStr, "lineHeight", 0);
        out.common.base = GetValue<u16>(lineStr, "base", 0);
        out.common.scaleW = GetValue<u16>(lineStr, "scaleW", 0);
        out.common.scaleH = GetValue<u16>(lineStr, "scaleH", 0);
        out.common.pages = GetValue<u16>(lineStr, "pages", 0);
        out.common.packed = GetValue<u8>(lineStr, "packed", 0);
        out.common.alphaChnl = GetValue<u8>(lineStr, "alphaChnl", 0);
        out.common.redChnl = GetValue<u8>(lineStr, "redChnl", 0);
        out.common.greenChnl = GetValue<u8>(lineStr, "greenChnl", 0);
        out.common.blueChnl = GetValue<u8>(lineStr, "blueChnl", 0);
    }

    // pages
    {
        FontPage page;
        for (u16 i = 0; i < out.common.pages; ++i)
        {
            lineStr = "";
            std::getline(file, lineStr);

            page.id = GetValue<u16>(lineStr, "id", 0);
            page.file = GetValue<std::string>(lineStr, "file", "");
            out.pages.push_back(page);
        }
    }

    // chars
    {
        lineStr = "";
        std::getline(file, lineStr);

        u32 charsCount = GetValue<u32>(lineStr, "count");
        u16 id = 0;

        for (u32 i = 0; i < FONT_DATA_CHARACTER_SIZE; ++i)
        {
            out.chars[i].id = 0xFFFF;
        }

        for (u32 i = 0; i < charsCount; ++i)
        {
            lineStr = "";
            std::getline(file, lineStr);

            id = GetValue<u16>(lineStr, "id", 0);
            out.chars[id].id = id;
            out.chars[id].x = GetValue<u16>(lineStr, "x", 0);
            out.chars[id].y = GetValue<u16>(lineStr, "y", 0);
            out.chars[id].width = GetValue<u16>(lineStr, "width", 0);
            out.chars[id].height = GetValue<u16>(lineStr, "height", 0);
            out.chars[id].xoffset = GetValue<u16>(lineStr, "xoffset", 0);
            out.chars[id].yoffset = GetValue<u16>(lineStr, "yoffset", 0);
            out.chars[id].xadvance = GetValue<u16>(lineStr, "xadvance", 0);
            out.chars[id].page = GetValue<u16>(lineStr, "page", 0);
            out.chars[id].chnl = GetValue<u8>(lineStr, "chnl", 0);
        }
    }

    // kerning
    {
        lineStr = "";
        std::getline(file, lineStr);

        u32 kerningsCount = GetValue<u32>(lineStr, "count", 0);

        FontKerning kerning = { 0 };

        for (u32 i = 0; i < kerningsCount; ++i)
        {
            lineStr = "";
            std::getline(file, lineStr);

            kerning.first = GetValue<u16>(lineStr, "first", 0);
            kerning.second = GetValue<u16>(lineStr, "second", 0);
            kerning.amount = GetValue<s16>(lineStr, "amount", 0);

            out.kernings.push_back(kerning);
        }
    }
}

void BitmapFont::Initialize(const std::string& fontName)
{
    // ファイル読み込み
    FileLoad(fontName, m_Data);
    
    // テクスチャ作成
    {
        std::vector<std::string> files;
        for (auto& page : m_Data.pages)
        {
            files.push_back("..\\Assets\\Font\\" + fontName + "\\" + page.file);
        }

        // テクスチャ配列として生成
        Util::CreateTextureArrayFromFile(
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

        Util::CreateBuffer(
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

        Util::CreateBuffer(
            m_Device,
            pIndex,
            sizeof(u16) * m_BufferSize * 6,
            D3D11_BIND_INDEX_BUFFER,
            sizeof(u16),
            m_IndexBuffer
        );
    }

    // 定数バッファ作成
    {
        Util::CreateBuffer(
            m_Device,
            &m_Projection,
            sizeof(m_Projection),
            D3D11_BIND_CONSTANT_BUFFER,
            sizeof(m_Projection),
            m_ConstantBuffer
        );

        // 定数バッファを更新
        m_Context->UpdateSubresource(
            m_ConstantBuffer.Get(),
            0,
            nullptr,
            &m_Projection,
            0,
            0
        );
    }

    // ラスタライザーステート作成
    {
        Util::CreateRasterizerState(
            m_Device,
            D3D11_CULL_BACK,
            FALSE,
            m_RasterizerState
        );
    }

    // ブレンドステート作成
    {
        Util::CreateBlendState(
            m_Device,
            m_BlendState
        );
    }

    // サンプラーステート作成
    {
        Util::CreateSamplerState(
            m_Device,
            D3D11_FILTER_ANISOTROPIC,
            D3D11_TEXTURE_ADDRESS_WRAP,
            D3D11_TEXTURE_ADDRESS_WRAP,
            D3D11_TEXTURE_ADDRESS_WRAP,
            m_SamplerState
        );
    }

    // 頂点シェーダーを作成
    {
        Util::CreateVertexShaderAndInputLayout(
            m_Device,
            "Resource\\Shader\\bin\\FontVS.cso",
            Vertex_BitmapFont::pInputElementDescs,
            Vertex_BitmapFont::InputElementCount,
            m_VertexShader,
            m_InputLayout
        );
    }

    // ピクセルシェーダーを作成
    {
        Util::CreatePixelShader(
            m_Device,
            "Resource\\Shader\\bin\\FontPS.cso",
            m_PixelShader
        );
    }

    m_SizePerPix = glm::vec2(1.0f / static_cast<f32>(m_TextureSize.width), 1.0f / static_cast<f32>(m_TextureSize.height));

    if (!m_Data.info.unicode)
    {
        // "あ","Ａ","A" これらの文字から標準フォント描画縦サイズを取得
        m_FontHeight = 22;
        static const u16 aTryCode[]{ 0x3042 ,0xFF21 };
        for (auto & code : aTryCode)
        {
            if (m_Data.chars[code].id != 0xFFFF)
            {
                m_FontHeight = m_Data.chars[code].height + 2.0f;
                break;
            }
        }
    }
    else
    {
        m_FontHeight = m_Data.chars[static_cast<int>('A')].height + 2.0f;
    }

    m_Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    m_CurrentColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

glm::vec4 BitmapFont::Put(const glm::vec2& position, const char* pUtf8)
{
    std::wstring widen = Util::Widen(pUtf8);
    return Put(position, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), widen.c_str());
}

glm::vec4 BitmapFont::Put(const glm::vec2& position, const glm::vec4& color, const char* pUtf8)
{
    std::wstring widen = Util::Widen(pUtf8);
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
        const FontCharacter* pChar = &m_Data.chars[utf16Code];
        const FontCharacter* pCharPrev = &m_Data.chars[utf16CodePrev];
        switch (utf16Code)
        {
        default:// 通常文字
            if (pChar->id != 0xFFFF)
            {
                PutVertex(pCharPrev, pChar);
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
    {
        ID3D11Buffer* pConstantBuffers[] = {
            m_ConstantBuffer.Get()
        };
        m_Context->VSSetConstantBuffers(0, _countof(pConstantBuffers), pConstantBuffers);
    }

    // ピクセルシェーダーを設定
    m_Context->PSSetShader(m_PixelShader.Get(), nullptr, 0);

    // ピクセルシェーダーにサンプラーステートを設定
    {
        ID3D11SamplerState* pSamplers[] = {
            m_SamplerState.Get()
        };
        m_Context->PSSetSamplers(0, _countof(pSamplers), pSamplers);
    }

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
    m_Context->OMSetBlendState(m_BlendState.Get(), nullptr, 0xFFFFFFFF);

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

void BitmapFont::PutVertex(const FontCharacter* pCharPrev, const FontCharacter* pChar)
{
    if (m_Count >= m_BufferSize) { return; }

    Vertex_BitmapFont * pVtx = m_VertexStream.get() + m_Count * 4;

    f32 x = m_Location.x + (pChar->xoffset * m_FontScale);
    f32 y = m_Location.y + (pChar->yoffset * m_FontScale);

    FontKerning* pKerning = GetKering(pCharPrev, pChar);

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
    pVtx[0].Color = Util::ColorPack(m_CurrentColor);
    pVtx[1].Color = pVtx[0].Color;
    pVtx[2].Color = pVtx[0].Color;
    pVtx[3].Color = pVtx[0].Color;
    pVtx[0].Page = pChar->page;
    pVtx[1].Page = pVtx[0].Page;
    pVtx[2].Page = pVtx[0].Page;
    pVtx[3].Page = pVtx[0].Page;

    m_Count++;
}

FontKerning* BitmapFont::GetKering(const FontCharacter* pCharLeft, const FontCharacter* pCharRight)
{
    if (pCharLeft == pCharRight) { return nullptr; }

    FontKerning* pKerning = nullptr;

    for (auto& kerning : m_Data.kernings)
    {
        if (kerning.first == pCharLeft->id && kerning.second == pCharRight->id)
        {
            pKerning = &kerning;
            break;
        }
    }

    return pKerning;
}

