#pragma once

#include "GameEngineTransformComponent.h"

class GameEngineBoneComponent : public GameEngineTransformComponent
{
public:
	GameEngineBoneComponent(); 
	~GameEngineBoneComponent();

	GameEngineBoneComponent(const GameEngineBoneComponent& _other) = delete; 
	GameEngineBoneComponent(GameEngineBoneComponent&& _other) = delete; 

	GameEngineBoneComponent& operator=(const GameEngineBoneComponent& _other) = delete;
	GameEngineBoneComponent& operator=(const GameEngineBoneComponent&& _other) = delete;

public:
	virtual void Start() override;
	virtual void Update(float _deltaTime) override;

public:
	void SetParentBoneName(GameEngineFBXRenderer* _fbxRenderer, const std::string& _boneNameToAffect);

private:
	GameEngineFBXRenderer* parentBoneRenderer_;
	int parentBoneIndex_;
};

