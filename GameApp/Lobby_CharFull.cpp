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
		CharFull_Renderer->TextSetting("����", "��Ű", 40.0f, FW1_CENTER, {0.85f, 0.85f, 1.0f, 1.0f}, {0.0f, -240.0f});
		CharFull_Renderer->On();

		qDescription_->SetImage("Yuki_Q_Icon.png", "PointSmp");
		qDescription_->TextSetting("����", "Q : ���� ������ �߰� ���ظ� �����ϴ�.\n     ���� �� ���� �̵��ӵ��� ���ҽ�ŵ�ϴ�.", 16.0f, FW1_LEFT, { 0.85f, 0.85f, 1.0f, 1.0f }, { 40.0f, 20.0f });
		qDescription_->On();

		wDescription_->SetImage("Yuki_W_Icon.png", "PointSmp");
		wDescription_->TextSetting("����", "W : ��� �� E�� ��Ÿ���� 3�� �����մϴ�.\n     ������ 50% �����մϴ�.", 16.0f, FW1_LEFT, { 0.85f, 0.85f, 1.0f, 1.0f }, { 40.0f, 20.0f });
		wDescription_->On();

		eDescription_->SetImage("Yuki_E_Icon.png", "PointSmp");
		eDescription_->TextSetting("����", "E : �����Ͽ� �ε��� ������ ���ظ� �����ϴ�.\n     ���� �� ��Ÿ���� 3�� �����մϴ�.", 16.0f, FW1_LEFT, { 0.85f, 0.85f, 1.0f, 1.0f }, { 40.0f, 20.0f });
		eDescription_->On();

		rDescription_->SetImage("Yuki_R_Icon.png", "PointSmp");
		rDescription_->TextSetting("����", "R : ���� ���� ������ ���մϴ�.\n     �ļ�Ÿ�� �ִ� ü�� ��� ���� ���ظ� �����ϴ�.", 16.0f, FW1_LEFT, { 0.85f, 0.85f, 1.0f, 1.0f }, { 40.0f, 20.0f });
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
	//	CharFull_Renderer->TextSetting("����", "����", 40.0f, FW1_CENTER, { 0.85f, 0.85f, 1.0f, 1.0f }, { 0.0f, -240.0f });
	//	CharFull_Renderer->On();
	//
	//	qDescription_->SetImage("Nadine_Q_Icon.png", "PointSmp");
	//	qDescription_->TextSetting("����", "Q : ���� ��� ȭ���� �߻��մϴ�.\n     �������� ���ݷ°� ��Ÿ��� �����մϴ�.", 16.0f, FW1_LEFT, { 0.85f, 0.85f, 1.0f, 1.0f }, { 40.0f, 20.0f });
	//	qDescription_->On();
	//
	//	wDescription_->SetImage("Nadine_W_Icon.png", "PointSmp");
	//	wDescription_->TextSetting("����", "W : ���� ��ġ�� ��θ� �����ϰ� ���ظ� �����ϴ�.\n     ���� �ɸ��� �̵��ӵ��� �����մϴ�.", 16.0f, FW1_LEFT, { 0.85f, 0.85f, 1.0f, 1.0f }, { 40.0f, 20.0f });
	//	wDescription_->On();
	//
	//	eDescription_->SetImage("Nadine_E_Icon.png", "PointSmp");
	//	eDescription_->TextSetting("����", "E : ������ ��ġ�� ���̾ �߻��մϴ�.\n     �ش� �������� ������ Ÿ�� �̵��� �� �ֽ��ϴ�.", 16.0f, FW1_LEFT, { 0.85f, 0.85f, 1.0f, 1.0f }, { 40.0f, 20.0f });
	//	eDescription_->On();
	//
	//	rDescription_->SetImage("Nadine_R_Icon.png", "PointSmp");
	//	rDescription_->TextSetting("����", "R : �� 3��° ���ݸ��� ���븦 ��ȯ�� ���� �����մϴ�.", 16.0f, FW1_LEFT, { 0.85f, 0.85f, 1.0f, 1.0f }, { 40.0f, 20.0f });
	//	rDescription_->On();
	//	break;
	case JobType::HYUNWOO:
		CharFull_Renderer->On();
		CharFull_Renderer->SetImage("Lobby_CharFull_Hyunwoo.png", "PointSmp");
		CharFull_Renderer->TextSetting("����", "����", 40.0f, FW1_CENTER, { 0.85f, 0.85f, 1.0f, 1.0f }, { 0.0f, -240.0f });
		CharFull_Renderer->On();

		qDescription_->SetImage("HyunWoo_Q_Icon.png", "PointSmp");
		qDescription_->TextSetting("����", "Q : ������ ���ϰ� ��� ���鿡�� ���ظ� �����ϴ�.\n     ���ߵ� ���� �̵��ӵ��� ���ҽ�ŵ�ϴ�.", 16.0f, FW1_LEFT, { 0.85f, 0.85f, 1.0f, 1.0f }, { 40.0f, 20.0f });
		qDescription_->On();

		wDescription_->SetImage("HyunWoo_W_Icon.png", "PointSmp");
		wDescription_->TextSetting("����", "W : 2.5�ʵ��� ������ �����մϴ�.\n     �ߵ� �� 1�� �� ��� ���� ȿ���� �����մϴ�.", 16.0f, FW1_LEFT, { 0.85f, 0.85f, 1.0f, 1.0f }, { 40.0f, 20.0f });
		wDescription_->On();

		eDescription_->SetImage("HyunWoo_E_Icon.png", "PointSmp");
		eDescription_->TextSetting("����", "E : �������� ������ ���� �ε����� ���� �о���ϴ�.\n     �з��� ���� ���� �ε����� �����մϴ�.", 16.0f, FW1_LEFT, { 0.85f, 0.85f, 1.0f, 1.0f }, { 40.0f, 20.0f });
		eDescription_->On();

		rDescription_->SetImage("HyunWoo_R_Icon.png", "PointSmp");
		rDescription_->TextSetting("����", "R : ���� ������ ��, ������ �� ���� ���� ���ظ� �����ϴ�.\n     �����Ҽ��� ���ݷ°� �����Ÿ��� �����մϴ�.", 16.0f, FW1_LEFT, { 0.85f, 0.85f, 1.0f, 1.0f }, { 40.0f, 20.0f });
		rDescription_->On();
		break;
	case JobType::JACKIE:
		CharFull_Renderer->On();
		CharFull_Renderer->SetImage("Lobby_CharFull_Jackie.png", "PointSmp");
		CharFull_Renderer->TextSetting("����", "��Ű", 40.0f, FW1_CENTER, { 0.85f, 0.85f, 1.0f, 1.0f }, { 0.0f, -240.0f });
		CharFull_Renderer->On();

		qDescription_->SetImage("Jackie_Q_Icon.png", "PointSmp");
		qDescription_->TextSetting("����", "Q : �� �� �������� �����մϴ�.\n     ���ߵ� ���� 6�� ���� �������� ���ظ� �Խ��ϴ�.", 16.0f, FW1_LEFT, { 0.85f, 0.85f, 1.0f, 1.0f }, { 40.0f, 20.0f });
		qDescription_->On();

		wDescription_->SetImage("Jackie_W_Icon.png", "PointSmp");
		wDescription_->TextSetting("����", "W : �̵��ӵ��� ��� �����ϰ�, 4�ʿ� ���� ���ҵ˴ϴ�.\n     ���� �����ϸ� ü���� ȸ���մϴ�.", 16.0f, FW1_LEFT, { 0.85f, 0.85f, 1.0f, 1.0f }, { 40.0f, 20.0f });
		wDescription_->On();

		eDescription_->SetImage("Jackie_E_Icon.png", "PointSmp");
		eDescription_->TextSetting("����", "E : �������� ������ ���鿡�� ���ظ� �����ϴ�.\n     ���ߵ� ���� �̵��ӵ��� 3�ʰ� ���ҽ�ŵ�ϴ�.", 16.0f, FW1_LEFT, { 0.85f, 0.85f, 1.0f, 1.0f }, { 40.0f, 20.0f });
		eDescription_->On();

		rDescription_->SetImage("Jackie_R_Icon.png", "PointSmp");
		rDescription_->TextSetting("����", "R : 15�ʰ� �������� ������ϴ�.\n     ���� �ӵ��� ���ݷ��� �����մϴ�.", 16.0f, FW1_LEFT, { 0.85f, 0.85f, 1.0f, 1.0f }, { 40.0f, 20.0f });
		rDescription_->On();
		break;
	case JobType::RIO:
		CharFull_Renderer->On();
		CharFull_Renderer->SetImage("Lobby_CharFull_Rio.png", "PointSmp");
		CharFull_Renderer->TextSetting("����", "����", 40.0f, FW1_CENTER, { 0.85f, 0.85f, 1.0f, 1.0f }, { 0.0f, -240.0f });
		CharFull_Renderer->On();

		qDescription_->SetImage("Rio_Q_Icon.png", "PointSmp");
		qDescription_->TextSetting("����", "Q : ���⸦ �ܱð� ȭ������ �����մϴ�.\n     �ܱ��� ���ݼӵ��� ������ ȭ���� �����Ÿ��� ��ϴ�.", 16.0f, FW1_LEFT, { 0.85f, 0.85f, 1.0f, 1.0f }, { 40.0f, 20.0f });
		qDescription_->On();

		wDescription_->SetImage("Rio_W_Icon.png", "PointSmp");
		wDescription_->TextSetting("����", "W : (�ܱ�)���ظ� �ְ� �̵� �ӵ��� ���ҽ�ŵ�ϴ�.\n     (ȭ��)���� ȭ���� ���� ���ظ� �ݴϴ�.", 16.0f, FW1_LEFT, { 0.85f, 0.85f, 1.0f, 1.0f }, { 40.0f, 20.0f });
		wDescription_->On();

		eDescription_->SetImage("Rio_E_Icon.png", "PointSmp");
		eDescription_->TextSetting("����", "E : �������� �����ϸ� ���ظ� �����ϴ�.\n     ���ߵ� ���� �ִٸ� ��ٿ��� �����մϴ�.", 16.0f, FW1_LEFT, { 0.85f, 0.85f, 1.0f, 1.0f }, { 40.0f, 20.0f });
		eDescription_->On();

		rDescription_->SetImage("Rio_R_Icon.png", "PointSmp");
		rDescription_->TextSetting("����", "R : (�ܱ�)���� ȭ���� �߻� ��, ���� �о���ϴ�.\n     (ȭ��)������ ȭ�� �� ���� �����ϴ�.", 16.0f, FW1_LEFT, { 0.85f, 0.85f, 1.0f, 1.0f }, { 40.0f, 20.0f });
		rDescription_->On();
		break;
	case JobType::AYA:
		CharFull_Renderer->On();
		CharFull_Renderer->SetImage("Lobby_CharFull_Aya.png", "PointSmp");
		CharFull_Renderer->TextSetting("����", "�ƾ�", 40.0f, FW1_CENTER, { 0.85f, 0.85f, 1.0f, 1.0f }, { 0.0f, -240.0f });
		CharFull_Renderer->On();

		qDescription_->SetImage("Aya_Q_Icon.png", "PointSmp");
		qDescription_->TextSetting("����", "Q : ����� ���� 2�� ����մϴ�.\n     ��ź ���� �� �ļ�Ÿ�� Ȯ�� �����մϴ�.", 16.0f, FW1_LEFT, { 0.85f, 0.85f, 1.0f, 1.0f }, { 40.0f, 20.0f });
		qDescription_->On();

		wDescription_->SetImage("Aya_W_Icon.png", "PointSmp");
		wDescription_->TextSetting("����", "W : �� �������� ���� �����մϴ�.", 16.0f, FW1_LEFT, { 0.85f, 0.85f, 1.0f, 1.0f }, { 40.0f, 20.0f });
		wDescription_->On();

		eDescription_->SetImage("Aya_E_Icon.png", "PointSmp");
		eDescription_->TextSetting("����", "E : ������ �������� ������� �̵��մϴ�.", 16.0f, FW1_LEFT, { 0.85f, 0.85f, 1.0f, 1.0f }, { 40.0f, 20.0f });
		eDescription_->On();

		rDescription_->SetImage("Aya_R_Icon.png", "PointSmp");
		rDescription_->TextSetting("����", "R : �ֺ��� ���鿡�� ���ظ� �����ϴ�.\n     ������ ���� �� �ʰ� �ൿ �Ҵ� ���°� �˴ϴ�.", 16.0f, FW1_LEFT, { 0.85f, 0.85f, 1.0f, 1.0f }, { 40.0f, 20.0f });
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
