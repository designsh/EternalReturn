#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>

class GameEngineUIRenderer;
class UI_BloodBackground : public GameEngineActor
{
public:
	UI_BloodBackground(); // default constructer 디폴트 생성자
	~UI_BloodBackground(); // default destructer 디폴트 소멸자
	UI_BloodBackground(const UI_BloodBackground& _other) = delete; // default Copy constructer 디폴트 복사생성자
	UI_BloodBackground(UI_BloodBackground&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자
	UI_BloodBackground& operator=(const UI_BloodBackground& _other) = delete; // default Copy operator 디폴트 대입 연산자
	UI_BloodBackground& operator=(const UI_BloodBackground&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:
	void Draw(bool _flag);
	void PlayerDead() { playerDead_ = true;}

protected:
	void Start() override;
	void Update(float _Time) override;

	void startGoRed();
	void updateGoRed(float _Time);
	void startGoBlack();
	void updateGoBlack(float _Time);



private:
	GameEngineFSM state_;
	GameEngineUIRenderer* bloodRenderer;
	float timer_;
	bool playerDead_;
};

