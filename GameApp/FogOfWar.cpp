#include "PreCompile.h"
#include "FogOfWar.h"

FogOfWar::FogOfWar()
{

}

FogOfWar::~FogOfWar()
{

}

void FogOfWar::Initialize()
{
	CreateResultTarget();
	SetEffect("FogOfWar");
}

void FogOfWar::Effect(float _DeltaTime)
{
	Result_->Clear();
	Result_->Setting();
	Res_.SettingTexture("Target", Target_->GetTexture(0));
	Res_.Setting();
	Effect_->Rendering();
	Effect_->Reset();
	Res_.ReSet();

	Target_->Clear(false);
	Target_->Copy(Result_);
}

void FogOfWar::SetFilter(GameEngineRenderTarget* _renderTarget)
{
	Res_.SettingTexture("Filter", _renderTarget->GetTexture(0));
}
