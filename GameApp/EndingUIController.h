#pragma once
#include <GameEngine/GameEngineUIRenderer.h>
#include <GameEngine/GameEngineActor.h>
// 분류 : 
// 용도 : 
// 설명 : 
class EndingUIController : public GameEngineActor
{
public:
	EndingUIController(); // default constructer 디폴트 생성자
	~EndingUIController(); // default destructer 디폴트 소멸자
	EndingUIController(const EndingUIController& _other) = delete; // default Copy constructer 디폴트 복사생성자
	EndingUIController(EndingUIController&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자
	EndingUIController& operator=(const EndingUIController& _other) = delete; // default Copy operator 디폴트 대입 연산자
	EndingUIController& operator=(const EndingUIController&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

protected:
	void Start() override;
	void Update(float _DeltaTime) override;



public:
	GameEngineUIRenderer* GetBackGroundRenderer() { return backgroundRenderer_; }
	GameEngineUIRenderer* GetDeskRenderer() { return deskRenderer_; }
	GameEngineUIRenderer* GetPersonRenderer() { return personRenderer_; }
	GameEngineUIRenderer* GetCreditTextRenderer() { return creditTextRenderer_; }
	GameEngineUIRenderer* GetFoxteamRenderer() { return foxTeamRenderer_; }
	void ResourceLoad();
private:
	GameEngineUIRenderer* backgroundRenderer_;
	GameEngineUIRenderer* deskRenderer_;
	GameEngineUIRenderer* personRenderer_;
	GameEngineUIRenderer* creditTextRenderer_;
	GameEngineUIRenderer* foxTeamRenderer_;
};

