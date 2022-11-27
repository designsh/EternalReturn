#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>
#include <GameEngine/GameEngineUIRenderer.h>
// �з� : 
// �뵵 : 
// ���� : 
class CharFullSelectEffect : public GameEngineActor
{
public:
	CharFullSelectEffect(); // default constructer ����Ʈ ������
	~CharFullSelectEffect(); // default destructer ����Ʈ �Ҹ���
	CharFullSelectEffect(const CharFullSelectEffect& _other) = delete; // default Copy constructer ����Ʈ ���������
	CharFullSelectEffect(CharFullSelectEffect&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������
	CharFullSelectEffect& operator=(const CharFullSelectEffect& _other) = delete; // default Copy operator ����Ʈ ���� ������
	CharFullSelectEffect& operator=(const CharFullSelectEffect&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:
	void PlayAwake();
	void SetCharImage(JobType _type);

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
	GameEngineUIRenderer* charStencilRenderer_;
	GameEngineUIRenderer* glowLineRenderer_;
	GameEngineUIRenderer* ringRenderer_;

private:
	float timer_;
	float glowTimer_;
	float rotationDegree_;
	float4 scaleContainer_;
};

