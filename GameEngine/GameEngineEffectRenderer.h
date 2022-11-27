#pragma once
#include "GameEngineRenderer.h"
#include <GameEngineBase\GameEngineObjectNameBase.h>

struct CorrectEffectResultColor
{
	float4 vMulColor;
	float4 vPlusColor;
};

class GameEngineTexture;
class GameEngineFolderTexture;
class GameEngineEffectRenderer : public GameEngineRenderer
{
private:
	struct Animation2D : public GameEngineObjectNameBase
	{
		GameEngineFolderTexture* FolderTextures_;
		GameEngineTexture* AnimationTexture_;
		float InterTime_;
		float CurTime_;

		bool IsEnd;
		bool Loop_;
		int CurFrame_;
		int StartFrame_;
		int EndFrame_;

		GameEngineEffectRenderer* Renderer;

		std::map<int, std::vector<std::function<void()>>> FrameCallBack_;
		std::vector<std::function<void()>> EndCallBack_;
		std::vector<std::function<void()>> StartCallBack_;

	public:
		void SetCurrentIndex(int _Index)
		{
			CurFrame_ = _Index;
		}

		void Reset();
		void CallStart();
		void CallEnd();
		void CallFrame();
		void Update(float _DeltaTime);

		void FrameUpdate();
		void ReverseFrameUpdate();
	};

public:
	// constrcuter destructer
	GameEngineEffectRenderer();
	~GameEngineEffectRenderer();

	// delete Function
	GameEngineEffectRenderer(const GameEngineEffectRenderer& _Other) = delete;
	GameEngineEffectRenderer(GameEngineEffectRenderer&& _Other) noexcept = delete;
	GameEngineEffectRenderer& operator=(const GameEngineEffectRenderer& _Other) = delete;
	GameEngineEffectRenderer& operator=(GameEngineEffectRenderer&& _Other) noexcept = delete;

	void SetImage(const std::string& _ImageName, const std::string& _Sampler = "");

	void SetIndex(const int Index);



	void CreateAnimation(const std::string& _TextureName, const std::string& _Name, int _StartFrame, int _EndFrame, float _InterTime, bool _Loop = true);

	void CreateAnimationFolder(const std::string& _FolderTexName, const std::string& _Name, float _InterTime, bool _Loop = true);

	// _IsForce 같은 애니메이션 인데도 바꿀거냐? true 바꾼다.
	void SetChangeAnimation(const std::string& _Name, bool _IsForce = false);

	void SetStartCallBack(const std::string& _Name, std::function<void()> _CallBack);
	void SetEndCallBack(const std::string& _Name, std::function<void()> _CallBack);
	void SetFrameCallBack(const std::string& _Name, int _Index, std::function<void()> _CallBack);



	void SetManualConstantBuffer(const std::string& _atlasData, const std::string& _resultColor);

	bool CheckAnimation(const std::string& _Name);


	inline GameEngineTexture* GetCurrentTexture()
	{
		return CurTexture;
	}

	inline std::string GetCurrentAnimationName()
	{
		return CurAnimation_->GetName();
	}

	inline void IsCurrentAnimationIndex(const int _Index)
	{
		CurAnimation_->SetCurrentIndex(_Index);
	}


	inline bool IsCurrentAnimationString(const std::string& _Name)
	{
		return CurAnimation_->GetName() == _Name;
	}

	inline bool IsCurrentAnimationPtr(const char* _Name)
	{
		return CurAnimation_->GetName() == _Name;
	}

	inline bool IsCurAnimationEnd()
	{
		if (nullptr == CurAnimation_)
		{
			return false;
		}

		return CurAnimation_->IsEnd;
	}

	inline void SetColor(float4 _RGB)
	{
		CorrectResultColor_.vMulColor.r = _RGB.r;
		CorrectResultColor_.vMulColor.g = _RGB.g;
		CorrectResultColor_.vMulColor.b = _RGB.b;
	}

	inline void SetAlpha(float _Value)
	{
		CorrectResultColor_.vMulColor.a = _Value;
	}

	inline float GetAlpha()
	{
		return CorrectResultColor_.vMulColor.a;
	}

	inline void SetPlusColor(float4 _Color)
	{
		CorrectResultColor_.vPlusColor = _Color;
	}

	inline void AnimationStop()
	{
		IsPlay_ = false;
	}

	inline void AnimationPlay()
	{
		IsPlay_ = true;
	}

protected:
	void SetRenderingPipeLineSettingNext() override;

	void Start() override;
	void Update(float _DeltaTime) override;

private:
	std::map<std::string, Animation2D*> AllAnimations_;
	Animation2D* CurAnimation_;

	CorrectEffectResultColor CorrectResultColor_;
	float4 CutData;
	GameEngineTexture* CurTexture;
	bool IsPlay_;
};

