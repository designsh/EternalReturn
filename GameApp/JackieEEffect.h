#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>
// �з� : 
// �뵵 : 
// ���� : 
class GameEngineImageRenderer;
class GameEngineEffectRenderer;
class JackieEEffect : public GameEngineActor
{
public:
	JackieEEffect(); // default constructer ����Ʈ ������
	~JackieEEffect(); // default destructer ����Ʈ �Ҹ���
	JackieEEffect(const JackieEEffect& _other) = delete; // default Copy constructer ����Ʈ ���������
	JackieEEffect(JackieEEffect&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������
	JackieEEffect& operator=(const JackieEEffect& _other) = delete; // default Copy operator ����Ʈ ���� ������
	JackieEEffect& operator=(const JackieEEffect&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

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

