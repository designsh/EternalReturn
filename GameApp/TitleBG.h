#pragma once
#include <GameEngine/GameEngineActor.h>
// 분류 : 
// 용도 : 
// 설명 : 

class GameEngineImageRenderer;
class GameEngineUIRenderer;
class TitleBG : public GameEngineActor
{
public:
	TitleBG(); // default constructer 디폴트 생성자
	~TitleBG(); // default destructer 디폴트 소멸자
	TitleBG(const TitleBG& _other) = delete; // default Copy constructer 디폴트 복사생성자
	TitleBG(TitleBG&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자
	TitleBG& operator=(const TitleBG& _other) = delete; // default Copy operator 디폴트 대입 연산자
	TitleBG& operator=(const TitleBG&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자
	
protected:
	virtual void Start();
	virtual void Update(float _DeltaTime);

private:
	GameEngineImageRenderer* renderer_;

};

