#pragma once
#include <GameEngine/GameEngineActor.h>

// 분류 : 
// 용도 : 
// 설명 : 
class GameEngineImageRenderer;
class GameEngineCollision;
class TitleStartButton : public GameEngineActor
{
public:
	TitleStartButton(); // default constructer 디폴트 생성자
	~TitleStartButton(); // default destructer 디폴트 소멸자
	TitleStartButton(const TitleStartButton& _other) = delete; // default Copy constructer 디폴트 복사생성자
	TitleStartButton(TitleStartButton&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자
	TitleStartButton& operator=(const TitleStartButton& _other) = delete; // default Copy operator 디폴트 대입 연산자
	TitleStartButton& operator=(const TitleStartButton&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

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

