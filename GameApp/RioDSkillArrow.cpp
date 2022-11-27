#include "PreCompile.h"
#include "RioDSkillArrow.h"
#include <GameEngine/GameEngineEffectRenderer.h>
RioDSkillArrow::RioDSkillArrow() // default constructer 디폴트 생성자
	: arrowRenderer_(nullptr), speed_(1200.0f)
{

}

RioDSkillArrow::~RioDSkillArrow() // default destructer 디폴트 소멸자
{

}

RioDSkillArrow::RioDSkillArrow(RioDSkillArrow&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}

void RioDSkillArrow::PlayAwake(float _speed, float4 _localPosition)
{
	//speed_ = _speed;


	//arrowRenderer_->GetTransform()->SetLocalPosition(_localPosition);


	renderState_ << "Awake";

}

void RioDSkillArrow::PlaySleep()
{
//	speed_ = 0.0f;
	renderState_ << "Sleep";
}

void RioDSkillArrow::Start()
{
	arrowRenderer_ = CreateTransformComponent<GameEngineFBXRenderer>(/*static_cast<int>(ObjectRenderOrder::CHARACTER)*/);
	arrowRenderer_->SetFBXMesh("Rio_000_Arrow.fbx", "TextureDeferredLight", false);


	float randomX = random_.RandomFloat(0.0f, 590.0f);
	float randomZ = random_.RandomFloat(-320.0f, 320.0f);
	float randomSpeed = random_.RandomFloat(6000.0f, 8400.0f);

	speed_ = randomSpeed;
	arrowRenderer_->GetTransform()->SetLocalPosition({ randomX, 1000.0f, randomZ });
	arrowRenderer_->GetTransform()->SetLocalScaling({ 300.0f, 300.0f, 300.0f });
	arrowRenderer_->GetTransform()->SetLocalRotationDegree({ 0.f,0.0f,0.0f });
	arrowRenderer_->Off();

	shotEffect_ = CreateTransformComponent<GameEngineEffectRenderer>();
	shotEffect_->SetImage("rio_dskillTarget.png", "LinerSmp");
	shotEffect_->GetTransform()->SetLocalPosition({arrowRenderer_->GetTransform()->GetLocalPosition().x + 30.0f, 10.0f, arrowRenderer_->GetTransform()->GetLocalPosition().z});
	shotEffect_->GetTransform()->SetLocalRotationDegree({ -90.0f, 0.0f, 0.0f });
	shotEffect_->GetTransform()->SetLocalScaling({30.0f, 30.0f});
	shotEffect_->SetColor({ 1.0f, 1.0f, 0.1f });
	shotEffect_->Off();

	renderState_.CreateState(MakeState(RioDSkillArrow, Sleep));
	renderState_.CreateState(MakeState(RioDSkillArrow, Awake));
	renderState_.CreateState(MakeState(RioDSkillArrow, Fade));
	renderState_ << "Sleep";
}

void RioDSkillArrow::Update(float _deltaTime)
{
	renderState_.Update(_deltaTime);
}

void RioDSkillArrow::startSleep()
{
	arrowRenderer_->Off();
	shotEffect_->Off();
	shotEffect_->GetTransform()->SetLocalScaling({ 30.0f ,30.0f });
	timer_ = 0.0f;
	GetTransform()->SetLocalPosition({ 0.0f, 0.0f, 0.0f });

	scaleContainer_ = { 0.0f, 0.0f };
}

void RioDSkillArrow::updateSleep(float _deltaTime)
{

}

void RioDSkillArrow::startAwake()
{
	float randomX = random_.RandomFloat(-220.0f, 420.0f);
	float randomZ = random_.RandomFloat(-320.0f, 320.0f);

	arrowRenderer_->GetTransform()->SetLocalPosition({ randomX, 1000.0f, randomZ });
	shotEffect_->GetTransform()->SetLocalPosition({ arrowRenderer_->GetTransform()->GetLocalPosition().x - 180.0f, 10.0f, arrowRenderer_->GetTransform()->GetLocalPosition().z });
	float randomSpeed = random_.RandomFloat(6000.0f, 8400.0f);

	speed_ = randomSpeed;
	arrowRenderer_->On();
	// 위치 설정
	timer_ = 0.0f;
}

void RioDSkillArrow::updateAwake(float _deltaTime)
{
	arrowRenderer_->GetTransform()->SetLocalMove({ 0.0f, -1 * speed_ * _deltaTime, 0.0f });

	if (arrowRenderer_->GetTransform()->GetLocalPosition().y <= -400.0f )
	{
		// 지상에 거의 닿으면 페이드
		renderState_ << "Fade";
		return;
	}
}

void RioDSkillArrow::startFade()
{
	timer_ = 0.8f;
	shotEffect_->On();
}

void RioDSkillArrow::updateFade(float _deltaTime)
{
	timer_ -= _deltaTime;

	if (0.0f >= timer_)
	{
		timer_ = 0.0f;
		arrowRenderer_->Off();

		this->Release();
		renderState_ << "Sleep";
	}

	scaleContainer_ += { 3600.0f * _deltaTime, 3600.0f * _deltaTime };
	shotEffect_->SetAlpha(timer_/1.2f);
	shotEffect_->GetTransform()->SetLocalScaling({ 30.0f + scaleContainer_.x, 30.0f + scaleContainer_.y });

}

