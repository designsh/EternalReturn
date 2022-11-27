#pragma once
#include "RenderSet.h"
#include "FBXAnimation.h"

class CameraComponent;
class GameEngineLevel;
class GameEngineIndexBuffer;
class GameEngineVertexBuffer;
class GameEngineRenderingPipeLine;
class GameEnginePreprocessingRenderer;
class GameEngineShadowRenderer;
class GameEngineFBXRenderer : public GameEngineRendererBase
{
	friend FBXAnimation;

private:
	friend GameEngineLevel;
	friend CameraComponent;

public:
	GameEngineFBXRenderer();
	~GameEngineFBXRenderer();
	GameEngineFBXRenderer(const GameEngineFBXRenderer& _Other) = delete;
	GameEngineFBXRenderer(GameEngineFBXRenderer&& _Other) noexcept = delete;
	GameEngineFBXRenderer& operator=(const GameEngineFBXRenderer& _Other) = delete;
	GameEngineFBXRenderer& operator=(GameEngineFBXRenderer&& _Other) noexcept = delete;

	bool CheckIntersects(const float4& _Position, const float4& _Direction, float& _Distance);

	bool CheckMeshToPointCollision(const float4& _Position, const float4& _Direction, const float _Range);

public:
	void CreateFBXAnimation(const std::string& _AnimationName, const std::string& _AnimationFBXName, int _AnimationIndex = 0, bool _isLoop = true, float _frameTime = 0.033f);
	void ChangeFBXAnimation(const std::string& _AnimationName, bool _bForceChange = false);
	void OverrideFBXAnimation(const std::string& _animationName, const std::string& _boneNameToAffect, bool _bForceChange = false);
	void ClearOverrideAnimation();

	bool IsCurrentAnimationEnd();
	bool IsOverrideAnimationEnd();

public:
	void SetFBXMesh(const std::string& _Value, std::string _PipeLine, bool _bChar = false);
	void SetFBXMeshRenderSet(const std::string& _Value, std::string _PipeLine, int _MeshIndex);
	void SetFBXMeshRenderSetChar(const std::string& _Value, std::string _PipeLine, int _MeshIndex);


	inline GameEngineFBXMesh* GetMesh() { return FBXMesh; }
	FBXAnimation* GetCurrentAnimation() { return currentAnimation_; }
	const std::string GetCurAnimationName() { return currentAnimation_->AnimationName_; }
	const int GetCurAnimationCurFrame() { return currentAnimation_->CurFrame; }
	const float GetCurAnimationFrameTime() { return currentAnimation_->CurFrameTime; }
	const float GetCurAnimationCurFrameTime() { return currentAnimation_->FrameTime; }
	std::vector<std::string> GetAnimationNameList();
	size_t GetRenderSetCount() { return RenderSets.size(); }
	RenderSet& GetRenderSet(unsigned int _Index) { return RenderSets[_Index]; }
	std::vector<RenderSet>& GetAllRenderSet() { return RenderSets; }
	std::vector<RenderSet> GetAllRenderSetValue() { return RenderSets; }

	float4x4 GetCurrentAffine(int _boneIndex, int _renderSetIndex = 0);
	float4x4 GetParentAffine();

	void SetParentBoneIndex(GameEngineFBXRenderer* _fbxRenderer, int _boneIndex);
	void SetParentBoneName(GameEngineFBXRenderer* _fbxRenderer, const std::string& _boneNameToAffect);
	void SetCustomOffset(const float4& _customOffset) { customOffest_ = _customOffset; }
	void ClearParentBone();

public:
	void SetPreprocessingRenderer(GameEnginePreprocessingRenderer* _PreprocessingRenderer);
	void SetShadowRenderer(GameEngineShadowRenderer* _ShadowRenderer);

public:
	void Start() override;
	void Update(float _DeltaTime) override;
	void Render(float _DeltaTime, bool _IsDeferred) override;

private: // 221102 SJH ADD : 그림자 렌더링
	void LightShadowRender(float _DeltaTime) override;

private:
	std::vector<RenderSet> RenderSets;
	GameEngineFBXMesh* FBXMesh;

	std::map<std::string, FBXAnimation*> Animations;
	FBXAnimation* currentAnimation_;
	FBXAnimation* overrideAnimation_;

	int overrideBoneIndex_;

	std::unordered_map<int, int> overrideBoneIndexCache_;

	GameEngineFBXRenderer* parentBoneRenderer_;
	int parentBoneIndex_;
	float4 customOffest_;
};

