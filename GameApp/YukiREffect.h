#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>
// �з� : 
// �뵵 : 
// ���� : 
class GameEngineImageRenderer;
class GameEngineEffectRenderer;
class YukiREffect : public GameEngineActor
{
public:
	YukiREffect(); // default constructer ����Ʈ ������
	~YukiREffect(); // default destructer ����Ʈ �Ҹ���
	YukiREffect(const YukiREffect& _other) = delete; // default Copy constructer ����Ʈ ���������
	YukiREffect(YukiREffect&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������
	YukiREffect& operator=(const YukiREffect& _other) = delete; // default Copy operator ����Ʈ ���� ������
	YukiREffect& operator=(const YukiREffect&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:
	void PlayAwake();
	void PlaySlash();

protected:
	void Start() override;
	void Update(float _deltaTime) override;


protected:
	void startAwake();
	void updateAwake(float _deltaTime);

	void startSleep();
	void updateSleep(float _deltaTime);

	void startIntermission();
	void updateIntermission(float _deltaTime);

	void startSlash();
	void updateSlash(float _deltaTime);
private:

	GameEngineFSM renderState_;
	GameEngineEffectRenderer* zoneRenderer_;
	GameEngineEffectRenderer* impactRenderer_;

private:
	float groundFadeTime_;
	float groundOutTime_;

	bool slashFlag_;
	float slashFadeTime_;
	float timer_;
};

