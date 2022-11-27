#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>

// �з� : 
// �뵵 : 
// ���� : 
class GameEngineEffectRenderer;
class HyunwooWEffect : public GameEngineActor
{
public:
	HyunwooWEffect(); // default constructer ����Ʈ ������
	~HyunwooWEffect(); // default destructer ����Ʈ �Ҹ���
	HyunwooWEffect(const HyunwooWEffect& _other) = delete; // default Copy constructer ����Ʈ ���������
	HyunwooWEffect(HyunwooWEffect&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������
	HyunwooWEffect& operator=(const HyunwooWEffect& _other) = delete; // default Copy operator ����Ʈ ���� ������
	HyunwooWEffect& operator=(const HyunwooWEffect&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:
	void PlayAwake();

protected:
	void Start() override;
	void Update(float _deltaTime) override;


protected:
	void startSleep();
	void updateSleep(float _deltaTime);

	void startAwake();
	void updateAwake(float _deltaTime);

	void startFade();
	void updateFade(float _deltaTime);

private:
	GameEngineFSM renderState_;
	GameEngineEffectRenderer* maskRenderer_;
	GameEngineEffectRenderer* shieldRenderer_;

private:
	float awakeTimer_;
	float fadeTimer_;


};

