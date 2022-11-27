#pragma once
#include "GameEngine/GameEngineUIRenderer.h"

using namespace std;

//

class UI_CalculateHelper : public GameEngineActor
{

public:
	UI_CalculateHelper();
	~UI_CalculateHelper();

public:
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

	float4 CalRound(float4 _origin)
	{
		float4 returnfloat4 = { round(_origin.x), round(_origin.y), _origin.z, _origin.w };
		return returnfloat4;
	}

private:
	UI_CalculateHelper(const UI_CalculateHelper& _Other) = delete;
	UI_CalculateHelper(UI_CalculateHelper&& _Other) noexcept = delete;
	UI_CalculateHelper& operator=(const UI_CalculateHelper& _Other) = delete;
	UI_CalculateHelper& operator=(UI_CalculateHelper&& _Other) noexcept = delete;
};

