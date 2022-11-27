#include "CbufferHeader.fx"
#include "AniHeader.fx"

struct VertexIn
{
    float4 Position : POSITION;
    float4 Weight : BLENDWEIGHT;
    int4 Index : BLENDINDICES;
};

struct VertexOut
{
    float4 Position : SV_POSITION;
    float4 ProjectionPosition : POSITION;
};

VertexOut LightShadow_VS(VertexIn _In)
{
    VertexOut Out = (VertexOut)0;
    
    if(1 == IsAni)
    {
        Skinning(_In.Position, _In.Weight, _In.Index, ArrAniMationMatrix);
    }
    
    _In.Position.w = 1.0f;
    Out.Position = mul(_In.Position, WVP_);
    
    // 투영영역까지 이동한 정점의 위치정보를 저장
    Out.ProjectionPosition = Out.Position;
    
    return Out;
}

float4 LightShadow_PS(VertexOut _In) : SV_Target0
{
    return float4(_In.ProjectionPosition.z / _In.ProjectionPosition.w, 0.0f, 0.0f, 1.0f);
}
