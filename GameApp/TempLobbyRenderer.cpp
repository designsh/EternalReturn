#include "PreCompile.h"
#include "TempLobbyRenderer.h"
#include "GameEngine/GameEngineUIRenderer.h"

TempLobbyRenderer::TempLobbyRenderer() // default constructer ����Ʈ ������
	:portraitRenderer_(nullptr)
{

}

TempLobbyRenderer::~TempLobbyRenderer() // default destructer ����Ʈ �Ҹ���
{

}

TempLobbyRenderer::TempLobbyRenderer(const TempLobbyRenderer& _other)
{
}

void TempLobbyRenderer::SetPos(const float4& _pos)
{
	GetTransform()->SetLocalPosition(_pos);
}

void TempLobbyRenderer::SetScale(const float4& _pos)
{
	GetTransform()->SetLocalScaling(_pos);
}

void TempLobbyRenderer::SetImage(std::string _imageName)
{
	portraitRenderer_->SetImage(_imageName);
}

void TempLobbyRenderer::SetRender(bool _bool)
{
	if (true == _bool)
	{
		portraitRenderer_->On();
	}
	else
	{
		portraitRenderer_->Off();
	}
}

void TempLobbyRenderer::SetText(const std::string& _text)
{
	portraitRenderer_->TextSetting("����", _text, 12, FW1_LEFT, float4::WHITE, {-100.0f, 0.0f, 0.0f});
}

void TempLobbyRenderer::Start()
{
	portraitRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), static_cast<int>(UIRenderOrder::UI));
	portraitRenderer_->SetImage("Down_Lobby_Portrait_Anon.png", "PointSmp");
}

void TempLobbyRenderer::Update(float _DeltaTime)
{
}


