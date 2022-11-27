#include "PreCompile.h"
#include "UI_CharPicture.h"
#include "GameEngine/GameEngineUIRenderer.h"
#include "GameEngine/GameEngineInput.h"

UI_CharPicture::UI_CharPicture()
	: Time(1.0f), UIOn(true)
{
}

UI_CharPicture::~UI_CharPicture()
{
	//for (auto& UIRenderer : UIRendererMap)
	//{
	//	if (true)
	//	{
	//		delete UIRenderer.second;
	//		UIRenderer.second = nullptr;
	//	}
	//}
}

void UI_CharPicture::SetJobType(JobType _myjob)
{
	switch (_myjob)
	{
	case JobType::NONE:
		break;
	case JobType::YUKI:
		CharRenderer->SetImage("Char_UI_Yuki.png", "PointSmp");
		break;
	//case JobType::FIORA:
	//	CharRenderer->SetImage("Char_UI_Fiora.png", "PointSmp");
	//	break;
	//case JobType::ZAHIR:
	//	CharRenderer->SetImage("Char_UI_Zahir.png", "PointSmp");
	//	break;
	//case JobType::NADINE:
	//	CharRenderer->SetImage("Char_UI_Nadine.png", "PointSmp");
	//	break;
	case JobType::HYUNWOO:
		CharRenderer->SetImage("Char_UI_Hyunwoo.png", "PointSmp");
		break;
	case JobType::JACKIE:
		CharRenderer->SetImage("Char_UI_Jackie.png", "PointSmp");
		break;
	case JobType::RIO:
		CharRenderer->SetImage("Char_UI_Rio.png", "PointSmp");
		break;
	case JobType::AYA:
		CharRenderer->SetImage("Char_UI_Aya.png", "PointSmp");
		break;
	case JobType::DUMMY:
		break;
	case JobType::MAX:
		break;
	default:
		break;
	}
}

void UI_CharPicture::Start()
{
	//���ҽ����� SetImage�ϰ� ��ġ�� �����մϴ�
	//��ġ�� ������ ���ҽ����� UIRenderMap�� String�� ���� insert�˴ϴ�.
	//�ΰ�������(HP�� ���¹̳� ����, ������񿩺�)���� ������, UIRendererMap���� �̹����̸����� Find�ؼ� ���ҽ��� �ٲ� �������ϴ�.

	float4 CharUI_Pos = { -312.0f, -319.0f, 1.0f };

		//z���� �̿��� �տ����� �̹���/�ڿ����� �̹��� ������ ���ϰ� �ֽ��ϴ�.
		//��ġ������ �� float�� ���� �þ�� map�̳� vector�� ������ �����Դϴ�.

	{
		BackGroundRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::BACKDROP);
		BackGroundRenderer->SetImage("Char_UI_Background.png", "PointSmp");
		BackGroundRenderer->GetTransform()->SetLocalPosition(CharUI_Pos);
		BackGroundRenderer->GetTransform()->SetLocalScaling(BackGroundRenderer->GetCurrentTexture()->GetTextureSize());
	}

	{
		CharRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::UIICON);
		CharRenderer->SetImage("Char_UI_Hyunwoo.png", "PointSmp");
		//��溸�� �׻� �տ� ����
		CharRenderer->GetTransform()->SetLocalPosition(CharUI_Pos + float4{0.0f, 0.0f, -1.0f, 0.0f});
		CharRenderer->GetTransform()->SetLocalScaling(CharRenderer->GetCurrentTexture()->GetTextureSize());
	}

}

void UI_CharPicture::Update(float _Time)
{
	if (0.0f >= (Time -= _Time))
	{
		//TopRenderer->SetAlpha(Time);
	}
	
	//UI �¿��� üũ
	{
		if (false == UIOn)
		{
			BackGroundRenderer->Off();
			CharRenderer->Off();
		}
		else
		{
			BackGroundRenderer->On();
			CharRenderer->On();
		}
	}

	if (true == GameEngineInput::GetInst().Down("Esc"))
	{
		if (UIOn == true)
		{
			UIOn = false;
		}
		else
		{
			UIOn = true;
		}
	}
}

