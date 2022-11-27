#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>
// �з� : 
// �뵵 : 
// ���� : 
class GameEngineEffectRenderer;
class YukiQSlashEffect : public GameEngineActor
{
public:
	YukiQSlashEffect(); // default constructer ����Ʈ ������
	~YukiQSlashEffect(); // default destructer ����Ʈ �Ҹ���
	YukiQSlashEffect(const YukiQSlashEffect& _other) = delete; // default Copy constructer ����Ʈ ���������
	YukiQSlashEffect(YukiQSlashEffect&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������
	YukiQSlashEffect& operator=(const YukiQSlashEffect& _other) = delete; // default Copy operator ����Ʈ ���� ������
	YukiQSlashEffect& operator=(const YukiQSlashEffect&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:
	void PlayBurst(float4 _pos);
	void Stop();

protected:
	void Start() override;
	void Update(float _deltaTime) override;

protected:
	void startSleep();
	void updateSleep(float _deltaTime);

	void startBurst();
	void updateBurst(float _deltaTime);

private:
	GameEngineFSM renderState_;

	GameEngineEffectRenderer* slashRenderer_;
	GameEngineEffectRenderer* kiBurstRenderer_;
private:
	float timer_;
	float4 targetPos_;
};

