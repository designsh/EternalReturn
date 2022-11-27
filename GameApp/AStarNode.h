#pragma once

#include <GameEngineBase/GameEngineMath.h>
#include <stack>

// 코스트가 float 로 되어 있는데 느리다면 정수로 바꾸는 것을 고려
class AStarNode
{
public:
	// 노드의 위치가 변하지 않을 것이기 때문에 생성할 때 초기화한다.
	AStarNode(int _index1D, int _zIndex, int _xIndex, const float4& _position);
	~AStarNode();
	AStarNode(const AStarNode& _other);
	AStarNode(AStarNode&& _other) noexcept;
	AStarNode& operator=(const AStarNode& _other) = delete;
	AStarNode& operator=(const AStarNode&& _other) noexcept;

public:
	// 현재노드로부터 _other 노드로 갈때의 G 값을 반환한다.
	float GetEvaluatedGCostToOhterNode(const AStarNode& _other);

	// 헤당 노드의 f값을 평가한다.
	void EvaluateCost(const AStarNode& _other, const AStarNode& _endNode);

	bool IsObstacle() { return bObstacle_; }

	void SetObstacle(bool _bObstacle) { bObstacle_ = _bObstacle; }
	void SetZIndex(int _zIndex) { zIndex_ = _zIndex; }
	void SetXIndex(int _xIndex) { xIndex_ = _xIndex; }
	void SetParent(AStarNode* _parent);

	float GetGCost() { return gCost_; }
	float GetHCost() { return hCost_; }
	float GetFCost() { return fCost_; }

	float4 GetPosition() { return position_; }
	int GetIndex() { return index_; }
	int GetZIndex() { return zIndex_; }
	int GetXIndex() { return xIndex_; }

	std::vector<float4> GetPath();

private:
	int index_;
	int zIndex_;
	int xIndex_;
	float4 position_;
	bool bObstacle_;

	float gCost_;
	float hCost_;
	float fCost_;

	AStarNode* parent_;
};

