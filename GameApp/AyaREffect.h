#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>
// �з� : 
// �뵵 : 
// ���� : 
class GameEngineImageRenderer;
class GameEngineEffectRenderer;
class AyaREffect : public GameEngineActor
{
public:
	AyaREffect(); // default constructer ����Ʈ ������
	~AyaREffect(); // default destructer ����Ʈ �Ҹ���
	AyaREffect(const AyaREffect& _other) = delete; // default Copy constructer ����Ʈ ���������
	AyaREffect(AyaREffect&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������
	AyaREffect& operator=(const AyaREffect& _other) = delete; // default Copy operator ����Ʈ ���� ������
	AyaREffect& operator=(const AyaREffect&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

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

	void startExplode();
	void updateExplode(float _deltaTime);


	void startFade();
	void updateFade(float _deltaTime);
private:

	GameEngineFSM renderState_;
	GameEngineEffectRenderer* rangeRenderer_;
	GameEngineEffectRenderer* gatherRenderer_;

	GameEngineEffectRenderer* shotRenderer1_;
	GameEngineEffectRenderer* shotRenderer2_;
	GameEngineEffectRenderer* shotRenderer3_;

private:
	float timer_;
	float fadeTimer_;
	float spinCounter_;
	float4 rangeScale_;
	float4 gatherScale_;
};

