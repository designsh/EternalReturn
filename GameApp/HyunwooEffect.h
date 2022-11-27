#pragma once
#include <GameEngine/GameEngineActor.h>

// 분류 : 
// 용도 : 
// 설명 : 

class Hyunwoo;
class GameEngineImageRenderer;
class HyunwooEffect : public GameEngineActor
{
	friend Hyunwoo;

public:
	HyunwooEffect(); // default constructer 디폴트 생성자
	~HyunwooEffect(); // default destructer 디폴트 소멸자
	HyunwooEffect(const HyunwooEffect& _other) = delete; // default Copy constructer 디폴트 복사생성자
	HyunwooEffect(HyunwooEffect&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자
	HyunwooEffect& operator=(const HyunwooEffect& _other) = delete; // default Copy operator 디폴트 대입 연산자
	HyunwooEffect& operator=(const HyunwooEffect&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:

protected:
	GameEngineImageRenderer* frontRenderer_;

	void Start() override;
	void Update(float _deltaTime) override;



private:
};

