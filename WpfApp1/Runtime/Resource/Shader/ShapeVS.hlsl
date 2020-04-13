

cbuffer CB0 : register(b0)
{
    float4x4 Model;                 // モデル転送行列
    float4x4 ModelViewProjection;   // 転送行列
    float4   LightPosition;         // 点光源      ※逆行列で変換済み
    float4   EyePosition;           // 視点        ※逆行列で変換済み

    float4   DiffuseColor;           // ディフューズ
    float4   AmbientColor;           // アンビエント
    float4   SpecularColor;          // スペキュラ    w : power
    float4   EmissiveColor;          // エミッシブ
};


struct VS_INPUT
{
    float4 position : POSITION;     // 頂点
    float4 color    : COLOR;        // 頂点色
    float4 normal   : NORMAL;       // 法線
    float2 tex      : TEXCOORD;     // テクスチャ座標
};


struct VS_OUTPUT
{
    float4 position : SV_POSITION;  // 頂点
    float4 color    : COLOR;        // 頂点色
    float2 tex      : TEXCOORD;     // テクスチャ座標
    float3 light    : LIGHT;        // ライトベクトル
    float3 eye      : EYE;          // 視線ベクトル
};


VS_OUTPUT main(VS_INPUT vs_input)
{
    VS_OUTPUT vs_output = (VS_OUTPUT)0;

    float3 position = mul(vs_input.position, Model).xyz;
    float3 light = LightPosition.xyz - position;
    float3 eye = EyePosition.xyz - position;

    // 接ベクトル、従接線ベクトルを簡易計算
    float3 normal = normalize(vs_input.normal.xyz);
    float3 tangent = normalize(cross(normal, float3(0.0, 1.0, 0.0))); // float3(0.0, 1.0, 0.0) と外積を取るのは本当は良くない
    float3 binormal = normalize(cross(normal, tangent));

    vs_output.light.x = dot(tangent, light);
    vs_output.light.y = dot(binormal, light);
    vs_output.light.z = dot(normal, light);
    vs_output.light = normalize(vs_output.light);

    vs_output.eye.x = dot(tangent, eye);
    vs_output.eye.y = dot(binormal, eye);
    vs_output.eye.z = dot(normal, eye);
    vs_output.eye = normalize(vs_output.eye);

    vs_output.position = mul(vs_input.position, ModelViewProjection);
    vs_output.color = vs_input.color;
    vs_output.tex = vs_input.tex;

    return vs_output;
}

