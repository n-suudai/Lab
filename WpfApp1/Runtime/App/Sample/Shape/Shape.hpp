
#pragma once


class Shape
{
public:
    struct ConstantBuffer
    {
        glm::mat4x4 Model;                  // モデル転送行列
        glm::mat4x4 ModelViewProjection;    // 転送行列
        glm::vec4   LightPosition;          // 平行光      ※逆行列で変換済み
        glm::vec4   EyePosition;            // 視線ベクトル ※逆行列で変換済み

        glm::vec4   DiffuseColor;           // ディフューズ
        glm::vec4   AmbientColor;           // アンビエント
        glm::vec4   SpecularColor;          // スペキュラ    w : power
        glm::vec4   EmissiveColor;          // エミッシブ

        void Initialize();
    };

    using Vertex = Vertex_PositionColorNormalTexture;

public:
    Shape(
        const ComPtr<ID3D11Device>& device,
        const ComPtr<ID3D11DeviceContext> context
    );

    ~Shape();

    // トーラスとして初期化
    void InitializeAsTorus(u16 x, u16 y, f32 irad, f32 orad, const glm::vec4* pColor = nullptr);

    // 球体として初期化
    void InitializeAsSphere(u16 x, u16 y, f32 rad, const glm::vec4* pColor = nullptr);

    // 立方体として初期化
    void InitializeAsCube(f32 scale, const glm::vec4* pColor = nullptr);

    // 円柱として初期化
    void InitializeAsCylinder(u16 x, u16 y, f32 rad, f32 height, const glm::vec4* pColor = nullptr);

    // 更新
    void Update(
        const glm::vec3* pTranslate = nullptr,
        const glm::vec3* pRotate = nullptr,
        const glm::vec3* pScale = nullptr
    );

    // 描画
    void Draw();

    //------------------------------
    // 各種設定関数
    //------------------------------

    void SetAmbientColor(const glm::vec4& ambient) { m_ConstantBufferData.AmbientColor = ambient; m_bUpdateContantBuffer = true; }

    void SetDiffuseColor(const glm::vec4& diffuse) { m_ConstantBufferData.DiffuseColor = diffuse; m_bUpdateContantBuffer = true; }

    void SetSpecularColor(const glm::vec4& specular) { m_ConstantBufferData.SpecularColor = specular; m_bUpdateContantBuffer = true; }

    void SetEmissiveColor(const glm::vec4& emissive) { m_ConstantBufferData.EmissiveColor = emissive; m_bUpdateContantBuffer = true; }

    void SetLightPosition(const glm::vec4& light) { m_LightPosition = light; m_bUpdateContantBuffer = true; }

    void SetEyePosition(const glm::vec4& eye) { m_EyePosition = eye; m_bUpdateContantBuffer = true; }

    void SetViewMatrix(const glm::mat4x4& view) { m_ViewMatrix = view; m_bUpdateContantBuffer = true; }

    void SetProjectionMatrix(const glm::mat4x4& projection) { m_ProjectionMatrix = projection; m_bUpdateContantBuffer = true; }

protected:
    //void CalculateTangentAndBinormal(
    //    std::vector<Vertex>& vertices,
    //    const std::vector<u16>& indices
    //);

    void Initialize(
        const std::vector<Vertex>& vertices,
        const std::vector<u16>& indices,
        const std::string& vertexShader,
        const std::string& pixelShader
    );

protected:
    bool        m_bUpdateContantBuffer; // 定数バッファ更新フラグ
    glm::vec4   m_LightPosition;        // 平行光 逆行列変換前 
    glm::vec4   m_EyePosition;          // 視線ベクトル 逆行列変換前
    glm::mat4x4 m_ViewMatrix;           // ビュー行列
    glm::mat4x4 m_ProjectionMatrix;     // プロジェクション行列

    UINT                                m_IndexCount;           // インデックス数
    ConstantBuffer                      m_ConstantBufferData;   // 定数バッファ用データ
    ComPtr<ID3D11Device>                m_Device;               // デバイス
    ComPtr<ID3D11DeviceContext>         m_Context;              // デバイスコンテキスト
    ComPtr<ID3D11Buffer>                m_VertexBuffer;         // 頂点バッファ
    ComPtr<ID3D11Buffer>                m_IndexBuffer;          // インデックスバッファ
    ComPtr<ID3D11Buffer>                m_ConstantBuffer;       // 定数バッファ
    ComPtr<ID3D11VertexShader>          m_VertexShader;         // 頂点シェーダー
    ComPtr<ID3D11PixelShader>           m_PixelShader;          // ピクセルシェーダー
    ComPtr<ID3D11InputLayout>           m_InputLayout;          // 入力レイアウト
    ComPtr<ID3D11RasterizerState>       m_RasterizerState;      // ラスタライザーステート
    ComPtr<ID3D11BlendState>            m_BlendState;           // ブレンドステート
    ComPtr<ID3D11SamplerState>          m_SamplerState;         // サンプラーステート

    std::unique_ptr<Texture>    m_DiffuseTexture;
    std::unique_ptr<Texture>    m_AmbientTexture;
    std::unique_ptr<Texture>    m_SpecularTexture;
    std::unique_ptr<Texture>    m_EmissiveTexture;
    std::unique_ptr<Texture>    m_NormalTexture;
};


