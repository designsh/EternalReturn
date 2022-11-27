#pragma once
#include "GameEngineTransformComponent.h"
#include "GameEngineDebugRenderData.h"
#include "GameEngineLightComponent.h"
#include "GameEnginePreprocessingRenderer.h"
#include "GameEngineShadowRenderer.h"
#include "DeferredCalLightEffect.h"
#include "DeferredMerge.h"
#include "Enums.h"

// 분류 : 카메라 컴포넌트
// 용도 : 
// 설명 : 
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

	GameEngineRenderTarget* LightShadowRenderTarget_;				// 그림자처리 타겟(기존) : 광원(빛) 기준 그림자 렌더타겟 -> 현재 게임에서 사용못함 : 현재 맵에 노말텍스쳐가 매칭되어있지않으므로 사용불가
	GameEngineRenderTarget* CameraPreprocessingTarget_;				// 선처리렌더러(외곽선, 실루엣) 타겟 : 깊이버퍼 없음
	GameEngineRenderTarget* CameraRendererShadowTarget_;			// 그림자처리 타겟(변경) : 하나의 메쉬를 기준으로 타겟에 렌더링 후 해당타겟의 텍스쳐를 회전하여 메인타겟에 합치는 타겟
};

