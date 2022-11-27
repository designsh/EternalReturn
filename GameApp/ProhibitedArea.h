#pragma once
#include "Enums.h"
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFBXRenderer.h>
#include <GameEngine/GameEngineCollision.h>

// �з� : 
// �뵵 : 
// ���� : 
class ProhibitedArea : public GameEngineActor
{
public:
	ProhibitedArea(); // default constructer ����Ʈ ������
	~ProhibitedArea(); // default destructer ����Ʈ �Ҹ���
	ProhibitedArea(const ProhibitedArea& _other) = delete; // default Copy constructer ����Ʈ ���������
	ProhibitedArea(ProhibitedArea&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������
	ProhibitedArea& operator=(const ProhibitedArea& _other) = delete; // default Copy operator ����Ʈ ���� ������
	ProhibitedArea& operator=(const ProhibitedArea&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

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

