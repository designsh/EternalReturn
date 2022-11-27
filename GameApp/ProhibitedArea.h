#pragma once
#include "Enums.h"
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFBXRenderer.h>
#include <GameEngine/GameEngineCollision.h>

// 분류 : 
// 용도 : 
// 설명 : 
class ProhibitedArea : public GameEngineActor
{
public:
	ProhibitedArea(); // default constructer 디폴트 생성자
	~ProhibitedArea(); // default destructer 디폴트 소멸자
	ProhibitedArea(const ProhibitedArea& _other) = delete; // default Copy constructer 디폴트 복사생성자
	ProhibitedArea(ProhibitedArea&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자
	ProhibitedArea& operator=(const ProhibitedArea& _other) = delete; // default Copy operator 디폴트 대입 연산자
	ProhibitedArea& operator=(const ProhibitedArea&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:
	void Init(Location _location, const std::string& _fbxName, float4 _pos, const std::string& _name);
	void SetProhibited(bool _tf);
	Location GetLocation() { return location_; }
	bool IsProhibited() { return isProhibited_; }



	const std::string& GetKoreanName() { return locationName_; }

	void Start() override;
	void Update(float _DeltaTime) override;

private:
	Location location_;
	std::string locationName_;
	bool isProhibited_;

	GameEngineFBXRenderer* renderer_;
};

