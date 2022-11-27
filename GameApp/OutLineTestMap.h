#pragma once
#include "LumiaMap.h"

// �з� : ��
// �뵵 : �׽�Ʈ
// ���� : 
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

