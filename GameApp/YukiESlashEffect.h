#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>
// �з� : 
// �뵵 : 
// ���� : 
class GameEngineEffectRenderer;
class YukiESlashEffect : public GameEngineActor
{
public:
	YukiESlashEffect(); // default constructer ����Ʈ ������
	~YukiESlashEffect(); // default destructer ����Ʈ �Ҹ���
	YukiESlashEffect(const YukiESlashEffect& _other) = delete; // default Copy constructer ����Ʈ ���������
	YukiESlashEffect(YukiESlashEffect&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������
	YukiESlashEffect& operator=(const YukiESlashEffect& _other) = delete; // default Copy operator ����Ʈ ���� ������
	YukiESlashEffect& operator=(const YukiESlashEffect&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

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

private:
	GameEngineFSM renderState_;

	GameEngineEffectRenderer* slashRenderer_;

private:
	float timer_;
};

