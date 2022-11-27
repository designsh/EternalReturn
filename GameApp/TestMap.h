#pragma once

#include "LumiaMap.h"

class TestMap : public LumiaMap
{
public:
	TestMap(); 
	~TestMap();

	TestMap(const TestMap& _other) = delete; 
	TestMap(TestMap&& _other) = delete; 

	TestMap& operator=(const TestMap& _other) = delete;
	TestMap& operator=(const TestMap&& _other) = delete;

public:
	void Start() override;
	void Update(float _deltaTime);

	void makeAStarNode(float _intervalX, float _intervalZ);

private:
	GameEngineRenderer* renderer_;
};

