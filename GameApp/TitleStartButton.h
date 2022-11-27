#pragma once
#include <GameEngine/GameEngineActor.h>

// �з� : 
// �뵵 : 
// ���� : 
class GameEngineImageRenderer;
class GameEngineCollision;
class TitleStartButton : public GameEngineActor
{
public:
	TitleStartButton(); // default constructer ����Ʈ ������
	~TitleStartButton(); // default destructer ����Ʈ �Ҹ���
	TitleStartButton(const TitleStartButton& _other) = delete; // default Copy constructer ����Ʈ ���������
	TitleStartButton(TitleStartButton&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������
	TitleStartButton& operator=(const TitleStartButton& _other) = delete; // default Copy operator ����Ʈ ���� ������
	TitleStartButton& operator=(const TitleStartButton&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:
	bool MouseCollisionCheck();

	void SetImageByIndex(int _index);

protected:
	virtual void Start();
	virtual void Update(float _DeltaTime);

private:
	GameEngineImageRenderer* renderer_;
	GameEngineCollision* collision_;


};

