

Texture2DArray<float4> diffuseTextures : register(t0);
SamplerState linearSampler : register(s0);


struct PS_INPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 tex : TEXCOORD;
    float4 channel : CHANNEL;
    uint   page : PAGE;
};


float4 main(PS_INPUT ps_input) : SV_TARGET
{
    // distance field texture
    //const float smoothing = 1.0 / 16.0;
    //float3 tex = float3(ps_input.tex, ps_input.page);
    //float distance = diffuseTextures.Sample(linearSampler, tex).a;
    //float alpha = smoothstep(0.5 - smoothing, 0.5 + smoothing, distance);
    //float4 color = float4(ps_input.color.rgb, ps_input.color.a * alpha);
    //return color;

    // simple texture
    float3 tex = float3(ps_input.tex, ps_input.page);
    float4 texColor = diffuseTextures.Sample(linearSampler, tex);

    if (dot(float4(1.0, 1.0, 1.0, 1.0), ps_input.channel))
    {
        float value = dot(texColor, ps_input.channel);

        texColor.rgb = value > 0.5 ? 2.0 * value - 1.0 : 0.0;
        texColor.a = value > 0.5 ? 2.0 * value - 1.0 : 0.0;
    }

    return ps_input.color * texColor;
}

