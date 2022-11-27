#pragma once
#include <GameEngine/GameEngineActor.h>
#include "MousePointer.h"

using namespace std;

class UI_TestMouse : public MousePointer
{
public:
	UI_TestMouse();
	~UI_TestMouse();

protected:
	virtual void Start();
	virtual void Update(float _DeltaTime);

private:
	UI_TestMouse(const UI_TestMouse& _Other) = delete;
	UI_TestMouse(UI_TestMouse&& _Other) noexcept = delete;
	UI_TestMouse& operator=(const UI_TestMouse& _Other) = delete;
	UI_TestMouse& operator=(UI_TestMouse&& _Other) noexcept = delete;

	GameEngineCollision* PointerCollision;
};

