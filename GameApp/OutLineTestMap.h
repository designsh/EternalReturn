#pragma once
#include "LumiaMap.h"

// 분류 : 맵
// 용도 : 테스트
// 설명 : 
class OutLineTestMap : public LumiaMap
{
public:
	void Start() override;
	void Update(float _deltaTime);

protected:
private:
	void makeAStarNode(float _intervalX, float _intervalZ);

public:
	OutLineTestMap();
	~OutLineTestMap();

protected:
	OutLineTestMap(const OutLineTestMap& _other) = delete;
	OutLineTestMap(OutLineTestMap&& _other) = delete;

private:
	OutLineTestMap& operator=(const OutLineTestMap& _other) = delete;
	OutLineTestMap& operator=(const OutLineTestMap&& _other) = delete;

public:
protected:
private:
	GameEngineRenderer* Renderer_;
};

