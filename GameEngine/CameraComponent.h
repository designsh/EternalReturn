#pragma once
#include "GameEngineTransformComponent.h"
#include "GameEngineDebugRenderData.h"
#include "GameEngineLightComponent.h"
#include "GameEnginePreprocessingRenderer.h"
#include "GameEngineShadowRenderer.h"
#include "DeferredCalLightEffect.h"
#include "DeferredMerge.h"
#include "Enums.h"

// �з� : ī�޶� ������Ʈ
// �뵵 : 
// ���� : 
class GameEngineRendererBase;
class CameraComponent : public GameEngineTransformComponent
{
	friend class CameraActor;
	friend class GameEngineLevel;
	friend class GameEngineRenderer;
public:
	CameraComponent();
	~CameraComponent();
	CameraComponent(const CameraComponent& _other) = delete;
	CameraComponent(CameraComponent&& _other) noexcept = delete;
	CameraComponent& operator=(const CameraComponent& _other) = delete;
	CameraComponent& operator=(const CameraComponent&& _other) = delete;

public:
	void CameraTransformUpdate();
	void CameraZoomReset();
	void CameraZoomSetting(float _Value);

public:
	void SetProjectionMode(ProjectionMode _ProjectionMode);
	void PushRenderer(int _Order, GameEngineRendererBase* _Renderer);
	void PushLight(GameEngineLightComponent* _Light);
	void PushPreprocessingRenderer(GameEngineRendererBase* _BaseRenderer, GameEnginePreprocessingRenderer* _PreprocessingRenderer);
	void PushShadowRenderer(GameEngineRendererBase* _BaseRenderer, GameEngineShadowRenderer* _ShadowRenderer);

public:
	void PushDebugRender(GameEngineTransform* _Trans, CollisionType _Type, float4 _Color = float4::GREEN);
	void ChangeRendererGroup(int _Group, GameEngineRendererBase* _Renderer);

public:
	inline const LightsData& GetLightData() const
	{
		return LightData_;
	}

	inline GameEngineRenderTarget* GetCameraDeferredTarget()
	{
		return CameraDeferredTarget_;
	}

	inline GameEngineRenderTarget* GetCameraDeferredGBufferTarget()
	{
		return CameraDeferredGBufferTarget;
	}

	inline GameEngineRenderTarget* GetCameraDeferredLightTarget()
	{
		return CameraDeferredLightTarget;
	}

	inline GameEngineRenderTarget* GetCameraForwardTarget_()
	{
		return CameraForwardTarget_;
	}

	inline GameEngineRenderTarget* GetCameraRenderTarget()
	{
		return CameraBufferTarget_;
	}

	inline GameEngineRenderTarget* GetLightShadowRenderTarget() const
	{
		return LightShadowRenderTarget_;
	}

	inline float GetZoomValue()
	{
		return ZoomValue_;
	}

	inline float GetFar() 
	{
		return FarZ_;
	}

	inline const LightsData& LightData() const
	{
		return LightData_;
	}

	inline void SetFov(float _fovAngle)
	{
		FovAngleY_ = _fovAngle;
	}

	DeferredCalLightEffect CalLightEffect;
	DeferredMerge DeferredMergeEffect;



protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	void ClearCameraTarget();
	void Render(float _DeltaTime);
	void DebugRender();
	void ReleaseRenderer();

	void NextLevelMoveRenderer(CameraComponent* _NextCamera, GameEngineActor* _Actor);

	void RenderForward(float _DeltaTime);
	void RenderDeffered(float _DeltaTime);
	void RenderLightShadow(float _DeltaTime);

private:
	private:	// member Var
		ProjectionMode				ProjectionMode_;			// 
		float						FovAngleY_;					// 
		float4						CamSize_;					// 
		float						NearZ_;						// 
		float						FarZ_;						// 
		float						ZoomValue;

private:
	LightsData LightData_;
	std::list<GameEngineLightComponent*> Lights_;
	std::map<int, std::list<GameEngineRendererBase*>> RendererList_;
	std::map<GameEngineRendererBase*, std::list<GameEngineRendererBase*>> PreprocessingRendererList_;
	std::map<GameEngineRendererBase*, GameEngineRendererBase*> ShadowRendererList_;

private:
	int DebugRenderCount_;
	float ZoomValue_;
	std::vector<GameEngineDebugRenderData> DebugVector_;
	GameEngineRenderTarget* CameraBufferTarget_;

	GameEngineRenderTarget* CameraForwardTarget_;

	GameEngineRenderTarget* CameraDeferredGBufferTarget;
	GameEngineRenderTarget* CameraDeferredLightTarget;
	GameEngineRenderTarget* CameraDeferredTarget_;

	GameEngineRenderTarget* LightShadowRenderTarget_;				// �׸���ó�� Ÿ��(����) : ����(��) ���� �׸��� ����Ÿ�� -> ���� ���ӿ��� ������ : ���� �ʿ� �븻�ؽ��İ� ��Ī�Ǿ����������Ƿ� ���Ұ�
	GameEngineRenderTarget* CameraPreprocessingTarget_;				// ��ó��������(�ܰ���, �Ƿ翧) Ÿ�� : ���̹��� ����
	GameEngineRenderTarget* CameraRendererShadowTarget_;			// �׸���ó�� Ÿ��(����) : �ϳ��� �޽��� �������� Ÿ�ٿ� ������ �� �ش�Ÿ���� �ؽ��ĸ� ȸ���Ͽ� ����Ÿ�ٿ� ��ġ�� Ÿ��
};

