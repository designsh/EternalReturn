#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>
// �з� : 
// �뵵 : 
// ���� : 
class GameEngineImageRenderer;
class GameEngineEffectRenderer;
class JackieQEffect : public GameEngineActor
{
public:
	JackieQEffect(); // default constructer ����Ʈ ������
	~JackieQEffect(); // default destructer ����Ʈ �Ҹ���
	JackieQEffect(const JackieQEffect& _other) = delete; // default Copy constructer ����Ʈ ���������
	JackieQEffect(JackieQEffect&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������
	JackieQEffect& operator=(const JackieQEffect& _other) = delete; // default Copy operator ����Ʈ ���� ������
	JackieQEffect& operator=(const JackieQEffect&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:
	void PlayAwake();

protected:
	void Start() override;
	void Update(float _deltaTime) override;


protected:
	void startAwake();
	void updateAwake(float _deltaTime);

	void startSleep();
	void updateSleep(float _deltaTime);

private:

	GameEngineFSM renderState_;
	GameEngineEffectRenderer* zoneRenderer_;
	GameEngineEffectRenderer* impactRenderer_;

private:
	bool slashFlag_;
	float crackFadeTime_;
	float timer_;

};

