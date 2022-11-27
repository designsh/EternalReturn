#include "Precompile.h"
#include "Lobby_CharFull.h"
#include "GameEngine/GameEngineInput.h"
#include "GameEngine/GameEngineCollision.h"
#include "GameEngineBase/GameEngineDebug.h"
#include "LobbyUIController.h"

Lobby_CharFull::Lobby_CharFull()
{
}

Lobby_CharFull::~Lobby_CharFull()
{
}

void Lobby_CharFull::SetChar(JobType _Character)
{

}

void Lobby_CharFull::Start()
{
	CharFull_Renderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), static_cast<int>(UIRenderOrder::UI));
	CharFull_Renderer->SetImage("Lobby_CharFull_Hyunwoo.png", "PointSmp");
	CharFull_Renderer->GetTransform()->SetLocalPosition({ 0.0f, 125.0f });
	CharFull_Renderer->GetTransform()->SetLocalScaling(CharFull_Renderer->GetCurrentTexture()->GetTextureSize());
	CharFull_Renderer->Off();

	qDescription_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), static_cast<int>(UIRenderOrder::UI));
	qDescription_->SetImage("Yuki_Q_Icon.png", "PointSmp");
	qDescription_->GetTransform()->SetLocalPosition({ -300.0f, -200.0f });
	qDescription_->GetTransform()->SetLocalScaling(qDescription_->GetCurrentTexture()->GetTextureSize() * 0.4f);
	qDescription_->Off();

	wDescription_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), static_cast<int>(UIRenderOrder::UI));
	wDescription_->SetImage("Yuki_Q_Icon.png", "PointSmp");
	wDescription_->GetTransform()->SetLocalPosition({ 170.0f, -200.0f });
	wDescription_->GetTransform()->SetLocalScaling(wDescription_->GetCurrentTexture()->GetTextureSize() * 0.4f);
	wDescription_->Off();

	eDescription_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), static_cast<int>(UIRenderOrder::UI));
	eDescription_->SetImage("Yuki_Q_Icon.png", "PointSmp");
	eDescription_->GetTransform()->SetLocalPosition({ -300.0f, -250.0f });
	eDescription_->GetTransform()->SetLocalScaling(eDescription_->GetCurrentTexture()->GetTextureSize() * 0.4f);
	eDescription_->Off();

	rDescription_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), static_cast<int>(UIRenderOrder::UI));
	rDescription_->SetImage("Yuki_Q_Icon.png", "PointSmp");
	rDescription_->GetTransform()->SetLocalPosition({ 170.0f, -250.0f });
	rDescription_->GetTransform()->SetLocalScaling(rDescription_->GetCurrentTexture()->GetTextureSize() * 0.4f);
	rDescription_->Off();
}

void Lobby_CharFull::Update(float _DeltaTime)
{
	switch (LobbyUIController::SelectedChar)
	{
	case JobType::NONE:
		CharFull_Renderer->Off();
		break;
	case JobType::YUKI:
		CharFull_Renderer->SetImage("Lobby_CharFull_Yuki.png", "PointSmp");
		CharFull_Renderer->TextSetting("굴림", "유키", 40.0f, FW1_CENTER, {0.85f, 0.85f, 1.0f, 1.0f}, {0.0f, -240.0f});
		CharFull_Renderer->On();

		qDescription_->SetImage("Yuki_Q_Icon.png", "PointSmp");
		qDescription_->TextSetting("굴림", "Q : 다음 공격이 추가 피해를 입힙니다.\n     적중 시 적의 이동속도를 감소시킵니다.", 16.0f, FW1_LEFT, { 0.85f, 0.85f, 1.0f, 1.0f }, { 40.0f, 20.0f });
		qDescription_->On();

		wDescription_->SetImage("Yuki_W_Icon.png", "PointSmp");
		wDescription_->TextSetting("굴림", "W : 사용 시 E의 쿨타임이 3초 감소합니다.\n     방어력이 50% 증가합니다.", 16.0f, FW1_LEFT, { 0.85f, 0.85f, 1.0f, 1.0f }, { 40.0f, 20.0f });
		wDescription_->On();

		eDescription_->SetImage("Yuki_E_Icon.png", "PointSmp");
		eDescription_->TextSetting("굴림", "E : 돌진하여 부딪힌 적에게 피해를 입힙니다.\n     적중 시 쿨타임이 3초 감소합니다.", 16.0f, FW1_LEFT, { 0.85f, 0.85f, 1.0f, 1.0f }, { 40.0f, 20.0f });
		eDescription_->On();

		rDescription_->SetImage("Yuki_R_Icon.png", "PointSmp");
		rDescription_->TextSetting("굴림", "R : 넓은 범위 공격을 가합니다.\n     후속타는 최대 체력 비례 고정 피해를 입힙니다.", 16.0f, FW1_LEFT, { 0.85f, 0.85f, 1.0f, 1.0f }, { 40.0f, 20.0f });
		rDescription_->On();
		break;
	//case JobType::FIORA:
	//	CharFull_Renderer->Off();
	//	break;
	//case JobType::ZAHIR:
	//	CharFull_Renderer->Off();
	//	break;
	//case JobType::NADINE:
	//	CharFull_Renderer->On();
	//	CharFull_Renderer->SetImage("Lobby_CharFull_Nadine.png", "PointSmp");
	//	CharFull_Renderer->TextSetting("굴림", "나딘", 40.0f, FW1_CENTER, { 0.85f, 0.85f, 1.0f, 1.0f }, { 0.0f, -240.0f });
	//	CharFull_Renderer->On();
	//
	//	qDescription_->SetImage("Nadine_Q_Icon.png", "PointSmp");
	//	qDescription_->TextSetting("굴림", "Q : 힘을 모아 화살을 발사합니다.\n     모을수록 공격력과 사거리가 증가합니다.", 16.0f, FW1_LEFT, { 0.85f, 0.85f, 1.0f, 1.0f }, { 40.0f, 20.0f });
	//	qDescription_->On();
	//
	//	wDescription_->SetImage("Nadine_W_Icon.png", "PointSmp");
	//	wDescription_->TextSetting("굴림", "W : 덫을 설치해 경로를 봉쇄하고 피해를 입힙니다.\n     덫에 걸리면 이동속도가 감소합니다.", 16.0f, FW1_LEFT, { 0.85f, 0.85f, 1.0f, 1.0f }, { 40.0f, 20.0f });
	//	wDescription_->On();
	//
	//	eDescription_->SetImage("Nadine_E_Icon.png", "PointSmp");
	//	eDescription_->TextSetting("굴림", "E : 지정한 위치에 와이어를 발사합니다.\n     해당 방향으로 빠르게 타고 이동할 수 있습니다.", 16.0f, FW1_LEFT, { 0.85f, 0.85f, 1.0f, 1.0f }, { 40.0f, 20.0f });
	//	eDescription_->On();
	//
	//	rDescription_->SetImage("Nadine_R_Icon.png", "PointSmp");
	//	rDescription_->TextSetting("굴림", "R : 매 3번째 공격마다 늑대를 소환해 적을 공격합니다.", 16.0f, FW1_LEFT, { 0.85f, 0.85f, 1.0f, 1.0f }, { 40.0f, 20.0f });
	//	rDescription_->On();
	//	break;
	case JobType::HYUNWOO:
		CharFull_Renderer->On();
		CharFull_Renderer->SetImage("Lobby_CharFull_Hyunwoo.png", "PointSmp");
		CharFull_Renderer->TextSetting("굴림", "현우", 40.0f, FW1_CENTER, { 0.85f, 0.85f, 1.0f, 1.0f }, { 0.0f, -240.0f });
		CharFull_Renderer->On();

		qDescription_->SetImage("HyunWoo_Q_Icon.png", "PointSmp");
		qDescription_->TextSetting("굴림", "Q : 정면을 강하게 밟아 적들에게 피해를 입힙니다.\n     적중된 적의 이동속도를 감소시킵니다.", 16.0f, FW1_LEFT, { 0.85f, 0.85f, 1.0f, 1.0f }, { 40.0f, 20.0f });
		qDescription_->On();

		wDescription_->SetImage("HyunWoo_W_Icon.png", "PointSmp");
		wDescription_->TextSetting("굴림", "W : 2.5초동안 방어력이 증가합니다.\n     발동 후 1초 간 모든 방해 효과를 무시합니다.", 16.0f, FW1_LEFT, { 0.85f, 0.85f, 1.0f, 1.0f }, { 40.0f, 20.0f });
		wDescription_->On();

		eDescription_->SetImage("HyunWoo_E_Icon.png", "PointSmp");
		eDescription_->TextSetting("굴림", "E : 전방으로 돌진해 적과 부딪히면 적을 밀어냅니다.\n     밀려난 적이 벽에 부딪히면 기절합니다.", 16.0f, FW1_LEFT, { 0.85f, 0.85f, 1.0f, 1.0f }, { 40.0f, 20.0f });
		eDescription_->On();

		rDescription_->SetImage("HyunWoo_R_Icon.png", "PointSmp");
		rDescription_->TextSetting("굴림", "R : 힘을 충전한 후, 강력한 한 방을 날려 피해를 입힙니다.\n     충전할수록 공격력과 사정거리가 증가합니다.", 16.0f, FW1_LEFT, { 0.85f, 0.85f, 1.0f, 1.0f }, { 40.0f, 20.0f });
		rDescription_->On();
		break;
	case JobType::JACKIE:
		CharFull_Renderer->On();
		CharFull_Renderer->SetImage("Lobby_CharFull_Jackie.png", "PointSmp");
		CharFull_Renderer->TextSetting("굴림", "재키", 40.0f, FW1_CENTER, { 0.85f, 0.85f, 1.0f, 1.0f }, { 0.0f, -240.0f });
		CharFull_Renderer->On();

		qDescription_->SetImage("Jackie_Q_Icon.png", "PointSmp");
		qDescription_->TextSetting("굴림", "Q : 두 번 연속으로 공격합니다.\n     적중된 적은 6초 동안 지속적인 피해를 입습니다.", 16.0f, FW1_LEFT, { 0.85f, 0.85f, 1.0f, 1.0f }, { 40.0f, 20.0f });
		qDescription_->On();

		wDescription_->SetImage("Jackie_W_Icon.png", "PointSmp");
		wDescription_->TextSetting("굴림", "W : 이동속도가 즉시 증가하고, 4초에 걸쳐 감소됩니다.\n     적을 공격하면 체력을 회복합니다.", 16.0f, FW1_LEFT, { 0.85f, 0.85f, 1.0f, 1.0f }, { 40.0f, 20.0f });
		wDescription_->On();

		eDescription_->SetImage("Jackie_E_Icon.png", "PointSmp");
		eDescription_->TextSetting("굴림", "E : 전방으로 점프해 적들에게 피해를 입힙니다.\n     적중된 적의 이동속도를 3초간 감소시킵니다.", 16.0f, FW1_LEFT, { 0.85f, 0.85f, 1.0f, 1.0f }, { 40.0f, 20.0f });
		eDescription_->On();

		rDescription_->SetImage("Jackie_R_Icon.png", "PointSmp");
		rDescription_->TextSetting("굴림", "R : 15초간 전기톱을 꺼내듭니다.\n     공격 속도와 공격력이 증가합니다.", 16.0f, FW1_LEFT, { 0.85f, 0.85f, 1.0f, 1.0f }, { 40.0f, 20.0f });
		rDescription_->On();
		break;
	case JobType::RIO:
		CharFull_Renderer->On();
		CharFull_Renderer->SetImage("Lobby_CharFull_Rio.png", "PointSmp");
		CharFull_Renderer->TextSetting("굴림", "리오", 40.0f, FW1_CENTER, { 0.85f, 0.85f, 1.0f, 1.0f }, { 0.0f, -240.0f });
		CharFull_Renderer->On();

		qDescription_->SetImage("Rio_Q_Icon.png", "PointSmp");
		qDescription_->TextSetting("굴림", "Q : 무기를 단궁과 화궁으로 변경합니다.\n     단궁은 공격속도가 빠르며 화궁은 사정거리가 깁니다.", 16.0f, FW1_LEFT, { 0.85f, 0.85f, 1.0f, 1.0f }, { 40.0f, 20.0f });
		qDescription_->On();

		wDescription_->SetImage("Rio_W_Icon.png", "PointSmp");
		wDescription_->TextSetting("굴림", "W : (단궁)피해를 주고 이동 속도를 감소시킵니다.\n     (화궁)관통 화살을 날려 피해를 줍니다.", 16.0f, FW1_LEFT, { 0.85f, 0.85f, 1.0f, 1.0f }, { 40.0f, 20.0f });
		wDescription_->On();

		eDescription_->SetImage("Rio_E_Icon.png", "PointSmp");
		eDescription_->TextSetting("굴림", "E : 전방으로 점프하며 피해를 입힙니다.\n     적중된 적이 있다면 쿨다운이 감소합니다.", 16.0f, FW1_LEFT, { 0.85f, 0.85f, 1.0f, 1.0f }, { 40.0f, 20.0f });
		eDescription_->On();

		rDescription_->SetImage("Rio_R_Icon.png", "PointSmp");
		rDescription_->TextSetting("굴림", "R : (단궁)여러 화살을 발사 후, 적을 밀어냅니다.\n     (화궁)강력한 화살 한 방을 날립니다.", 16.0f, FW1_LEFT, { 0.85f, 0.85f, 1.0f, 1.0f }, { 40.0f, 20.0f });
		rDescription_->On();
		break;
	case JobType::AYA:
		CharFull_Renderer->On();
		CharFull_Renderer->SetImage("Lobby_CharFull_Aya.png", "PointSmp");
		CharFull_Renderer->TextSetting("굴림", "아야", 40.0f, FW1_CENTER, { 0.85f, 0.85f, 1.0f, 1.0f }, { 0.0f, -240.0f });
		CharFull_Renderer->On();

		qDescription_->SetImage("Aya_Q_Icon.png", "PointSmp");
		qDescription_->TextSetting("굴림", "Q : 대상을 향해 2번 사격합니다.\n     초탄 명중 시 후속타가 확정 명중합니다.", 16.0f, FW1_LEFT, { 0.85f, 0.85f, 1.0f, 1.0f }, { 40.0f, 20.0f });
		qDescription_->On();

		wDescription_->SetImage("Aya_W_Icon.png", "PointSmp");
		wDescription_->TextSetting("굴림", "W : 한 방향으로 총을 난사합니다.", 16.0f, FW1_LEFT, { 0.85f, 0.85f, 1.0f, 1.0f }, { 40.0f, 20.0f });
		wDescription_->On();

		eDescription_->SetImage("Aya_E_Icon.png", "PointSmp");
		eDescription_->TextSetting("굴림", "E : 지정한 방향으로 재빠르게 이동합니다.", 16.0f, FW1_LEFT, { 0.85f, 0.85f, 1.0f, 1.0f }, { 40.0f, 20.0f });
		eDescription_->On();

		rDescription_->SetImage("Aya_R_Icon.png", "PointSmp");
		rDescription_->TextSetting("굴림", "R : 주변의 적들에게 피해를 입힙니다.\n     적중한 적은 몇 초간 행동 불능 상태가 됩니다.", 16.0f, FW1_LEFT, { 0.85f, 0.85f, 1.0f, 1.0f }, { 40.0f, 20.0f });
		rDescription_->On();
		break;
	case JobType::MAX:
		CharFull_Renderer->Off();
		break;
	default:
		break;
	}
	return;
	

}
