

cbuffer CB0 : register(b0)
{
    float4x4 Model;                 // ���f���]���s��
    float4x4 ModelViewProjection;   // �]���s��
    float4   LightPosition;         // �_����      ���t�s��ŕϊ��ς�
    float4   EyePosition;           // ���_        ���t�s��ŕϊ��ς�

    float4   DiffuseColor;           // �f�B�t���[�Y
    float4   AmbientColor;           // �A���r�G���g
    float4   SpecularColor;          // �X�y�L����    w : power
    float4   EmissiveColor;          // �G�~�b�V�u
};


struct VS_INPUT
{
    float4 position : POSITION;     // ���_
    float4 color    : COLOR;        // ���_�F
    float4 normal   : NORMAL;       // �@��
    float2 tex      : TEXCOORD;     // �e�N�X�`�����W
};


struct VS_OUTPUT
{
    float4 position : SV_POSITION;  // ���_
    float4 color    : COLOR;        // ���_�F
    float2 tex      : TEXCOORD;     // �e�N�X�`�����W
    float3 light    : LIGHT;        // ���C�g�x�N�g��
    float3 eye      : EYE;          // �����x�N�g��
};


VS_OUTPUT main(VS_INPUT vs_input)
{
    VS_OUTPUT vs_output = (VS_OUTPUT)0;

    float3 position = mul(vs_input.position, Model).xyz;
    float3 light = LightPosition.xyz - position;
    float3 eye = EyePosition.xyz - position;

    // �ڃx�N�g���A�]�ڐ��x�N�g�����ȈՌv�Z
    float3 normal = normalize(vs_input.normal.xyz);
    float3 tangent = normalize(cross(normal, float3(0.0, 1.0, 0.0))); // float3(0.0, 1.0, 0.0) �ƊO�ς����͖̂{���͗ǂ��Ȃ�
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

