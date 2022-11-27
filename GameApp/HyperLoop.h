#pragma once
#include <GameEngine/GameEngineActor.h>

//10.27 이건호 하이퍼 루프 맵 UI 추가
#include "UI_HyperMap.h"

// 설명 : 
class HyperLoop : public GameEngineActor
{
public:
	HyperLoop();
	~HyperLoop();
	HyperLoop(const HyperLoop& _other) = delete; 
	HyperLoop(HyperLoop&& _other) noexcept = delete;
	HyperLoop& operator=(const HyperLoop& _other) = delete;
	HyperLoop& operator=(const HyperLoop&& _other) = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	GameEngineFBXRenderer* renderer_;
	GameEngineCollision* collision_;

	UI_HyperMap* mapUI_;

	Location SelectedArea;

	
};

