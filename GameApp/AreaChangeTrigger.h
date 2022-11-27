#pragma once
#include <GameEngine/GameEngineActor.h>
// 분류 : 
// 용도 : 
// 설명 : 
class GameEngineCollision;
class AreaChangeTrigger : public GameEngineActor
{
public:
	AreaChangeTrigger(); // default constructer 디폴트 생성자
	~AreaChangeTrigger(); // default destructer 디폴트 소멸자
	AreaChangeTrigger(const AreaChangeTrigger& _other) = delete; // default Copy constructer 디폴트 복사생성자
	AreaChangeTrigger(AreaChangeTrigger&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자
	AreaChangeTrigger& operator=(const AreaChangeTrigger& _other) = delete; // default Copy operator 디폴트 대입 연산자
	AreaChangeTrigger& operator=(const AreaChangeTrigger&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:
	void SetLocations(Location _loca0, const std::string& _loca0Name, Location _loca1, const std::string& _loca1Name);
	void SetCollision(float4 _entry0Pos, float4 _entryScale, float4 _entry1Pos);

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	Location entryPoint0_;
	Location entryPoint1_;

	std::string entryPointName0_;
	std::string entryPointName1_;

	GameEngineCollision* entryCollision0_;
	GameEngineCollision* entryCollision1_;

	bool bIsIn_;
	bool bIsIn2_;
};

