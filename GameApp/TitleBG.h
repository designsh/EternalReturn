#pragma once
#include <GameEngine/GameEngineActor.h>
// �з� : 
// �뵵 : 
// ���� : 

class GameEngineImageRenderer;
class GameEngineUIRenderer;
class TitleBG : public GameEngineActor
{
public:
	TitleBG(); // default constructer ����Ʈ ������
	~TitleBG(); // default destructer ����Ʈ �Ҹ���
	TitleBG(const TitleBG& _other) = delete; // default Copy constructer ����Ʈ ���������
	TitleBG(TitleBG&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������
	TitleBG& operator=(const TitleBG& _other) = delete; // default Copy operator ����Ʈ ���� ������
	TitleBG& operator=(const TitleBG&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����
	
protected:
	virtual void Start();
	virtual void Update(float _DeltaTime);

private:
	GameEngineImageRenderer* renderer_;

};

