#pragma once
#include <GameEngine/GameEngineActor.h>

// 분류 : 마우스
// 용도 : UI 상호 및 플레이어 이동
// 설명 : 
class GameEngineUIRenderer;
class GameEngineCollision;
class Character;
class Monsters;
class ItemBase;
class MousePointer : public GameEngineActor
{
public: // Static Value
	static MousePointer* InGameMouse;

public: // Inline Get Function
	inline float4 GetRayDirection()
	{
		return rayDirection_;
	}

	inline float4 GetRayOrigin()
	{
		return rayOrigin_;
	}

	inline GameEngineCollision* GetRayCollision()
	{
		return rayCollision_;
	}

	inline bool isGrabbed()
	{
		return isGrabbed_;
	}

public: // Public Function
	float4 GetIntersectionYAxisPlane(float _height, float _rayLength);
	GameEngineCollision* GetPickCollision(int _Order);

	void GrabItem(ItemBase* _item, GameEngineUIRenderer* _icon);

	ItemBase* PutItem();

private:
	void updateMouseRay();

private: // Functions related to outline activation and deactivation
	void CheckCollision();
	void CheckCharacterCollision();
	void CheckMonsterCollision();

private:
	void Start() override;
	void Update(float _deltaTime) override;
	void LevelChangeStartEvent(GameEngineLevel* _PrevLevel) override;
	void LevelChangeEndEvent(GameEngineLevel* _NextLevel) override;

public:
	MousePointer();
	~MousePointer();

protected:
	MousePointer(const MousePointer& _other) = delete;
	MousePointer(MousePointer&& _other) = delete;

private:
	MousePointer& operator=(const MousePointer& _other) = delete;
	MousePointer& operator=(const MousePointer&& _other) = delete;

private: // Ray(InGame)
	float4 rayDirection_;
	float4 rayOrigin_;
	GameEngineCollision* rayCollision_;

private: // Mouse(UI)
	GameEngineUIRenderer* MouseRenderer_;
	GameEngineCollision* MouseCollider_;

private: // Object That Collided with the Previous Ray
	Character* PrevColCharacter_;
	Monsters* PrevColMonster_;

private: // Grabbed Item
	bool isGrabbed_;
	ItemBase* item_;
	GameEngineUIRenderer* itemIconRenderer_;

};

