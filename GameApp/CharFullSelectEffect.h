#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>
#include <GameEngine/GameEngineUIRenderer.h>
// 분류 : 
// 용도 : 
// 설명 : 
class CharFullSelectEffect : public GameEngineActor
{
public:
	CharFullSelectEffect(); // default constructer 디폴트 생성자
	~CharFullSelectEffect(); // default destructer 디폴트 소멸자
	CharFullSelectEffect(const CharFullSelectEffect& _other) = delete; // default Copy constructer 디폴트 복사생성자
	CharFullSelectEffect(CharFullSelectEffect&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자
	CharFullSelectEffect& operator=(const CharFullSelectEffect& _other) = delete; // default Copy operator 디폴트 대입 연산자
	CharFullSelectEffect& operator=(const CharFullSelectEffect&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:
	void PlayAwake();
	void SetCharImage(JobType _type);

protected:
	void Start() override;
	void Update(float _deltaTime) override;


protected:
	void startSleep();
	void updateSleep(float _deltaTime);

	void startAwake();
	void updateAwake(float _deltaTime);

private:
	GameEngineFSM renderState_;
	GameEngineUIRenderer* charStencilRenderer_;
	GameEngineUIRenderer* glowLineRenderer_;
	GameEngineUIRenderer* ringRenderer_;

private:
	float timer_;
	float glowTimer_;
	float rotationDegree_;
	float4 scaleContainer_;
};

