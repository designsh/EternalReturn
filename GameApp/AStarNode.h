#pragma once

#include <GameEngineBase/GameEngineMath.h>
#include <stack>

// �ڽ�Ʈ�� float �� �Ǿ� �ִµ� �����ٸ� ������ �ٲٴ� ���� ���
class AStarNode
{
public:
	// ����� ��ġ�� ������ ���� ���̱� ������ ������ �� �ʱ�ȭ�Ѵ�.
	AStarNode(int _index1D, int _zIndex, int _xIndex, const float4& _position);
	~AStarNode();
	AStarNode(const AStarNode& _other);
	AStarNode(AStarNode&& _other) noexcept;
	AStarNode& operator=(const AStarNode& _other) = delete;
	AStarNode& operator=(const AStarNode&& _other) noexcept;

public:
	// ������κ��� _other ���� ������ G ���� ��ȯ�Ѵ�.
	float GetEvaluatedGCostToOhterNode(const AStarNode& _other);

	// ��� ����� f���� ���Ѵ�.
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

