#pragma once
#include <GameEngine/GameEngineUIRenderer.h>
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>
#include <GameEngine/GameEngineEffectRenderer.h>
// �з� : 
// �뵵 : 
// ���� : 
class UI_FOW : public GameEngineActor
{
public:
	UI_FOW(); // default constructer ����Ʈ ������
	~UI_FOW(); // default destructer ����Ʈ �Ҹ���
	UI_FOW(const UI_FOW& _other) = delete; // default Copy constructer ����Ʈ ���������
	UI_FOW(UI_FOW&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������
	UI_FOW& operator=(const UI_FOW& _other) = delete; // default Copy operator ����Ʈ ���� ������
	UI_FOW& operator=(const UI_FOW&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

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

