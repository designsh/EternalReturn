#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineUIRenderer.h>

using namespace std;

class UI_HyperMap : public GameEngineActor
{
public:
	UI_HyperMap();
	~UI_HyperMap();

	Location GetSelectLocation(float4 _Position);

	Location ReturnSelectedLocation();

	void MapOff();
	void MapOn();

	void SelectedAreaRender(float4 _Position);

	bool MapOnFlag;

public:
	GameEngineUIRenderer* fullMapRenderer_;
	GameEngineUIRenderer* areaChoiceMapRenderer_;
	GameEngineUIRenderer* selectAreaRenderer_;

	GameEngineCollision* textureCollision_;

protected:
	virtual void Start();
	virtual void Update(float _DeltaTime);

private:
	UI_HyperMap(const UI_HyperMap& _Other) = delete;
	UI_HyperMap(UI_HyperMap&& _Other) noexcept = delete;
	UI_HyperMap& operator=(const UI_HyperMap& _Other) = delete;
	UI_HyperMap& operator=(UI_HyperMap&& _Other) noexcept = delete;

private:
	//현재 내가 선택한 지역을 저장해둔 멤버변수입니다
	Location SelectedLocation;

	float4 MapPos;
};
