#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>

class GameEngineUIRenderer;
class UI_BloodBackground : public GameEngineActor
{
public:
	UI_BloodBackground(); // default constructer ����Ʈ ������
	~UI_BloodBackground(); // default destructer ����Ʈ �Ҹ���
	UI_BloodBackground(const UI_BloodBackground& _other) = delete; // default Copy constructer ����Ʈ ���������
	UI_BloodBackground(UI_BloodBackground&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������
	UI_BloodBackground& operator=(const UI_BloodBackground& _other) = delete; // default Copy operator ����Ʈ ���� ������
	UI_BloodBackground& operator=(const UI_BloodBackground&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

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

