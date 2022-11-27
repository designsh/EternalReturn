#include "PreCompile.h"
#include "UI_WinLose.h"

#include "Enums.h"
#include <GameEngine/GameEngineUIRenderer.h>
#include <GameEngine/GameEngineCollision.h>
#include <GameEngine/GameEngineCore.h>

UI_WinLose::UI_WinLose() // default constructer 디폴트 생성자
	: portraitRenderer_(nullptr), backRenderer_(nullptr), textRenderer_(nullptr), appearTimer_(0.0f), isAllAppeared_(false), isActivated_(false), 
	bloodStainRendererUp_(nullptr), bloodStainRenderer_(nullptr), isWinner_(false), deathCheckRenderer_(nullptr)
{

}

UI_WinLose::~UI_WinLose() // default destructer 디폴트 소멸자
{

}

UI_WinLose::UI_WinLose(UI_WinLose&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}

void UI_WinLose::SetPortrait(JobType _jobtype, bool _isWin)
{

	switch (_jobtype)
	{
	case JobType::NONE:
		break;
	case JobType::YUKI:
		if (true == _isWin)
		{
			portraitRenderer_->SetImage("Lobby_CharFull_Yuki.png", "PointSmp");
			break;
		}
		portraitRenderer_->SetImage("Death_CharFull_Yuki.png", "PointSmp");
		break;
	//case JobType::FIORA:
	//	break;
	//case JobType::ZAHIR:
	//	break;
	//case JobType::NADINE:
	//	break;
	case JobType::HYUNWOO:
		if (true == _isWin)
		{
			portraitRenderer_->SetImage("Lobby_CharFull_Hyunwoo.png", "PointSmp");
			break;
		}
		portraitRenderer_->SetImage("Death_CharFull_Hyunwoo.png", "PointSmp");
		break;
	case JobType::JACKIE:
		if (true == _isWin)
		{
			portraitRenderer_->SetImage("Lobby_CharFull_Jackie.png", "PointSmp");
			break;
		}
		portraitRenderer_->SetImage("Death_CharFull_Jackie.png", "PointSmp");
		break;
	case JobType::RIO:
		if (true == _isWin)
		{
			portraitRenderer_->SetImage("Lobby_CharFull_Rio.png", "PointSmp");
			break;
		}
		portraitRenderer_->SetImage("Death_CharFull_Rio.png", "PointSmp");
		break;
	case JobType::AYA:
		if (true == _isWin)
		{
			portraitRenderer_->SetImage("Lobby_CharFull_Aya.png", "PointSmp");
			break;
		}
		portraitRenderer_->SetImage("Death_CharFull_Aya.png", "PointSmp");
		break;
		
		break;
	case JobType::DUMMY:
		break;
	case JobType::MAX:
		break;
	default:
		break;
	}
}


void UI_WinLose::SetText(const std::string& _text)
{
	textRenderer_->TextSetting("굴림", _text, 20, FW1_CENTER);
}

void UI_WinLose::SetWinner()
{
	winLosePanelRenderer_->SetImage("winPanel.png", "PointSmp");
	isWinner_ = true;
}

bool UI_WinLose::MouseCollisionCheck()
{
	return exitBtnCollision_->Collision(static_cast<int>(eCollisionGroup::MousePointer));;
}

void UI_WinLose::SetImageByIndex(int _index)
{
	switch (_index)
	{
	case 0:
		exitBtnRenderer_->SetImage("Btn_Accept_Out.png", "PointSmp");
		break;
	case 1:
		exitBtnRenderer_->SetImage("Btn_Accept_OutClick.png", "PointSmp");
		break;
	default:
		break;
	}
}

void UI_WinLose::Start()
{

	backRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	backRenderer_->SetImage("Img_Result_PortraitBg.png", "PointSmp");
	backRenderer_->GetTransform()->SetLocalScaling(backRenderer_->GetCurrentTexture()->GetTextureSize() * 2);
	backRenderer_->SetAlpha(0.0f);
	backRenderer_->Off();

	portraitRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::UIPANEL1);
	portraitRenderer_->SetImage("Death_CharFull_Hyunwoo.png", "PointSmp");
	//	portraitRenderer_->GetTransform()->SetLocalPosition();
	portraitRenderer_->GetTransform()->SetLocalScaling(portraitRenderer_->GetCurrentTexture()->GetTextureSize());
	portraitRenderer_->SetAlpha(0.0f);
	portraitRenderer_->Off();


	GameEngineTexture* hitBase = GameEngineTextureManager::GetInst().Find("deathcheck.png");
	hitBase->Cut(4, 2);
	deathCheckRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::UIPANEL2);
	deathCheckRenderer_->SetImage("deathcheck.png", "PointSmp");
	deathCheckRenderer_->GetTransform()->SetLocalScaling({512.0f, 512.0f});
	deathCheckRenderer_->CreateAnimation("deathcheck.png", "deathcheck", 0, 7, 0.03f, false);
	deathCheckRenderer_->SetChangeAnimation("deathcheck", true);
	deathCheckRenderer_->Off();


	bloodStainRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	bloodStainRenderer_->GetTransform()->SetLocalPosition({ -550.0f, -250.0f, 0.0f });
	bloodStainRenderer_->SetImage("FX_UI_Bloodstain.png", "PointSmp");
	bloodStainRenderer_->GetTransform()->SetLocalScaling(bloodStainRenderer_->GetCurrentTexture()->GetTextureSize());
	bloodStainRenderer_->SetAlpha(0.0f);
	bloodStainRenderer_->Off();

	bloodStainRendererUp_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	bloodStainRendererUp_->GetTransform()->SetLocalPosition({ 550.0f, 250.0f, 0.0f });
	bloodStainRendererUp_->SetImage("FX_UI_Bloodstain2.png", "PointSmp");
	bloodStainRendererUp_->GetTransform()->SetLocalScaling(bloodStainRenderer_->GetCurrentTexture()->GetTextureSize());
	bloodStainRendererUp_->SetAlpha(0.0f);
	bloodStainRendererUp_->Off();

	winLosePanelRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::UIPANEL2);
	winLosePanelRenderer_->SetImage("losePanel.png", "PointSmp");
	winLosePanelRenderer_->GetTransform()->SetLocalScaling(winLosePanelRenderer_->GetCurrentTexture()->GetTextureSize());
	winLosePanelRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 220.0f, 0.0f });
	winLosePanelRenderer_->SetAlpha(0.0f);
	winLosePanelRenderer_->Off();

	winLoseTextRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::FONT);
	winLoseTextRenderer_->SetTextColor(float4::WHITE);
	winLoseTextRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 250.0f, 0.0f });
	winLoseTextRenderer_->TextSetting("굴림", "실험 종료", 40, FW1_CENTER);
	winLoseTextRenderer_->SetAlpha(0.0f);
	winLoseTextRenderer_->Off();


	rankPanelRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::UIPANEL2);
	rankPanelRenderer_->SetImage("rankPanel.png", "PointSmp");
	rankPanelRenderer_->GetTransform()->SetLocalScaling(rankPanelRenderer_->GetCurrentTexture()->GetTextureSize());
	rankPanelRenderer_->GetTransform()->SetLocalPosition({ 0.0f,-240.0f,0.0f });
	rankPanelRenderer_->SetAlpha(0.0f);
	rankPanelRenderer_->Off();

	textRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::FONT);
	textRenderer_->SetTextColor(float4::WHITE);
	textRenderer_->TextSetting("굴림", " ", 40, FW1_CENTER);
	textRenderer_->GetTransform()->SetLocalPosition({ 0.0f,-220.0f,0.0f });
	textRenderer_->SetAlpha(0.0f);
	textRenderer_->Off();

	exitBtnRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	exitBtnRenderer_->SetImage("Btn_Accept_Out.png", "PointSmp");
	exitBtnRenderer_->GetTransform()->SetLocalScaling(exitBtnRenderer_->GetCurrentTexture()->GetTextureSize());
	exitBtnRenderer_->GetTransform()->SetLocalPosition({ 500.0f, -300.0f, 0.0f });
	exitBtnRenderer_->SetAlpha(0.0f);
	exitBtnRenderer_->Off();

	exitBtnCollision_ = CreateTransformComponent<GameEngineCollision>(GetTransform());
	exitBtnCollision_->GetTransform()->SetLocalPosition(exitBtnRenderer_->GetTransform()->GetLocalPosition());
	exitBtnCollision_->GetTransform()->SetLocalScaling(exitBtnRenderer_->GetTransform()->GetLocalScaling());
	exitBtnCollision_->SetCollisionInfo(static_cast<int>(eCollisionGroup::UI), CollisionType::Rect);

	UIstate_.CreateState(MakeState(UI_WinLose, NoAppear));
	UIstate_.CreateState(MakeState(UI_WinLose, Interval));
	UIstate_.CreateState(MakeState(UI_WinLose, Appear));
	UIstate_.CreateState(MakeState(UI_WinLose, StandBy));

	UIstate_ << "NoAppear";
}

void UI_WinLose::Update(float _DeltaTime)
{
	UIstate_.Update(_DeltaTime);
}

void UI_WinLose::startNoAppear()
{
	appearTimer_ = 0.0f;
	isActivated_ = false;

}

void UI_WinLose::updateNoAppear(float _DeltaTime)
{
	if (true == isActivated_)
	{
		UIstate_.ChangeState("Interval", true);
		return;
	}
}

void UI_WinLose::startInterval()
{
	intervalTimer_ = 0.0f;
}

void UI_WinLose::updateInterval(float _DeltaTime)
{
	intervalTimer_ += _DeltaTime;

	if (intervalTimer_ >= 2.0f)
	{
		intervalTimer_ = 0.0f;
		UIstate_.ChangeState("Appear", true);
		return;
	}
}

void UI_WinLose::startAppear()
{
	portraitRenderer_->On();
	backRenderer_->On();
	winLoseTextRenderer_->On();
	textRenderer_->On();

	if (false == isWinner_)
	{
		bloodStainRenderer_->On();
		bloodStainRendererUp_->On();
		GameEngineSoundManager::GetInstance()->PlaySoundByName("Announce_GameLose_02.wav");
	}
	else if (true == isWinner_)
	{
		GameEngineSoundManager::GetInstance()->PlaySoundByName("Announce_Win_Game_02.wav");
	}

	winLosePanelRenderer_->On();
	rankPanelRenderer_->On();
	exitBtnRenderer_->On();
}

void UI_WinLose::updateAppear(float _DeltaTime)
{


	appearTimer_ += _DeltaTime;

	float adjustedRatio = appearTimer_ / TIME_APPEAR_DEFAULT;
	if (adjustedRatio >= 1.0f)
	{
		if (false == isWinner_)
		{
			GameEngineSoundManager::GetInstance()->PlaySoundByName("Bernice_Skill02_Attack.wav");
			deathCheckRenderer_->On();
			deathCheckRenderer_->SetChangeAnimation("deathcheck", true);
		}
		UIstate_.ChangeState("StandBy", true);
		return;
	}

	
	portraitRenderer_->SetAlpha(adjustedRatio);
	backRenderer_->SetAlpha(adjustedRatio);
	winLoseTextRenderer_->SetAlpha(adjustedRatio);
	textRenderer_->SetAlpha(adjustedRatio);
	bloodStainRenderer_->SetAlpha(adjustedRatio);
	bloodStainRendererUp_->SetAlpha(adjustedRatio);
	winLosePanelRenderer_->SetAlpha(adjustedRatio);
	rankPanelRenderer_->SetAlpha(adjustedRatio);
	exitBtnRenderer_->SetAlpha(adjustedRatio);
}

void UI_WinLose::startStandBy()
{

}

void UI_WinLose::updateStandBy(float _DeltaTime)
{
	if (true == MouseCollisionCheck())
	{

		int tmp = 0;

		if (true == GameEngineInput::GetInst().Press("LBUTTON"))
		{
			SetImageByIndex(1);
			
		}
		else if (true == GameEngineInput::GetInst().Up("LBUTTON"))
		{
			SetImageByIndex(0);

			GameEngineSoundManager::GetInstance()->PlaySoundByName("oui_btnClick_v1.wav");

			GameEngineCore::LevelChange("EndingLevel");
		}
	}

}

