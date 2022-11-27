#pragma once
#include <GameEngine/GameEngineActor.h>
#include "CharacterStat.h"
#include "MonsterStateInfo.h"

using namespace std;

//ĳ���͸� �Ѿƴٴϴ� UI���� ��Ƶ� Ŭ�����Դϴ�
class GameEngineUIRenderer;
class UI_DamagePopUp : public GameEngineActor
{
public:
	UI_DamagePopUp();
	~UI_DamagePopUp();
	
	void SetChar(JobType _Char)
	{
		MyChar = _Char;
	}


public:
	GameEngineUIRenderer* HPBar_Renderer;

	float4 HpBarPos;
	float4 HpBarSize;

	float Time_;

	bool UIOn;

	JobType MyChar;

	void DamageFontAppear(float4 _Pos, string _DamageValue);
	void DamageFontAppearMonster(float4 _Pos, string _DamageValue);

protected:
	virtual void Start();
	virtual void Update(float _DeltaTime);

private:
	UI_DamagePopUp(const UI_DamagePopUp& _Other) = delete;
	UI_DamagePopUp(UI_DamagePopUp&& _Other) noexcept = delete;
	UI_DamagePopUp& operator=(const UI_DamagePopUp& _Other) = delete;
	UI_DamagePopUp& operator=(UI_DamagePopUp&& _Other) noexcept = delete;

private:
	float4 Cal3Dto2D(float4 _Position)
	{
		//3D������ ��ǥ�� 2Dȭ����ǥ�� ��ȯ

		DirectX::XMVECTOR Position = _Position.DirectVector;
		D3D11_VIEWPORT ViewPort_;

		UINT Number = 1;

		GameEngineDevice::GetContext()->RSGetViewports(&Number, &ViewPort_);

		float4x4 Pro = GetLevel()->GetMainCamera()->GetTransform()->GetTransformData().Projection_;
		float4x4 View = GetLevel()->GetMainCamera()->GetTransform()->GetTransformData().View_;
		DirectX::XMMATRIX ViewProjectionMatrix = View.DirectMatrix * Pro.DirectMatrix;
		Position = DirectX::XMVector3TransformCoord(Position, ViewProjectionMatrix);
		float4 ReturnPosition = { 0.0f, 0.0f };
		ReturnPosition.DirectVector = Position;

		ReturnPosition.DxXmfloat3 = DirectX::XMFLOAT3(ViewPort_.Width * (ReturnPosition.x + 1.0f) / 2.0f + ViewPort_.TopLeftX, ViewPort_.Height * (2.0f - (ReturnPosition.y + 1.0f)) / 2.0f + ViewPort_.TopLeftY, 0.0f);

		ReturnPosition.x -= (ViewPort_.Width / 2);
		ReturnPosition.y = (ReturnPosition.y - (ViewPort_.Height / 2)) * -1.0f;
		return ReturnPosition;
	}
};

