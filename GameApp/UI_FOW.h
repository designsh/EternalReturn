#pragma once
#include <GameEngine/GameEngineUIRenderer.h>
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>
#include <GameEngine/GameEngineEffectRenderer.h>
// 분류 : 
// 용도 : 
// 설명 : 
class UI_FOW : public GameEngineActor
{
public:
	UI_FOW(); // default constructer 디폴트 생성자
	~UI_FOW(); // default destructer 디폴트 소멸자
	UI_FOW(const UI_FOW& _other) = delete; // default Copy constructer 디폴트 복사생성자
	UI_FOW(UI_FOW&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자
	UI_FOW& operator=(const UI_FOW& _other) = delete; // default Copy operator 디폴트 대입 연산자
	UI_FOW& operator=(const UI_FOW&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:
	void SetFOW(bool _b);

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

	void startToMorning();
	void updateToMorning(float _DeltaTime);

	void startToNight();
	void updateToNight(float _DeltaTime);

private:
	GameEngineFSM UIState_;
	GameEngineUIRenderer* ambientRenderer_;
	GameEngineUIRenderer* fowRenderer_;
	float timer_;
	bool isNight_;

};

