#pragma once
#include "GameEngineTransformComponent.h"
#include "GameEngineRenderingPipeLine.h"
#include "GameEngineShaderResHelper.h"

enum class LightShapeType
{
	Direction,
	Mesh,
};

// 라이트(광원) 1개의 데이터
struct LightData
{
	float4 ViewLightDir;
	float4 ViewNegLightDir;
	float4 ViewLightPosition;
	float4 AmbientLight;
	float4 DiffuseLightColor;
	float4 AmbientLightColor;
	float4 SpacularLightColor;
	float4 SpacularLightPow;
	float4 LightPower;							// x는 디퓨즈 라이트의 강도 y는 스펙큘러의 강도 z는 앰비언트의 강도 w는 모든 강도};

	//===================== 그림자 계산용데이터
	float4x4 LightView;
	float4x4 LightProj;
	float4x4 LightVP;
	float4x4 CameraViewInverse;
};

// 전체 라이트 데이터 목록
struct LightsData 
{
	int LightCount;
	LightData Lights[128];
};

// 분류 : 컴포넌트
// 용도 : 빛(광원)
// 설명 : 빛 연산 및 그림자적용 기능제공
class GameEngineLightComponent : public GameEngineTransformComponent
{
public: // Public Inline Get Function
	inline const LightData& GetLightData() 
	{
		return LightDataObject_;
	}

public: // Public Inline Set Function
	inline void SetDiffusePower(float _Power)
	{
		LightDataObject_.LightPower.x = _Power;
	}

	inline void SetSpacularLightPow(float _Pow)
	{
		LightDataObject_.SpacularLightPow.x = _Pow;
	}

	inline void SetAmbientPower(float _Power)
	{
		LightDataObject_.LightPower.z = _Power;
	}

	inline void SetLightShadowClipingRange(float4 _LightShadowClipingRange)
	{
		LightShadowClipingRange_ = _LightShadowClipingRange;
	}

public: // Public ShadowTarget Related Function
	void LightShadowTargetSetting();

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

public:
	GameEngineLightComponent();
	~GameEngineLightComponent();

protected:
	GameEngineLightComponent(const GameEngineLightComponent& _Other) = delete;
	GameEngineLightComponent(GameEngineLightComponent&& _Other) noexcept = delete;

private:
	GameEngineLightComponent& operator=(const GameEngineLightComponent& _Other) = delete;
	GameEngineLightComponent& operator=(GameEngineLightComponent&& _Other) noexcept = delete;

private: // Light Related Value
	LightShapeType ShapeType_;
	LightData LightDataObject_;

private: // Shadow Related Value
	ProjectionMode LightShadowProjectionMode_;								// 광원(빛)이 그림자를 렌더링할때 직교투영을 통해 ShadowRenderTarget_에 렌더링
	ID3D11RenderTargetView* LightShadowTargetView_;							// ShadowRenderTarget_의 리소스뷰
	ID3D11DepthStencilView* LightShadowTargetDepth_;						// ShadowRenderTarget_에 적용하는 깊이/스텐실 리소스뷰

private: // Shadow RenderTarget Related Value
	float4 LightShadowClipingRange_;										// 클리핑평면의 범위(너비,높이)
	float LightShadowClipingNear_;											// Near
	float LightShadowClipingFar_;											// Far
};

