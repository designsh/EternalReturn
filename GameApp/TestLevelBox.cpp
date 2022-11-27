#include "PreCompile.h"
#include "TestLevelBox.h"

#include <GameEngine/GameEngineRenderer.h>

TestLevelBox::TestLevelBox()
	: renderer_(nullptr)
	, time_(0.0f)
{

}

TestLevelBox::~TestLevelBox()
{

}

void TestLevelBox::SetTransparency(float _transparency)
{
	renderer_->SetAlpha(_transparency);
}

void TestLevelBox::Start()
{
	transform_.SetLocalPosition({ 0.0f, -30.f, 0.0f });
	renderer_ = CreateTransformComponent<GameEngineEffectRenderer>();
	renderer_->SetImage("LoadingLevel_BackDrop.png");
	//renderer_->ShaderHelper.SettingTexture("Tex", "LoadingLevel_BackDrop.png");
	renderer_->GetTransform()->SetLocalScaling({1000, 1000.f});
	renderer_->GetTransform()->SetLocalRotationDegree({ 90.f, 0.0 });
	
	renderer_->SetAlpha(0.5f);
}

void TestLevelBox::Update(float _deltaTime)
{
	//time_ += _deltaTime;
	//renderer_->SetAlpha(abs(sinf(time_)));
}
