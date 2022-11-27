
struct VertexIn
{
    float4 Position : POSITION;
    float4 Texcoord : TEXTURECOORD;
};

struct VertexOut
{
    float4 Position : SV_POSITION;
    float4 Texcoord : TEXTURECOORD;
};

VertexOut FogOfWar_VS(VertexIn _in)
{
    VertexOut Out = (VertexOut)0;

    Out.Position = _in.Position;
    Out.Texcoord = _in.Texcoord;

    return Out;
}


Texture2D Target : register(t0);
Texture2D Filter : register(t1);
SamplerState PointSmp : register(s0);

float4 FogOfWar_PS(VertexOut _in) : SV_Target0
{

    float4 targetColor = Target.Sample(PointSmp, _in.Texcoord.xy);
    float4 filterColor = Filter.Sample(PointSmp, _in.Texcoord.xy);

    return float4(targetColor.xyz * (0.70f + 0.1f * filterColor.x), 1.0f);
}