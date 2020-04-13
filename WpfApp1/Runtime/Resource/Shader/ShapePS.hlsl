
texture2D diffuseTexture    : register(t0);
texture2D ambientTexture    : register(t1);
texture2D specularTexture   : register(t2);
texture2D emissiveTexture   : register(t3);
texture2D normalTexture     : register(t4);


sampler linearSampler : register(s0);


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


struct PS_INPUT
{
    float4 position : SV_POSITION;  // ���_
    float4 color    : COLOR;        // ���_�F
    float2 tex      : TEXCOORD;     // �e�N�X�`�����W
    float3 light    : LIGHT;        // ���C�g�x�N�g��
    float3 eye      : EYE;          // �����x�N�g��
};


float4 main(PS_INPUT ps_input) : SV_TARGET
{
    //float4 diffuseColor  = diffuseTexture.Sample(linearSampler, ps_input.tex);
    //float4 ambientColor  = ambientTexture.Sample(linearSampler, ps_input.tex);
    //float4 specularColor = specularTexture.Sample(linearSampler, ps_input.tex);
    //float4 emissiveColor = emissiveTexture.Sample(linearSampler, ps_input.tex);

    float4 diffuseColor  = diffuseTexture.Sample(linearSampler, ps_input.tex);
    float4 ambientColor  = float4(1.0, 1.0, 1.0, 1.0);
    //float4 specularColor = specularTexture.Sample(linearSampler, ps_input.tex);
    float4 specularColor = float4(1.0, 1.0, 1.0, 1.0);
    float4 emissiveColor = float4(1.0, 1.0, 1.0, 1.0);
    float3 normal        = normalTexture.Sample(linearSampler, ps_input.tex).rgb;

    normal = normal * 2.0 - 1.0;

    diffuseColor  = diffuseColor  * DiffuseColor;
    ambientColor  = ambientColor  * AmbientColor;
    specularColor = specularColor * SpecularColor;
    emissiveColor = emissiveColor * EmissiveColor;

    float diffuse = clamp(dot(normal, ps_input.light), 0.1, 1.0);

    float3 halfLE  = normalize(ps_input.light + ps_input.eye);
    float specular = pow(clamp(dot(normal, halfLE), 0.0, 1.0), SpecularColor.w);

    float4 color = ps_input.color * diffuseColor * float4(diffuse, diffuse, diffuse, 1.0);
    color += ambientColor;
    color += float4(specular * specularColor.x, specular * specularColor.y, specular * specularColor.z, 1.0);
    color += emissiveColor;

    return color;
}

