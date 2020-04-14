

cbuffer CB0 : register(b0)
{
    float4x4 Projection;
};


struct VS_INPUT
{
    float2 position : POSITION;
    float4 color : COLOR;
    float2 tex : TEXCOORD;
    float4 channel : CHANNEL;
    uint   page : PAGE;
};


struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 tex : TEXCOORD;
    float4 channel : CHANNEL;
    uint   page : PAGE;
};


VS_OUTPUT main(VS_INPUT vs_input)
{
    VS_OUTPUT vs_output = (VS_OUTPUT)0;

    vs_output.position = mul(float4(vs_input.position.xy, 0.0, 1.0), Projection);
    vs_output.color = vs_input.color;
    vs_output.tex = vs_input.tex;
    vs_output.page = vs_input.page;
    vs_output.channel = vs_input.channel;

    return vs_output;
}

