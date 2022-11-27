#pragma once

#include <GameEngine/GameEngineActor.h>
#include "AStarNode.h"

#include <queue>

class GameEngineEffectRenderer;
class GameEngineFBXRenderer;
class NavMesh;
class LumiaMap : public GameEngineActor
{
public:
	// 플레이어 거리 기준
	const float RENDER_RANGE = 1300.f;

public:
	LumiaMap();
	~LumiaMap();
	LumiaMap(const LumiaMap& _other) = delete;
	LumiaMap(LumiaMap&& _other) = delete;
	LumiaMap& operator=(const LumiaMap& _other) = delete;
	LumiaMap& operator=(const LumiaMap&& _other) = delete;

public:
	void Start() override;
	void Update(float _deltaTime) override;

	std::string GetMapName(int _index)
	{
		return enum_MapName[_index];
	}

public:
	std::vector<float4> FindPath(const float4& _startPosition, const float4& _endPosition);

	void UpdateAStarNodeVertexInfo();

	NavMesh* GetNavMesh() { return navMesh_; }

	std::vector<float4> GetHyperLoopSpawnPoints()
	{
		return hyperLoopSpawnPoints_;
	}
	
	std::vector<float4> GetCharacterSpawnPoints(int _spawnArea);

	std::vector<float4> GetEyeSightPolygon(const float4& _position, const float _length = 600.f);

	void ToggleNavMeshRender();

protected:
	void makeAStarNode(float _intervalX, float _intervalZ);
	void checkASterNodeObstacle();
	void setAllItem();
	void setCharacterSpawnPoints(GameEngineDirectory _dir);
	void setHyperLoopSpawnPoints(GameEngineDirectory _dir);

protected:
	// A*
	std::vector<std::vector<AStarNode>> allNodes_;
	std::vector<GameEngineVertex> tileVertices_;
	std::vector<UINT> tileIndices_;
	GameEngineVertexBuffer* tileVertexBuffer_;
	GameEngineIndexBuffer* tileIndexBuffer_;

	float intervalX_;
	float intervalZ_;
	float gridStartX_;
	float gridStartZ_;
	int gridZCount_;
	int gridXCount_;

	// Rendering Object
	GameEngineFBXRenderer* navMeshRenderer_;
	NavMesh* navMesh_;
	GameEngineFBXRenderer* downTownRenderer_;
	GameEngineRenderer* navTileRenderer_;
	std::vector<GameEngineFBXRenderer*> mapRenderers;
	GameEngineEffectRenderer* areaOutlineRenderer_;

private:
	std::map<std::string, std::vector<float4>> characterSpawnPoints_;	// Area, Position
	std::vector<float4> hyperLoopSpawnPoints_;

	std::vector<std::string> enum_MapName;

	float mapScale_;

};

