#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>
// �з� : 
// �뵵 : 
// ���� : 
class GameEngineImageRenderer;
class GameEngineEffectRenderer;
class HyunwooQEffect : public GameEngineActor
{
public:
	HyunwooQEffect(); // default constructer ����Ʈ ������
	~HyunwooQEffect(); // default destructer ����Ʈ �Ҹ���
	HyunwooQEffect(const HyunwooQEffect& _other) = delete; // default Copy constructer ����Ʈ ���������
	HyunwooQEffect(HyunwooQEffect&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������
	HyunwooQEffect& operator=(const HyunwooQEffect& _other) = delete; // default Copy operator ����Ʈ ���� ������
	HyunwooQEffect& operator=(const HyunwooQEffect&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

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
	GameEngineEffectRenderer* groundCrackRenderer_;
	GameEngineEffectRenderer* impactRenderer_;
	GameEngineEffectRenderer* dustRenderer_;

private:
	bool isActivated_;
	float crackFadeTime_;
	float timer_;

};

