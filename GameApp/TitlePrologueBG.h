#pragma once
#include <GameEngine/GameEngineActor.h>
// 분류 : 
// 용도 : 
// 설명 : 
class GameEngineImageRenderer;
class TitlePrologueBG : public GameEngineActor
{
public:
	TitlePrologueBG(); // default constructer 디폴트 생성자
	~TitlePrologueBG(); // default destructer 디폴트 소멸자
	TitlePrologueBG(const TitlePrologueBG& _other) = delete; // default Copy constructer 디폴트 복사생성자
	TitlePrologueBG(TitlePrologueBG&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자
	TitlePrologueBG& operator=(const TitlePrologueBG& _other) = delete; // default Copy operator 디폴트 대입 연산자
	TitlePrologueBG& operator=(const TitlePrologueBG&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:
	void SetAlpha(float _Alpha);

protected:
	virtual void Start();
	virtual void Update(float _DeltaTime);

private:
	GameEngineImageRenderer* renderer_;

};

