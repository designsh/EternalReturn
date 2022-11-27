#include "LightHeader.fx"

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

VertexOut DeferredCalLight_VS(VertexIn _In)
{
    VertexOut Out = (VertexOut) 0;
    Out.Position = _In.Position;
    Out.Texcoord = _In.Texcoord;
    return Out;
}


Texture2D PositionTex : register(t1);
Texture2D NormalTex : register(t2);
Texture2DArray LightShadowTex : register(t3);                                        // 1장의 텍스쳐에 레벨에서 사용하는 모든 빛의 깊이데이터를 기록한 텍스쳐

SamplerState Smp : register(s0);

struct LightOutPut
{
    float4 DifLight : SV_Target0; // 텍스처 색깔
    float4 SpcLight : SV_Target1;
    float4 AmbLight : SV_Target2;
};

LightOutPut DeferredCalLight_PS(VertexOut _In)
{
    LightOutPut Out = (LightOutPut) 0.0f;
    
    float4 ViewPosition = (PositionTex.Sample(Smp, _In.Texcoord.xy));
    float4 ViewNormal = (NormalTex.Sample(Smp, _In.Texcoord.xy));
    
    if (0.0f >= ViewPosition.a)
    {
        clip(-1);
    }
    ViewNormal.w = 0.0f;
    
    Out.DifLight = (float4) 0.0f;
    Out.SpcLight = (float4) 0.0f;
    Out.AmbLight = (float4) 0.0f;
    
    for (int i = 0; i < LightCount; ++i)
    {
        Out.DifLight += CalculateDirectionDiffuseLight(ViewNormal, Lights[i]);
        Out.SpcLight += CalculateDirectionSpacularLight(ViewPosition, ViewNormal, Lights[i]);
        Out.AmbLight += CalculateDirectionAmbientLight(Lights[i]);
    }
    
    //============================================================= 그림자 계산
    int Count = 0;
    for (int LightNum = 0; LightNum < LightCount; ++LightNum)
    {
        if (Out.DifLight.x > 0.0f)
        {
            float4 WorldPosition = mul(float4(ViewPosition.xyz, 1.0f), Lights[LightNum].CameraViewInverse);
            float4 ShaodwPos = mul(WorldPosition, Lights[LightNum].LightVP);
            float fDepth = ShaodwPos.z / ShaodwPos.w;
            float fUvX = ShaodwPos.x / ShaodwPos.w;
            float fUvY = ShaodwPos.y / ShaodwPos.w;
            
            float2 ShadowUv = float2(fUvX * 0.5f + 0.5f, fUvY * -0.5f + 0.5f);
            if (0.001f < ShadowUv.x && 0.999f > ShadowUv.x && 0.001f < ShadowUv.y && 0.999f > ShadowUv.y)
            {
                float fShadowDepth = LightShadowTex.Sample(Smp, float3(ShadowUv.x, ShadowUv.y, LightNum)).r;
                
                if (0.0f < fShadowDepth && fDepth > fShadowDepth + 0.001f)
                {
                    Count++;
                }
            }
        }
    }

    if (0 != Count)
    {
        Out.DifLight *= 0.5f;
        Out.SpcLight *= 0.5f;
    }
    
    for (int ShadowNum = 0; ShadowNum < Count; ++ShadowNum)
    {
        Out.DifLight -= float4(0.5f, 0.5f, 0.5f, 0.5f);
    }
    
    //============================================================= 그림자 계산
    
    return Out;
}


