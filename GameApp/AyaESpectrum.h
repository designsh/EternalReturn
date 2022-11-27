#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>
// �뵵 : 
// ���� : 
class GameEngineFBXRenderer;
class AyaESpectrum : public GameEngineActor
{
public:
	AyaESpectrum(); // default constructer ����Ʈ ������
	~AyaESpectrum(); // default destructer ����Ʈ �Ҹ���
	AyaESpectrum(const AyaESpectrum& _other) = delete; // default Copy constructer ����Ʈ ���������
	AyaESpectrum(AyaESpectrum&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������
	AyaESpectrum& operator=(const AyaESpectrum& _other) = delete; // default Copy operator ����Ʈ ���� ������
	AyaESpectrum& operator=(const AyaESpectrum&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:
	void PlayAwake();
	void Reset();

	int GetCount() { return count_; }


protected:
	void Start() override;
	void Update(float _deltaTime) override;


protected:
	void startAwake();
	void updateAwake(float _deltaTime);

	void startSleep();
	void updateSleep(float _deltaTime);

	void startFade();
	void updateFade(float _deltaTime);

private:
	GameEngineFSM renderState_;
	std::vector<GameEngineFBXRenderer*> renderers_;
	float revealTime_;
	int count_;
	bool isStopped_;
};

