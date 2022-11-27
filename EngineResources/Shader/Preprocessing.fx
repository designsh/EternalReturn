#include "CbufferHeader.fx"
#include "LightHeader.fx"

struct VertexIn
{
    float4 Position : POSITION;
    float4 Texcoord : TEXTURECOORD;
    float4 Normal : NORMAL;
    float4 BiNormal : BINORMAL;
    float4 Tangent : TANGENT;
};

struct VertexOut
{
    // 
    float4 Position : SV_POSITION;
    float4 ViewPosition : POSITION;
    float4 Texcoord : TEXTURECOORD;
    float4 ViewNormal : NORMAL;
    float4 ViewBiNormal : BINORMAL;
    float4 ViewTangent : TANGENT;
};

VertexOut Preprocessing_VS(VertexIn _In)
{
    VertexOut Out = (VertexOut) 0;
    
    _In.Position.w = 1.0f;
    Out.Position = mul(_In.Position, WVP_);
    Out.ViewPosition = mul(_In.Position, WV_);
    Out.Texcoord = _In.Texcoord;
    
    _In.Normal.w = 0.0f;
    Out.ViewNormal = normalize(mul(_In.Normal, WV_));
    Out.ViewNormal.w = 0.0f;
    
    _In.BiNormal.w = 0.0f;
    Out.ViewBiNormal = normalize(mul(_In.BiNormal, WV_));
    Out.ViewBiNormal.w = 0.0f;
    
    _In.Tangent.w = 0.0f;
    Out.ViewTangent = normalize(mul(_In.Tangent, WV_));
    Out.ViewTangent.w = 0.0f;
    
    return Out;
}

cbuffer OutLineData : register(b1)
{
    float4 LineColor;
    float LineThickness;
};

float4 Preprocessing_PS(VertexOut _In) : SV_Target0
{
    return LineColor;
}

