#include "PreCompile.h"
#include "UI_FOW.h"

UI_FOW::UI_FOW() // default constructer 디폴트 생성자
	: timer_(0.0f), fowRenderer_(nullptr), isNight_(false), ambientRenderer_(nullptr)
{

}

UI_FOW::~UI_FOW() // default destructer 디폴트 소멸자
{

}

UI_FOW::UI_FOW(UI_FOW&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}

void UI_FOW::SetFOW(bool _b)
{
	if (true == _b)
	{
		timer_ = 0.0f;
		UIState_.ChangeState("ToNight", true);
		return;
	}
	else if (false == _b)
	{
		timer_ = 1.0f;
		UIState_.ChangeState("ToMorning", true);
		return;
	}
}

void UI_FOW::Start()
{
	//ambientRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	//ambientRenderer_->SetImage("nightshade.png", "PointSmp");
	//ambientRenderer_->GetTransform()->SetLocalScaling(ambientRenderer_->GetCurrentTexture()->GetTextureSize());


	fowRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	fowRenderer_->SetImage("nightshade.png", "LinerSmp");
	fowRenderer_->GetTransform()->SetLocalScaling(fowRenderer_->GetCurrentTexture()->GetTextureSize());
	fowRenderer_->SetAlpha(0.0f);
	fowRenderer_->Off();

	UIState_.CreateState(MakeState(UI_FOW, ToMorning));
	UIState_.CreateState(MakeState(UI_FOW, ToNight));

	UIState_ << "ToMorning";
}

void UI_FOW::Update(float _DeltaTime)
{
	UIState_.Update(_DeltaTime);
}

void UI_FOW::startToMorning()
{
	timer_ = 1.0f;
	fowRenderer_->On();
	fowRenderer_->SetAlpha(1.0f);
}

void UI_FOW::updateToMorning(float _DeltaTime)
{
	if (0.0f >= timer_)
	{
		return;
	}
	
	fowRenderer_->SetAlpha(timer_);
	
	timer_ -= _DeltaTime;
}

void UI_FOW::startToNight()
{
	timer_ = 0.0f;
	fowRenderer_->On();
	fowRenderer_->SetAlpha(0.0f);
}

void UI_FOW::updateToNight(float _DeltaTime)
{
	if (1.0f <= timer_)
	{
		return;
	}

	
	fowRenderer_->SetAlpha(timer_);

	timer_ += _DeltaTime;
}
