

void CreatePolygonNormal(
    const glm::vec4& p0,
    const glm::vec4& p1,
    const glm::vec4& p2,
    glm::vec4& outNormal
)
{
    const glm::vec4 v1 = p1 - p0;
    const glm::vec4 v2 = p2 - p1;

    glm::vec3 normal = glm::cross(
        glm::vec3(v1.x, v1.y, v1.z),
        glm::vec3(v2.x, v2.y, v2.z)
    );

    normal = glm::normalize(normal);
    outNormal.x = normal.x;
    outNormal.y = normal.y;
    outNormal.z = normal.z;
    outNormal.w = 1.0f;
}

void Shape::ConstantBuffer::Initialize()
{
    ModelViewProjection = glm::mat4x4(1);
    LightPosition = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    EyePosition = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

    DiffuseColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    AmbientColor = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
    SpecularColor = glm::vec4(1.0f, 1.0f, 1.0f, 50.0f);
    EmissiveColor = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
}

Shape::Shape(
    const ComPtr<ID3D11Device>& device,
    const ComPtr<ID3D11DeviceContext> context
)
    : m_Device(device)
    , m_Context(context)
{
    m_ConstantBufferData.Initialize();
}

Shape::~Shape()
{

}

// トーラスとして初期化
void Shape::InitializeAsTorus(u16 x, u16 y, f32 irad, f32 orad, const glm::vec4* pColor)
{
    if (x <= 2 || y <= 2) { return; }

    std::vector<Vertex> vertices;   // 頂点配列
    std::vector<u16>    indices;    // インデックス配列

    {
        for (u16 iy = 0; iy <= y; ++iy)
        {
            for (u16 ix = 0; ix <= x; ++ix)
            {
                Vertex v;

                // 頂点座標
                f32 ro = glm::radians(360.0f / x * ix);
                f32 ri = glm::radians(180.0f - 360.0f / y * iy);

                f32 ry = std::sinf(ri);
                f32 rr = std::cosf(ri);

                v.Position.x = (rr * irad + orad) * std::cosf(ro);
                v.Position.z = (rr * irad + orad) * std::sinf(ro);
                v.Position.y = ry * irad;
                v.Position.w = 1.0f;

                // 頂点色
                if (pColor != nullptr)
                {
                    v.Color.x = pColor->x;
                    v.Color.y = pColor->y;
                    v.Color.z = pColor->z;
                    v.Color.w = pColor->w;
                }
                else
                {
                    glm::vec3 color = glm::rgbColor(glm::vec3(360.0f / x * ix, 1.0f, 1.0f));
                    v.Color.x = color.x;
                    v.Color.y = color.y;
                    v.Color.z = color.z;
                    v.Color.w = 1.0f;
                }

                // 法線
                v.Normal.x = rr * std::cosf(ro);
                v.Normal.y = ry;
                v.Normal.z = rr * std::sinf(ro);
                v.Normal.w = 1.0f;

                // UV座標
                v.Texture.x = (1.0f / x) * ix;
                v.Texture.y = (1.0f / y) * iy;

                vertices.push_back(v);
            }
        }
    }

    // インデックス
    for (u16 iy = 0; iy <= y; ++iy)
    {
        for (u16 ix = 0; ix < x; ++ix)
        {
            u16 i0 = ix + x * iy;
            u16 i1 = i0 + 1;
            u16 i2 = i0 + x + 1;
            u16 i3 = i2 + 1;

            indices.push_back(i0);
            indices.push_back(i1);
            indices.push_back(i2);

            indices.push_back(i1);
            indices.push_back(i3);
            indices.push_back(i2);
        }
    }

    // 初期化
    Initialize(
        vertices,
        indices,
        "Resource\\Shader\\bin\\ShapeVS.cso",
        "Resource\\Shader\\bin\\ShapePS.cso"
    );
}

// 球体として初期化
void Shape::InitializeAsSphere(u16 x, u16 y, f32 rad, const glm::vec4* pColor)
{
    std::vector<Vertex> vertices;   // 頂点配列
    std::vector<u16>    indices;    // インデックス配列

    // 頂点配列を作成
    for (u16 iy = 0; iy <= y; iy++)
    {
        for (u16 ix = 0; ix <= x; ix++)
        {
            f32 r = glm::radians(180.0f / y * iy);
            f32 ry = std::cosf(r);
            f32 rr = std::sinf(r);

            Vertex v;
            f32 tr = glm::radians(360.0f / x * ix);

            // 頂点
            v.Position.x = rr * rad * std::cosf(tr);
            v.Position.y = ry * rad;
            v.Position.z = rr * rad * std::sinf(tr);
            v.Position.w = 1.0f;

            // 頂点色
            if (pColor != nullptr)
            {
                v.Color.x = pColor->x;
                v.Color.y = pColor->y;
                v.Color.z = pColor->z;
                v.Color.w = pColor->w;
            }
            else
            {
                glm::vec3 color = glm::rgbColor(glm::vec3(360.0f / x * ix, 1.0f, 1.0f));
                v.Color.x = color.x;
                v.Color.y = color.y;
                v.Color.z = color.z;
                v.Color.w = 1.0f;
            }

            // 法線
            v.Normal.x = rr * std::cosf(tr);
            v.Normal.y = ry;
            v.Normal.z = rr * std::sinf(tr);
            v.Normal.w = 1.0f;

            // テクスチャ座標
            v.Texture.x = 1.0f / x * ix;
            v.Texture.y = 1.0f / y * iy;

            vertices.push_back(v);
        }
    }

    // インデックス
    for (u16 iy = 0; iy <= y; ++iy)
    {
        for (u16 ix = 0; ix < x; ++ix)
        {
            u16 i0 = ix + x * iy;
            u16 i1 = i0 + 1;
            u16 i2 = i0 + x + 1;
            u16 i3 = i2 + 1;

            indices.push_back(i0);
            indices.push_back(i1);
            indices.push_back(i2);

            indices.push_back(i1);
            indices.push_back(i3);
            indices.push_back(i2);
        }
    }

    // 初期化
    Initialize(
        vertices,
        indices,
        "Resource\\Shader\\bin\\ShapeVS.cso",
        "Resource\\Shader\\bin\\ShapePS.cso"
    );
}

// 立方体として初期化
void Shape::InitializeAsCube(f32 scale, const glm::vec4* pColor)
{
    f32 hs = scale * 0.5f;

    // 頂点配列
    std::vector<Vertex> vertices = {
        //          Position                          Color                                Normal                                   Texture
        { glm::vec4( -hs, -hs,  hs, 1.0f ), glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f ), glm::vec4( -1.0f, -1.0f,  1.0f, 1.0f ),  glm::vec2( 0.0f, 0.0f ) },
        { glm::vec4(  hs, -hs,  hs, 1.0f ), glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f ), glm::vec4(  1.0f, -1.0f,  1.0f, 1.0f ),  glm::vec2( 1.0f, 0.0f ) },
        { glm::vec4(  hs,  hs,  hs, 1.0f ), glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f ), glm::vec4(  1.0f,  1.0f,  1.0f, 1.0f ),  glm::vec2( 1.0f, 1.0f ) },
        { glm::vec4( -hs,  hs,  hs, 1.0f ), glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f ), glm::vec4( -1.0f,  1.0f,  1.0f, 1.0f ),  glm::vec2( 0.0f, 1.0f ) },
                                                                                                                         
        { glm::vec4( -hs, -hs, -hs, 1.0f ), glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f ), glm::vec4( -1.0f, -1.0f, -1.0f, 1.0f ),  glm::vec2( 0.0f, 0.0f ) },
        { glm::vec4( -hs,  hs, -hs, 1.0f ), glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f ), glm::vec4( -1.0f,  1.0f, -1.0f, 1.0f ),  glm::vec2( 1.0f, 0.0f ) },
        { glm::vec4(  hs,  hs, -hs, 1.0f ), glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f ), glm::vec4(  1.0f,  1.0f, -1.0f, 1.0f ),  glm::vec2( 1.0f, 1.0f ) },
        { glm::vec4(  hs, -hs, -hs, 1.0f ), glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f ), glm::vec4(  1.0f, -1.0f, -1.0f, 1.0f ),  glm::vec2( 0.0f, 1.0f ) },
                                                                                                                         
        { glm::vec4( -hs,  hs, -hs, 1.0f ), glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f ), glm::vec4( -1.0f,  1.0f, -1.0f, 1.0f ),  glm::vec2( 0.0f, 0.0f ) },
        { glm::vec4( -hs,  hs,  hs, 1.0f ), glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f ), glm::vec4( -1.0f,  1.0f,  1.0f, 1.0f ),  glm::vec2( 1.0f, 0.0f ) },
        { glm::vec4(  hs,  hs,  hs, 1.0f ), glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f ), glm::vec4(  1.0f,  1.0f,  1.0f, 1.0f ),  glm::vec2( 1.0f, 1.0f ) },
        { glm::vec4(  hs,  hs, -hs, 1.0f ), glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f ), glm::vec4(  1.0f,  1.0f, -1.0f, 1.0f ),  glm::vec2( 0.0f, 1.0f ) },
                                                                                                                         
        { glm::vec4( -hs, -hs, -hs, 1.0f ), glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f ), glm::vec4( -1.0f, -1.0f, -1.0f, 1.0f ),  glm::vec2( 0.0f, 0.0f ) },
        { glm::vec4(  hs, -hs, -hs, 1.0f ), glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f ), glm::vec4(  1.0f, -1.0f, -1.0f, 1.0f ),  glm::vec2( 1.0f, 0.0f ) },
        { glm::vec4(  hs, -hs,  hs, 1.0f ), glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f ), glm::vec4(  1.0f, -1.0f,  1.0f, 1.0f ),  glm::vec2( 1.0f, 1.0f ) },
        { glm::vec4( -hs, -hs,  hs, 1.0f ), glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f ), glm::vec4( -1.0f, -1.0f,  1.0f, 1.0f ),  glm::vec2( 0.0f, 1.0f ) },
                                                                                                                         
        { glm::vec4(  hs, -hs, -hs, 1.0f ), glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f ), glm::vec4(  1.0f, -1.0f, -1.0f, 1.0f ),  glm::vec2( 0.0f, 0.0f ) },
        { glm::vec4(  hs,  hs, -hs, 1.0f ), glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f ), glm::vec4(  1.0f,  1.0f, -1.0f, 1.0f ),  glm::vec2( 1.0f, 0.0f ) },
        { glm::vec4(  hs,  hs,  hs, 1.0f ), glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f ), glm::vec4(  1.0f,  1.0f,  1.0f, 1.0f ),  glm::vec2( 1.0f, 1.0f ) },
        { glm::vec4(  hs, -hs,  hs, 1.0f ), glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f ), glm::vec4(  1.0f, -1.0f,  1.0f, 1.0f ),  glm::vec2( 0.0f, 1.0f ) },
                                                                                                                         
        { glm::vec4( -hs, -hs, -hs, 1.0f ), glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f ), glm::vec4( -1.0f, -1.0f, -1.0f, 1.0f ),  glm::vec2( 1.0f, 0.0f ) },
        { glm::vec4( -hs, -hs,  hs, 1.0f ), glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f ), glm::vec4( -1.0f, -1.0f,  1.0f, 1.0f ),  glm::vec2( 1.0f, 1.0f ) },
        { glm::vec4( -hs,  hs,  hs, 1.0f ), glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f ), glm::vec4( -1.0f,  1.0f,  1.0f, 1.0f ),  glm::vec2( 0.0f, 1.0f ) },
        { glm::vec4( -hs,  hs, -hs, 1.0f ), glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f ), glm::vec4( -1.0f,  1.0f, -1.0f, 1.0f ),  glm::vec2( 0.0f, 0.0f ) },
    };

    // インデックス配列
    std::vector<u16> indices = {
        0,  1,  2,  0,  2,  3,
        4,  5,  6,  4,  6,  7,
        8,  9,  10, 8,  10, 11,
        12, 13, 14, 12, 14, 15,
        16, 17, 18, 16, 18, 19,
        20, 21, 22, 20, 22, 23,
    };

    for (size_t i = 0; i < vertices.size(); i++)
    {
        if (pColor != nullptr)
        {
            vertices[i].Color.x = pColor->x;
            vertices[i].Color.y = pColor->y;
            vertices[i].Color.z = pColor->z;
            vertices[i].Color.w = pColor->w;
        }
        else
        {
            glm::vec3 color = glm::rgbColor(glm::vec3(360.0f / vertices.size() * i, 1.0f, 1.0f));
            vertices[i].Color.x = color.x;
            vertices[i].Color.y = color.y;
            vertices[i].Color.z = color.z;
            vertices[i].Color.w = 1.0f;
        }
    }

    // 初期化
    Initialize(
        vertices,
        indices,
        "Resource\\Shader\\bin\\ShapeVS.cso",
        "Resource\\Shader\\bin\\ShapePS.cso"
    );
}

// 円柱として初期化
void Shape::InitializeAsCylinder(u16 x, u16 y, f32 rad, f32 height, const glm::vec4* pColor)
{
    if (x <= 2 || y <= 2) { return; }

    std::vector<Vertex> vertices;   // 頂点配列
    std::vector<u16>    indices;    // インデックス配列

    for (u16 iy = 0; iy <= y; ++iy)
    {
        for (u16 ix = 0; ix <= x; ++ix)
        {
            Vertex v;
            v.Position.w = 1.0f;

            // 頂点座標
            if (iy == 0 || iy == y)
            {
                v.Position.x = 0.0f;
                v.Position.z = 0.0f;
                v.Position.y = height * 0.5f;
                if (iy == y)
                {
                    v.Position.y = -height * 0.5f;
                }
            }
            else
            {
                f32 r = glm::radians(360.0f / x * ix);
                v.Position.x = rad * std::cosf(r);
                v.Position.z = rad * std::sinf(r);

                f32 h = 0.5f - (1.0f / (y - 2) * (iy - 1));
                v.Position.y = height * h;
            }

            // 頂点色
            if (pColor != nullptr)
            {
                v.Color.x = pColor->x;
                v.Color.y = pColor->y;
                v.Color.z = pColor->z;
                v.Color.w = pColor->w;
            }
            else
            {
                glm::vec3 color = glm::rgbColor(glm::vec3(360.0f / x * ix, 1.0f, 1.0f));
                v.Color.x = color.x;
                v.Color.y = color.y;
                v.Color.z = color.z;
                v.Color.w = 1.0f;
            }

            // 法線
            v.Normal.w = 1.0f;
            if (iy == 0 || iy == y)
            {
                v.Normal.x = 0.0f;
                v.Normal.z = 0.0f;
                v.Normal.y = 1.0f;
                if (iy == y)
                {
                    v.Normal.y = -1.0f;
                }
            }
            else
            {
                f32 r = glm::radians(360.0f / x * ix);
                v.Normal.x = rad * std::cosf(r);
                v.Normal.z = rad * std::sinf(r);
                v.Normal.y = 0.0f;
            }

            // UV座標
            v.Texture.x = (1.0f / x) * ix;
            v.Texture.y = (1.0f / y) * iy;

            vertices.push_back(v);
        }
    }

    // インデックス
    for (u16 iy = 0; iy <= y; ++iy)
    {
        for (u16 ix = 0; ix < x; ++ix)
        {
            u16 i0 = ix + x * iy;
            u16 i1 = i0 + 1;
            u16 i2 = i0 + x + 1;
            u16 i3 = i2 + 1;

            indices.push_back(i0);
            indices.push_back(i1);
            indices.push_back(i2);

            indices.push_back(i1);
            indices.push_back(i3);
            indices.push_back(i2);
        }
    }

    // 初期化
    Initialize(
        vertices,
        indices,
        "Resource\\Shader\\bin\\ShapeVS.cso",
        "Resource\\Shader\\bin\\ShapePS.cso"
    );
}

void Shape::Update(
    const glm::vec3* pTranslate,
    const glm::vec3* pRotate,
    const glm::vec3* pScale
)
{
    glm::mat4x4 model = glm::mat4x4(1.0f);

    if (pTranslate != nullptr)
    {
        model = glm::translate(model, *pTranslate);
        m_bUpdateContantBuffer = true;
    }

    if (pRotate != nullptr)
    {
        model = glm::rotate(model, pRotate->x, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, pRotate->y, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, pRotate->z, glm::vec3(0.0f, 0.0f, 1.0f));
        m_bUpdateContantBuffer = true;
    }

    if (pScale != nullptr)
    {
        model = glm::scale(model, *pScale);
        m_bUpdateContantBuffer = true;
    }

    // 定数バッファデータ更新
    if (m_bUpdateContantBuffer)
    {
        glm::mat4x4 inverseModelMatrix = glm::inverse(model);

        m_ConstantBufferData.LightPosition = inverseModelMatrix * m_LightPosition;

        m_ConstantBufferData.EyePosition = inverseModelMatrix * m_EyePosition;

        m_ConstantBufferData.Model = glm::transpose(model);
        m_ConstantBufferData.ModelViewProjection = m_ProjectionMatrix * m_ViewMatrix * model;
        m_ConstantBufferData.ModelViewProjection = glm::transpose(m_ConstantBufferData.ModelViewProjection);
    }
}

void Shape::Draw()
{
    // 入力レイアウトを設定
    m_Context->IASetInputLayout(m_InputLayout.Get());

    // 頂点バッファを設定
    {
        constexpr UINT stride = sizeof(Vertex);
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
        // 定数バッファを更新
        if (m_bUpdateContantBuffer)
        {
            m_Context->UpdateSubresource(
                m_ConstantBuffer.Get(),
                0,
                nullptr,
                &m_ConstantBufferData,
                0,
                0
            );
            m_bUpdateContantBuffer = false;
        }

        ID3D11Buffer* pConstantBuffers[] = {
            m_ConstantBuffer.Get()
        };
        m_Context->VSSetConstantBuffers(0, _countof(pConstantBuffers), pConstantBuffers);
    }

    // ピクセルシェーダーを設定
    m_Context->PSSetShader(m_PixelShader.Get(), nullptr, 0);

    // ピクセルシェーダーに定数バッファを設定
    {
        ID3D11Buffer* pConstantBuffers[] = {
            m_ConstantBuffer.Get()
        };
        m_Context->PSSetConstantBuffers(0, _countof(pConstantBuffers), pConstantBuffers);
    }

    // ピクセルシェーダーにサンプラーステートを設定
    {
        ID3D11SamplerState* pSamplerStates[] = {
            m_SamplerState.Get()
        };
        m_Context->PSSetSamplers(0, _countof(pSamplerStates), pSamplerStates);
    }

    // ラスタライザーステートを設定
    m_Context->RSSetState(m_RasterizerState.Get());

    // ブレンドステートを設定
    m_Context->OMSetBlendState(m_BlendState.Get(), nullptr, 0xFFFFFFFF);

    // テクスチャをバインド
    {
        if (m_DiffuseTexture)
        {
            m_DiffuseTexture->Bind(0);
        }

        if (m_AmbientTexture)
        {
            m_AmbientTexture->Bind(1);
        }

        if (m_SpecularTexture)
        {
            m_SpecularTexture->Bind(2);
        }

        if (m_EmissiveTexture)
        {
            m_EmissiveTexture->Bind(3);
        }

        if (m_NormalTexture)
        {
            m_NormalTexture->Bind(4);
        }
    }

    m_Context->DrawIndexed(m_IndexCount, 0, 0);
}

void Shape::Initialize(
    const std::vector<Vertex>& vertices,
    const std::vector<u16>& indices,
    const std::string& vertexShader,
    const std::string& pixelShader
)
{
    // 頂点バッファ
    Util::CreateBuffer(
        m_Device,
        vertices.data(),
        static_cast<UINT>(vertices.size()) * sizeof(Vertex),
        D3D11_BIND_VERTEX_BUFFER,
        sizeof(Vertex),
        m_VertexBuffer
    );

    // インデックスバッファ
    Util::CreateBuffer(
        m_Device,
        indices.data(),
        static_cast<UINT>(indices.size()) * sizeof(u16),
        D3D11_BIND_INDEX_BUFFER,
        sizeof(u16),
        m_IndexBuffer
    );
    m_IndexCount = static_cast<UINT>(indices.size());

    // 定数バッファ
    {
        Util::CreateBuffer(
            m_Device,
            &m_ConstantBufferData,
            sizeof(m_ConstantBufferData),
            D3D11_BIND_CONSTANT_BUFFER,
            sizeof(m_ConstantBufferData),
            m_ConstantBuffer
        );

        // 定数バッファを更新
        m_Context->UpdateSubresource(
            m_ConstantBuffer.Get(),
            0,
            nullptr,
            &m_ConstantBufferData,
            0,
            0
        );
    }

    // 頂点シェーダー&入力レイアウト
    Util::CreateVertexShaderAndInputLayout(
        m_Device,
        vertexShader,
        Vertex::pInputElementDescs,
        Vertex::InputElementCount,
        m_VertexShader,
        m_InputLayout
    );

    // ピクセルシェーダー
    Util::CreatePixelShader(
        m_Device,
        pixelShader,
        m_PixelShader
    );

    // ラスタライザーステート
    Util::CreateRasterizerState(
        m_Device,
        D3D11_CULL_BACK,
        FALSE,
        m_RasterizerState
    );

    // ブレンドステート
    Util::CreateBlendState(
        m_Device,
        m_BlendState
    );

    // サンプラーステート
    Util::CreateSamplerState(
        m_Device,
        D3D11_FILTER_ANISOTROPIC,
        D3D11_TEXTURE_ADDRESS_WRAP,
        D3D11_TEXTURE_ADDRESS_WRAP,
        D3D11_TEXTURE_ADDRESS_WRAP,
        m_SamplerState
    );

    // テクスチャ
    {
        m_DiffuseTexture = std::make_unique<Texture>(
            m_Device,
            m_Context
            );
        m_DiffuseTexture->Initialize("..\\Assets\\Image\\metal.png");

        //m_AmbientTexture = std::make_unique<Texture>(
        //    m_Device,
        //    m_Context
        //    );
        //m_AmbientTexture->Initialize("..\\Assets\\Image\\icon.png");

        m_SpecularTexture = std::make_unique<Texture>(
            m_Device,
            m_Context
            );
        m_SpecularTexture->Initialize("..\\Assets\\Image\\metal_specular.png");

        //m_EmissiveTexture = std::make_unique<Texture>(
        //    m_Device,
        //    m_Context
        //    );
        //m_EmissiveTexture->Initialize("..\\Assets\\Image\\icon.png");

        m_NormalTexture = std::make_unique<Texture>(
            m_Device,
            m_Context
            );
        m_NormalTexture->Initialize("..\\Assets\\Image\\normal.png");
    }

    m_ViewMatrix = glm::mat4x4(1);
    m_ProjectionMatrix = glm::mat4x4(1);
    m_ConstantBufferData.Model = glm::mat4x4(1);

    m_bUpdateContantBuffer = true;
}

