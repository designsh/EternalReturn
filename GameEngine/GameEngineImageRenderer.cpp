#include "PreCompile.h"
#include "GameEngineImageRenderer.h"
#include "GameEngineTextureManager.h"
#include "GameEngineFolderTextureManager.h"
#include "GameEngineFolderTexture.h"
#include "GameEngineSamplerManager.h"

void GameEngineImageRenderer::Animation2D::CallStart() 
{
	for (auto& CallBack : StartCallBack_)
	{
		CallBack();
	}
}
void GameEngineImageRenderer::Animation2D::CallEnd() 
{
	for (auto& CallBack : EndCallBack_)
	{
		CallBack();
	}
}
void GameEngineImageRenderer::Animation2D::CallFrame() 
{
	for (auto& CallBack : FrameCallBack_)
	{
		if (CallBack.first != CurFrame_)
		{
			continue;
		}

		if (CallBack.second.size() == 0)
		{
			continue;
		}

		for (size_t i = 0; i < CallBack.second.size(); i++)
		{
			CallBack.second[i]();
		}
	}
}

void GameEngineImageRenderer::Animation2D::Reset() 
{
	IsEnd = false;
	CurTime_ = InterTime_;
	CurFrame_ = StartFrame_;
}

void GameEngineImageRenderer::Animation2D::FrameUpdate()
{
	if (CurTime_ <= 0.0f)
	{
		++CurFrame_;
		CurTime_ = InterTime_;
		if (true == Loop_
			&& CurFrame_ > EndFrame_)
		{
			CallEnd();
			CurFrame_ = StartFrame_;
		}
		else if (false == Loop_
			&& CurFrame_ > EndFrame_)
		{
			if (false == IsEnd)
			{
				CallEnd();
			}

			IsEnd = true;

			CurFrame_ = EndFrame_;
		}
	}

}

void GameEngineImageRenderer::Animation2D::ReverseFrameUpdate()
{
	if (CurTime_ <= 0.0f)
	{
		--CurFrame_;
		CurTime_ = InterTime_;
		if (true == Loop_
			&& CurFrame_ < EndFrame_)
		{
			CallEnd();
			CurFrame_ = StartFrame_;
		}
		else if (false == Loop_
			&& CurFrame_ < EndFrame_)
		{
			if (false == IsEnd)
			{
				CallEnd();
			}

			IsEnd = true;

			CurFrame_ = EndFrame_;
		}
	}

}

void GameEngineImageRenderer::Animation2D::Update(float _DeltaTime)
{
	if (true == Renderer->IsPlay_)
	{
		CurTime_ -= _DeltaTime;
	}

	if (StartFrame_ < EndFrame_)
	{
		FrameUpdate();
	}
	else 
	{
		ReverseFrameUpdate();
	}

	CallFrame();
	if (nullptr == FolderTextures_)
	{
		Renderer->ShaderHelper.SettingTexture("Tex", AnimationTexture_);
		Renderer->CurTexture = AnimationTexture_;
		Renderer->SetIndex(CurFrame_);
	} else 
	{
		Renderer->CutData = float4(0,0,1,1);
		Renderer->ShaderHelper.SettingTexture("Tex", FolderTextures_->GetTextureIndex(CurFrame_));
	}

	

}

/// ///////////////////////////////////////////////////////////////////

GameEngineImageRenderer::GameEngineImageRenderer() 
	: CutData(0, 0, 1, 1)
	, CurAnimation_(nullptr)
{
}

GameEngineImageRenderer::~GameEngineImageRenderer() 
{
	for (auto& Animation : AllAnimations_)
	{
		if (nullptr == Animation.second)
		{
			continue;
		}

		delete Animation.second;
		Animation.second = nullptr;
	}
}

void GameEngineImageRenderer::Start()
{
	GameEngineRenderer::Start();
	SetRenderingPipeLine("TextureAtlas");
}

void GameEngineImageRenderer::SetRenderingPipeLineSettingNext()
{
	ShaderHelper.SettingConstantBufferLink("TextureAtlasData", CutData);

	CorrectResultColor_.vMulColor = float4::ONE;
	//CorrectResultColor_.vPlusColor = float4::ZERO;
	CorrectResultColor_.vPlusColor = float4{0.0f, 0.0f, 0.0f, 0.0f};

	ShaderHelper.SettingConstantBufferLink("TextureAtlasResultColor", CorrectResultColor_);
}


void GameEngineImageRenderer::SetManualConstantBuffer(const std::string& _atlasData, const std::string& _resultColor)
{
	ShaderHelper.SettingConstantBufferLink(_atlasData, CutData);

	CorrectResultColor_.vMulColor = float4::ONE;
	//CorrectResultColor_.vPlusColor = float4::ZERO;
	CorrectResultColor_.vPlusColor = float4{ 0.0f, 0.0f, 0.0f, 0.0f };

	ShaderHelper.SettingConstantBufferLink(_resultColor, CorrectResultColor_);
}


void GameEngineImageRenderer::SetIndex(const int Index) 
{
	if (nullptr == CurTexture)
	{
		GameEngineDebug::MsgBoxError("???????? ???????? ?????? ???????? ?????????? ????????");
	}

	if (false == CurTexture->IsCut())
	{
		GameEngineDebug::MsgBoxError("?????? ???? ???????? ???????? ?????????? ????????.");
	}

	CutData = CurTexture->GetCutData(Index);

}

void GameEngineImageRenderer::SetImage(const std::string& _ImageName, const std::string& _Sampler/* = ""*/)
{
	CurTexture = GameEngineTextureManager::GetInst().Find(_ImageName);

	if (nullptr == CurTexture)
	{
		GameEngineDebug::MsgBoxError("???????? ???? ???????? ?????????? ????????");
		return;
	}

	ShaderHelper.SettingTexture("Tex", _ImageName);
	SetName(_ImageName);

	GameEngineSampler* Sampler = GameEngineSamplerManager::GetInst().Find(_Sampler);

	if (nullptr == Sampler)
	{
		return;
	}

	ShaderHelper.SettingSampler("Smp", _Sampler);

}

void GameEngineImageRenderer::CreateAnimation(const std::string& _TextureName, const std::string& _Name, int _StartFrame, int _EndFrame, float _InterTime, bool _Loop /*= true*/)
{
	std::map<std::string, Animation2D*>::iterator FindIter = AllAnimations_.find(_Name);

	if (AllAnimations_.end() != FindIter)
	{
		GameEngineDebug::MsgBoxError("???? ???????? ???????????? ?? ????????????.");
	}

	Animation2D* NewAnimation = new Animation2D();

	NewAnimation->AnimationTexture_ = GameEngineTextureManager::GetInst().Find(_TextureName);

	if (nullptr == NewAnimation->AnimationTexture_)
	{
		GameEngineDebug::MsgBoxError("???????? ???? ???????? ???????????? ???????? ????????.");
	}

	NewAnimation->SetName(_Name);
	NewAnimation->IsEnd = false;
	NewAnimation->Loop_ = _Loop;
	NewAnimation->InterTime_ = _InterTime;
	NewAnimation->CurTime_ = _InterTime;

	NewAnimation->FolderTextures_ = nullptr;
	NewAnimation->CurFrame_ = _StartFrame;
	NewAnimation->EndFrame_ = _EndFrame;
	NewAnimation->StartFrame_ = _StartFrame;
	NewAnimation->Renderer = this;


	AllAnimations_.insert(std::map<std::string, Animation2D*>::value_type(_Name, NewAnimation));
}

void GameEngineImageRenderer::CreateAnimationFolder(const std::string& _FolderTexName, const std::string& _Name, float _InterTime, bool _Loop /*= true*/)
{
	std::map<std::string, Animation2D*>::iterator FindIter = AllAnimations_.find(_Name);

	if (AllAnimations_.end() != FindIter)
	{
		GameEngineDebug::MsgBoxError("???? ???????? ???????????? ?? ????????????.");
	}

	GameEngineFolderTexture* FolderTexture = GameEngineFolderTextureManager::GetInst().Find(_FolderTexName);
	
	if (nullptr == FolderTexture)
	{
		GameEngineDebug::MsgBoxError("???????? ???? ???? ???????? ?????????? ????????..");
	}


	Animation2D* NewAnimation = new Animation2D();

	NewAnimation->SetName(_Name);
	NewAnimation->IsEnd = false;
	NewAnimation->Loop_ = _Loop;
	NewAnimation->InterTime_ = _InterTime;
	NewAnimation->CurTime_ = _InterTime;

	NewAnimation->FolderTextures_ = FolderTexture;
	NewAnimation->CurFrame_ = 0;
	NewAnimation->EndFrame_ = FolderTexture->GetTextureCount() - 1;
	NewAnimation->StartFrame_ = 0;
	NewAnimation->Renderer = this;

	AllAnimations_.insert(std::map<std::string, Animation2D*>::value_type(_Name, NewAnimation));
}

void GameEngineImageRenderer::SetChangeAnimation(const std::string& _Name, bool _IsForce /*= false*/)
{
	std::map<std::string, Animation2D*>::iterator FindIter = AllAnimations_.find(_Name);

	if (AllAnimations_.end() == FindIter)
	{
		GameEngineDebug::MsgBoxError("???????? ???? ???????????? ?????????? ????????");
	}

	if (nullptr == FindIter->second)
	{
		GameEngineDebug::MsgBoxError("???????????? ?????????? nullptr ??????");
	}

	if (false == _IsForce && CurAnimation_ == FindIter->second)
	{
		return;
	}
	
	CurAnimation_ = FindIter->second;
	if (nullptr == CurAnimation_->FolderTextures_)
	{
		ShaderHelper.SettingTexture("Tex", CurAnimation_->AnimationTexture_);
	}
	else
	{
		ShaderHelper.SettingTexture("Tex", CurAnimation_->FolderTextures_->GetTextureIndex(CurAnimation_->CurFrame_));
	}

	CurAnimation_->Reset();
	CurAnimation_->CallStart();
	AnimationPlay();
}

void GameEngineImageRenderer::Update(float _DeltaTime)
{
	if (nullptr == CurAnimation_)
	{
		return;
	}

	CurAnimation_->Update(_DeltaTime);
}

void GameEngineImageRenderer::SetStartCallBack(const std::string& _Name, std::function<void()> _CallBack)
{
	std::map<std::string, Animation2D*>::iterator FindIter = AllAnimations_.find(_Name);

	if (AllAnimations_.end() == FindIter)
	{
		GameEngineDebug::MsgBoxError("???????? ???? ???????????? ?????????? ????????");
	}

	if (nullptr == FindIter->second)
	{
		GameEngineDebug::MsgBoxError("???????????? ?????????? nullptr ??????");
	}

	FindIter->second->StartCallBack_.push_back(_CallBack);
}
void GameEngineImageRenderer::SetEndCallBack(const std::string& _Name, std::function<void()> _CallBack)
{
	std::map<std::string, Animation2D*>::iterator FindIter = AllAnimations_.find(_Name);

	if (AllAnimations_.end() == FindIter)
	{
		GameEngineDebug::MsgBoxError("???????? ???? ???????????? ?????????? ????????");
	}

	if (nullptr == FindIter->second)
	{
		GameEngineDebug::MsgBoxError("???????????? ?????????? nullptr ??????");
	}

	FindIter->second->EndCallBack_.push_back(_CallBack);
}
void GameEngineImageRenderer::SetFrameCallBack(const std::string& _Name, int _Index, std::function<void()> _CallBack)
{
	std::map<std::string, Animation2D*>::iterator FindIter = AllAnimations_.find(_Name);

	if (AllAnimations_.end() == FindIter)
	{
		GameEngineDebug::MsgBoxError("???????? ???? ???????????? ?????????? ????????");
	}

	if (nullptr == FindIter->second)
	{
		GameEngineDebug::MsgBoxError("???????????? ?????????? nullptr ??????");
	}

	FindIter->second->FrameCallBack_[_Index].push_back(_CallBack);
}

bool GameEngineImageRenderer::CheckAnimation(const std::string& _Name)
{
	std::map<std::string, Animation2D*>::iterator FindIter = AllAnimations_.find(_Name);
	if (AllAnimations_.end() == FindIter)
	{
		return false;
	}

	return true;
}
