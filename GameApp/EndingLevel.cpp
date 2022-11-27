#include "Precompile.h"
#include "EndingLevel.h"
#include "EndingUIController.h"
#include <GameEngine/GameEngineUIRenderer.h>

EndingLevel::EndingLevel()
	: UIcontroller_(nullptr), timer_(0.0f)
{
}

EndingLevel::~EndingLevel()
{
}

void EndingLevel::LevelStart()
{
	UIcontroller_ = CreateActor<EndingUIController>();


	state_.CreateState(MakeState(EndingLevel,Idle));
	state_.CreateState(MakeState(EndingLevel, Popup));
	state_.CreateState(MakeState(EndingLevel, Person));
	state_.CreateState(MakeState(EndingLevel, Creditup));
	state_.ChangeState("Idle");
}

void EndingLevel::LevelUpdate(float _DeltaTime)
{
	state_.Update(_DeltaTime);
}

void EndingLevel::LevelChangeEndEvent(GameEngineLevel* _NextLevel)
{
}

void EndingLevel::LevelChangeStartEvent(GameEngineLevel* _PrevLevel)
{
	UIcontroller_->ResourceLoad();
}

void EndingLevel::startIdle()
{
	timer_ = 0.0f;
}

void EndingLevel::updateIdle(float _DeltaTime)
{
	if (1.5 <= timer_)
	{
		timer_ = 0.0f;
		state_.ChangeState("Popup", true);
		return;
	}

	timer_ += _DeltaTime;
}


void EndingLevel::startPopup()
{
	

}

void EndingLevel::updatePopup(float _DeltaTime)
{



	UIcontroller_->GetDeskRenderer()->SetAlpha(timer_);

	if (1.0f <= timer_)
	{
		state_.ChangeState("Person", true);

		return;
	}

	timer_ += _DeltaTime;
}

void EndingLevel::startPerson()
{

	timer_ = 0.0f;
}

void EndingLevel::updatePerson(float _DeltaTime)
{
	UIcontroller_->GetPersonRenderer()->SetAlpha(timer_);

	if (1.0f <= timer_)
	{
		state_.ChangeState("Creditup", true);

		return;
	}

	timer_ += _DeltaTime;
}


void EndingLevel::startCreditup()
{
	timer_ = 0.0f;
	UIcontroller_->GetCreditTextRenderer()->On();
	UIcontroller_->GetFoxteamRenderer()->On();
}

void EndingLevel::updateCreditup(float _DeltaTime)
{
	if (-290.0f <= UIcontroller_->GetFoxteamRenderer()->GetTransform()->GetLocalPosition().y)
	{
		return;
	}


	timer_ += _DeltaTime;

	UIcontroller_->GetCreditTextRenderer()->GetTransform()->SetLocalMove({ 0.0f, 100.0f * _DeltaTime, 0.0f });
	UIcontroller_->GetFoxteamRenderer()->GetTransform()->SetLocalMove({ 0.0f, 100.0f * _DeltaTime, 0.0f });
}
