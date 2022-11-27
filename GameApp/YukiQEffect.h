#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>
// �з� : 
// �뵵 : 
// ���� : 
class GameEngineEffectRenderer;
class YukiQEffect : public GameEngineActor
{
public:
	YukiQEffect(); // default constructer ����Ʈ ������
	~YukiQEffect(); // default destructer ����Ʈ �Ҹ���
	YukiQEffect(const YukiQEffect& _other) = delete; // default Copy constructer ����Ʈ ���������
	YukiQEffect(YukiQEffect&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������
	YukiQEffect& operator=(const YukiQEffect& _other) = delete; // default Copy operator ����Ʈ ���� ������
	YukiQEffect& operator=(const YukiQEffect&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:
	void PlayAwake();
	void Stop();

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
	GameEngineEffectRenderer* kiGatheringRenderer_;
	GameEngineEffectRenderer* kiCoreRenderer_;
private:
	float timer_;
	float4 targetPos_;

	float rotationDegree_;
	float4 scaleContainer_;
};