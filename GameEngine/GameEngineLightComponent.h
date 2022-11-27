#pragma once
#include "GameEngineTransformComponent.h"
#include "GameEngineRenderingPipeLine.h"
#include "GameEngineShaderResHelper.h"

enum class LightShapeType
{
	Direction,
	Mesh,
};

// ����Ʈ(����) 1���� ������
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
	float4 LightPower;							// x�� ��ǻ�� ����Ʈ�� ���� y�� ����ŧ���� ���� z�� �ں��Ʈ�� ���� w�� ��� ����};

	//===================== �׸��� ���뵥����
	float4x4 LightView;
	float4x4 LightProj;
	float4x4 LightVP;
	float4x4 CameraViewInverse;
};

// ��ü ����Ʈ ������ ���
struct LightsData 
{
	int LightCount;
	LightData Lights[128];
};

// �з� : ������Ʈ
// �뵵 : ��(����)
// ���� : �� ���� �� �׸������� �������
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
	ProjectionMode LightShadowProjectionMode_;								// ����(��)�� �׸��ڸ� �������Ҷ� ���������� ���� ShadowRenderTarget_�� ������
	ID3D11RenderTargetView* LightShadowTargetView_;							// ShadowRenderTarget_�� ���ҽ���
	ID3D11DepthStencilView* LightShadowTargetDepth_;						// ShadowRenderTarget_�� �����ϴ� ����/���ٽ� ���ҽ���

private: // Shadow RenderTarget Related Value
	float4 LightShadowClipingRange_;										// Ŭ��������� ����(�ʺ�,����)
	float LightShadowClipingNear_;											// Near
	float LightShadowClipingFar_;											// Far
};

