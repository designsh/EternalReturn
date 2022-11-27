#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineUIRenderer.h>

using namespace std;

class LocationNode : public GameEngineActor
{
	// 접속된 캐릭터 수만큼 활성화되는 클래스
	// 맵에 꽂힐 핀을 렌더러로 가진다.
	// 겹치면??
	friend class Lobby_Map;

public:
	LocationNode();
	~LocationNode();

protected:
	GameEngineUIRenderer* nodeRenderer_;
	Location SelectedLocation;

protected:
	virtual void Start();
	virtual void Update(float _DeltaTime);

public:
	int GetSelectedLocation() { return static_cast<int>(SelectedLocation); }
	GameEngineUIRenderer* GetRenderer() { return nodeRenderer_; }
	void SetRenderer(GameEngineUIRenderer* _renderer) { nodeRenderer_ = _renderer; }
	void ChangeLocation(int _location) { SelectedLocation = static_cast<Location>(_location); }
};


class NodeCapacityCounter : public GameEngineActor
{
	friend class Lobby_Map;

public:
	NodeCapacityCounter();
	~NodeCapacityCounter();

public:
	GameEngineUIRenderer* GetRenderer() { return counterRenderer_; }
	void SetCounter(int _count);
	std::string GetCount() { return count_; }


protected:
	GameEngineUIRenderer* counterRenderer_;

protected:
	virtual void Start();
	virtual void Update(float _DeltaTime);

private:
	std::string count_;

};


class Lobby_Map : public GameEngineActor
{
public:
	Lobby_Map();
	~Lobby_Map();

	int GetSelectLocation(float4 _Position);
	void ResizeMyPin();

	GameEngineCollision* GetCollision() { return textureCollision_; }
	std::vector<LocationNode*> GetNodeList() { return selectAreaNodeList_; }
	std::vector<NodeCapacityCounter*> GetCapacityCounterList() { return capacityCounterList_; }
	void ArrangeCounter();

public:
	GameEngineUIRenderer* fullMapRenderer_;

	GameEngineUIRenderer* areaChoiceMapRenderer_;
	
	std::vector<LocationNode*> selectAreaNodeList_;
	std::vector<NodeCapacityCounter*> capacityCounterList_;
	// 0~14 까지가 사용되는 로케이션

	GameEngineCollision* textureCollision_;

protected:
	virtual void Start();
	virtual void Update(float _DeltaTime);

private:
	Lobby_Map(const Lobby_Map& _Other) = delete;
	Lobby_Map(Lobby_Map&& _Other) noexcept = delete;
	Lobby_Map& operator=(const Lobby_Map& _Other) = delete;
	Lobby_Map& operator=(Lobby_Map&& _Other) noexcept = delete;
};

