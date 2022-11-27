#pragma once
#include <GameEngine/GameEngineActor.h>

// �з� : 
// �뵵 : 
// ���� : 

class Hyunwoo;
class GameEngineImageRenderer;
class HyunwooEffect : public GameEngineActor
{
	friend Hyunwoo;

public:
	HyunwooEffect(); // default constructer ����Ʈ ������
	~HyunwooEffect(); // default destructer ����Ʈ �Ҹ���
	HyunwooEffect(const HyunwooEffect& _other) = delete; // default Copy constructer ����Ʈ ���������
	HyunwooEffect(HyunwooEffect&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������
	HyunwooEffect& operator=(const HyunwooEffect& _other) = delete; // default Copy operator ����Ʈ ���� ������
	HyunwooEffect& operator=(const HyunwooEffect&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:

protected:
	GameEngineImageRenderer* frontRenderer_;

	void Start() override;
	void Update(float _deltaTime) override;



private:
};

