#pragma once
#include "GameEngineTransformComponent.h"
#include "GameEngineShaderResHelper.h"
#include "Enums.h"

// �̳༮�鸸�� �׷��� ����ڽ��ϴ�.


// ���� :
class GameEngineCore;
class GameEngineLevel;
class GameEngineFBXMesh;
class GameEngineCollision : public GameEngineTransformComponent
{
	friend GameEngineCore;
	friend GameEngineLevel;

	static std::function<bool(GameEngineTransform*, GameEngineTransform*)> 
		CollisionCheckFunction[static_cast<int>(CollisionType::MAX)][static_cast<int>(CollisionType::MAX)];

	static void Init();

public:
	static bool CirCleToCirCle(GameEngineTransform* _Left, GameEngineTransform* _Right);
	static bool Sphere3DToSphere3D(GameEngineTransform* _Left, GameEngineTransform* _Right);

	static bool RectToRect(GameEngineTransform* _Left, GameEngineTransform* _Right);
	static bool AABBToAABB(GameEngineTransform* _Left, GameEngineTransform* _Right);

	static bool OBBToOBB(GameEngineTransform* _Left, GameEngineTransform* _Right);
	static bool CirCleToAABB(GameEngineTransform* _Left, GameEngineTransform* _Right);
	static bool AABBToCirCle(GameEngineTransform* _Left, GameEngineTransform* _Right);

	static bool OBBToAABB(GameEngineTransform* _Left, GameEngineTransform* _Right);
	static bool AABBToOBB(GameEngineTransform* _Left, GameEngineTransform* _Right);

	static bool RayToAABB(GameEngineTransform* _left, GameEngineTransform* _right);
	static bool AABBToRay(GameEngineTransform* _left, GameEngineTransform* _right);

	static bool RayToOBB(GameEngineTransform* _left, GameEngineTransform* _right);
	static bool OBBToRay(GameEngineTransform* _left, GameEngineTransform* _right);

	static bool RayToSphere(GameEngineTransform* _left, GameEngineTransform* _right);
	static bool SphereToRay(GameEngineTransform* _left, GameEngineTransform* _right);

	static bool SphereToOBB(GameEngineTransform* _left, GameEngineTransform* _right);
	static bool OBBToSphere(GameEngineTransform* _left, GameEngineTransform* _right);

	static bool CirCleToOBB(GameEngineTransform* _Left, GameEngineTransform* _Right);
	static bool OBBToCirCle(GameEngineTransform* _Left, GameEngineTransform* _Right);
//======================================== SJH
private: // Ÿ�Ժ� �������� �����˻�
	bool OBBBoxToRay(const float4& _RayOriginPos, const float4& _RayDirection, float& _Dist);
	bool Sphere3DToRay(const float4& _RayOriginPos, const float4& _RayDirection, float& _Dist);
	bool AABBBoxToRay(const float4& _RayOriginPos, const float4& _RayDirection, float& _Dist);

public: // �ٿ���ڽ�,���� �̿��� �������� �浹üũ
	bool BoundingToRayCollision(CollisionType _ThisType, const float4& _RayOriginPos, const float4& _RayDirection, float& _Dist);
	bool BoundingToRayCollision(const float4& _RayOriginPos, const float4& _RayDirection, float& _Dist);

public:
	inline CollisionType GetCollisionType()
	{
		return ColType_;
	}
//======================================== SJH

public:
	inline void SetRayData(const float4& _origin, const float4& _direction) { GetTransform()->SetRayData(_origin, _direction); }

public:
	// constrcuter destructer
	GameEngineCollision();
	~GameEngineCollision();

	// delete Function
	GameEngineCollision(const GameEngineCollision& _Other) = delete;
	GameEngineCollision(GameEngineCollision&& _Other) noexcept = delete;
	GameEngineCollision& operator=(const GameEngineCollision& _Other) = delete;
	GameEngineCollision& operator=(GameEngineCollision&& _Other) noexcept = delete;

	template<typename UserType>
	void SetCollisionGroup(UserType _Type)
	{
		SetCollisionGroup(static_cast<int>(_Type));
	}

	void SetCollisionGroup(int _Type);

	void Collision(
		CollisionType _ThisType, 
		CollisionType _OtherType, 
		int _OtherGroup, 
		std::function<void(GameEngineCollision*)> _CallBack
	);


	void SphereToSphereCollision(int _OtherGroup, std::function<void(GameEngineCollision*)> _CallBack);

	/// ���� ///
	void SetCollisionType(CollisionType _ColType)
	{
		ColType_ = _ColType;
	}

	void SetCollisionInfo(int _Type, CollisionType _ColType)
	{
		SetCollisionGroup(_Type);
		SetCollisionType(_ColType);
	}

	bool Collision(int _OtherGroup);
	bool Collision(GameEngineCollision* _OtherCollision);
	template <typename T> bool Collision(T _OtherGroup);

	GameEngineCollision* GetCollision(int _OtherGroup);
	template<typename T> GameEngineCollision* GetCollision(T _otherGroup);
	std::list<GameEngineCollision*> GetCollisionList(int _OtherGroup);
	template<typename T> std::list<GameEngineCollision*> GetCollisionList(T _OtherGroup);

	// ������ 0728
	// �ݸ������� "�Ҽ�" �� ����ؼ�
	// ĳ���� �� ���Ϳ��� ������ִ� �ݸ������� �ϰ����� �Ҽ��� �ο���
	// �ݸ��� üũ ���������, ���� �Ҽӿ� ��� �ִ� �ݸ����� Ž���� ��� �н��ϴ� �ý�����.
	// �� �ʿ��Ѱ�? 
	// ĳ���� ������ ĳ���� �ݸ���, ���� ���� �ݸ����� ������ �ְ�,
	// ���� ���� �ݸ��� ���� ĳ���� �ݸ����� ������ ������ �õ��� �� ���� ��,
	// �ڱ� ĳ���� �ݸ����� �ڱ� ���� ���� �ݸ����� �ν����� �ʱ� ����...


protected:
	CollisionType ColType_;
	///  ���� ///

private:
	void Start() override;
	void Update(float _DeltaTime) override;
};

template<typename T>
inline bool GameEngineCollision::Collision(T _OtherGroup) { return Collision(static_cast<int>(_OtherGroup)); }

template<typename T>
inline GameEngineCollision* GameEngineCollision::GetCollision(T _otherGroup)
{
	return GetCollision(static_cast<int>(_otherGroup));
}

template<typename T>
inline std::list<GameEngineCollision*> GameEngineCollision::GetCollisionList(T _OtherGroup)
{
	return GetCollisionList(static_cast<int>(_OtherGroup));
}
