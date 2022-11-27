#pragma once
#include <GameEngine/GameEngineActor.h>
// �з� : 
// �뵵 : 
// ���� : 
class GameEngineImageRenderer;
class TitlePrologueBG : public GameEngineActor
{
public:
	TitlePrologueBG(); // default constructer ����Ʈ ������
	~TitlePrologueBG(); // default destructer ����Ʈ �Ҹ���
	TitlePrologueBG(const TitlePrologueBG& _other) = delete; // default Copy constructer ����Ʈ ���������
	TitlePrologueBG(TitlePrologueBG&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������
	TitlePrologueBG& operator=(const TitlePrologueBG& _other) = delete; // default Copy operator ����Ʈ ���� ������
	TitlePrologueBG& operator=(const TitlePrologueBG&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:
	void SetAlpha(float _Alpha);

protected:
	virtual void Start();
	virtual void Update(float _DeltaTime);

private:
	GameEngineImageRenderer* renderer_;

};

