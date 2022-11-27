#pragma once
#include <GameEngine/GameEngineActor.h>
// �з� : 
// �뵵 : 
// ���� : 
class GameEngineImageRenderer;
class TitleFoxLogo : public GameEngineActor
{
public:
	TitleFoxLogo(); // default constructer ����Ʈ ������
	~TitleFoxLogo(); // default destructer ����Ʈ �Ҹ���
	TitleFoxLogo(const TitleFoxLogo& _other) = delete; // default Copy constructer ����Ʈ ���������
	TitleFoxLogo(TitleFoxLogo&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������
	TitleFoxLogo& operator=(const TitleFoxLogo& _other) = delete; // default Copy operator ����Ʈ ���� ������
	TitleFoxLogo& operator=(const TitleFoxLogo&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

protected:
	virtual void Start();
	virtual void Update(float _DeltaTime);

private:
	GameEngineImageRenderer* renderer_;

};

