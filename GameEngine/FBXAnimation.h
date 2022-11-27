#pragma once


class FbxExAniData;
class GameEngineFBXRenderer;
class GameEngineFBXMesh;
class GameEngineFBXAnimation;
class FBXAnimation
{
public:
	GameEngineFBXMesh* Mesh;
	GameEngineFBXAnimation* Animation;
	FbxExAniData* PixAniData;
	GameEngineFBXRenderer* ParentRenderer;

	std::string AnimationName_;

	float CurFrameTime;
	float FrameTime;
	UINT CurFrame;
	UINT End;
	UINT Start;
	bool isLoop_;
	bool bEnd_;

	void Init(int _Index, bool _isLoop = true, float _frameTime = 0.033f);
	void Update(float _DeltaTime);
	void UpdateOverride(float _deltaTime, FBXAnimation* _overrideAnimation);
	void ResetFrame();

	float4x4 GetAffine(int _boneIndex, int _renderSetIndex = 0);
};