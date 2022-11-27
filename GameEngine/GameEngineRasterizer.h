#pragma once
#include <GameEngineBase/GameEngineObjectNameBase.h>
#include <GameEngine/GameEngineDevice.h>
#include <GameEngine\GameEngineWindow.h>

// 분류 : 
// 용도 : 
// 설명 : 
class GameEngineRasterizer : public GameEngineObjectNameBase
{
private:	// member Var
	ID3D11RasterizerState* State_;
	ID3D11RasterizerState* OldState_;
	D3D11_RASTERIZER_DESC Desc_;
	D3D11_RASTERIZER_DESC WireDesc_;
	D3D11_RECT rects;

	std::vector<D3D11_RECT> ScissorRects_;

	void Clear();

public:
	void Create(const D3D11_RASTERIZER_DESC& _Value);

	void PushScissorRect(D3D11_RECT _Rect);

	GameEngineRasterizer(); // default constructer 디폴트 생성자
	~GameEngineRasterizer(); // default destructer 디폴트 소멸자

protected:		// delete constructer
	GameEngineRasterizer(const GameEngineRasterizer& _other) = delete; // default Copy constructer 디폴트 복사생성자
	GameEngineRasterizer& operator=(const GameEngineRasterizer& _other) = delete; // default Copy operator 디폴트 대입 연산자
	GameEngineRasterizer& operator=(const GameEngineRasterizer&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:
	void SwitchState();

	void Setting();

	GameEngineRasterizer* Clone();
};



