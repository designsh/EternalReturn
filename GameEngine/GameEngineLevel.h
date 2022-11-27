#pragma once
#include <list>
#include <map>
#include <GameEngine\Enums.h>
#include "GameEngineActor.h"
#include "GameEnginePostProcessRender.h"

// �з� : ����
// �뵵 : 
// ���� : �ϳ��� ������ �����ϰ� ������ ��ġ�Ǵ� ���͸� �����ϴ� ��� ����
class GameEngineCore;
class CameraActor;
class CameraComponent;
class GameEngineRenderer;
class GameEngineTransform;
class GameEngineCollision;
class GameEngineDebugRenderData;
class GameEngineLightComponent;
class GameEngineLevelControlWindow;
class GameEngineDepthBuffer;
class GameEngineLevel : public GameEngineObjectNameBase
{
	friend GameEngineLevelControlWindow;
	friend GameEngineCore;
	friend GameEngineRenderer;
	friend GameEngineCollision;
	friend GameEngineLightComponent;
	friend CameraComponent;

	class NextLevelActor
	{
		public:
			GameEngineActor* Actor;
			GameEngineLevel* Level;
	};

public: // Public Inline Get Function
	inline std::map<int, std::list<GameEngineCollision*>>& GetAllCollision()
	{
		return CollisionList_;
	}

	inline std::list<GameEngineCollision*>& GetCollisionGroup(int _Group)
	{
		return CollisionList_[_Group];
	}

public: // Public Get Function
	CameraActor* GetMainCameraActor();
	CameraComponent* GetMainCamera();
	CameraActor* GetUICameraActor();
	CameraComponent* GetUICamera();
	GameEngineTexture* GetShadowTexture();

public: // Public Set Function
	void SetLevelActorMove(GameEngineLevel* _NextLevel, GameEngineActor* _Actor);

public: // 
	void ActorUpdate(float _DeltaTime);
	void Render(float _DeltaTime);

public: // Public Release Related Function
	void Release(float _DeltaTime);
	void ClearAll();

public: // Public Push Related Function
	void PushCollision(GameEngineCollision* _Collision, int _Group);
	void PushDebugRender(GameEngineTransform* _Transform, CollisionType _Type, float4 _Color = float4::GREEN);
	void PushDebugRenderUI(GameEngineTransform* _Transform, CollisionType _Type, float4 _Color = float4::GREEN);

public: // Public TimeEventer Related Function
	void AddTimeEvent(float _Time, std::function<void()> _Event);

public: // Public CollisionGroup Related Function
	void ChangeCollisionGroup(int _Group, GameEngineCollision* _Collision);

public: // Public Template Function
	template<typename ActorType>
	ActorType* CreateActor(int _UpdateOrder = 0);

	template<typename ActorType>
	ActorType* CreateActor(std::string _Name, int _UpdateOrder = 0);

	template<typename UserEnumType>
	void PushCollision(GameEngineCollision* _Collision, UserEnumType _Group);

	template<typename PostProcess, typename ... Parameter>
	PostProcess* AddPostProcessCameraMergePrev(Parameter ... _Arg);

	template<typename PostProcess, typename ... Parameter >
	PostProcess* AddPostProcessCameraMergeNext(Parameter ... _Arg);

	template<typename PostProcess, typename ... Parameter >
	PostProcess* AddPostProcess(const std::string& _Key, Parameter ... _Arg);

protected: // Protected Pure Virtual Function
	virtual void LevelStart() = 0;
	virtual void LevelUpdate(float _DeltaTime) = 0;
	virtual void LevelChangeEndEvent(GameEngineLevel* _NextLevel) = 0;
	virtual void LevelChangeStartEvent(GameEngineLevel* _PrevLevel) = 0;

protected:
private: // Private Level Init : ���������� 1�� ȣ��
	void Init();

private: // Private Actor Level Move Related Function
	void SetLevelActorMoveProcess();
	void LevelChangeEndActorEvent(GameEngineLevel* _NextLevel);
	void LevelChangeStartActorEvent(GameEngineLevel* _PrevLevel);

private: // Private TimeEventer Update Function
	void TimeEventUpdate();

private: // Private ShadowTarget Release Function(�����׸���ó��)
	void ReleaseLightShadowTarget();

public:
	GameEngineLevel();
	~GameEngineLevel();

protected:
	GameEngineLevel(const GameEngineLevel& _Other) = delete;
	GameEngineLevel(GameEngineLevel&& _Other) noexcept = delete;

private:
	GameEngineLevel& operator=(const GameEngineLevel& _Other) = delete;
	GameEngineLevel& operator=(GameEngineLevel&& _Other) noexcept = delete;

protected:
	CameraActor* MainCameraActor_;
	CameraActor* UICameraActor_;

private:
	std::map<int, std::list<GameEngineActor*>> ActorList_;
	std::vector<NextLevelActor> NextLevelActorsData_;
	
	std::map<int, std::list<GameEngineCollision*>> CollisionList_;
	std::map<std::string, std::vector<GameEnginePostProcessRender*>> PostRender;

	std::list<TimeEvent*> AllEvent_;
	std::list<TimeEvent*> AddEvent_;

private: // Shadow Processing Related Value(���� �׸��� ó��)
	GameEngineTexture* LightShadowTexture_;												// ���� �������� ����ϴ� ��� ����(��)�� �������� �迭�� ���� �ؽ��� 1��			: ����(��) �������� �����Ǿ��ٰ� �����
	std::vector<GameEngineLightComponent*> AllLights_;									// ���� �������� ����ϴ� ��� ����(��)�� ���									: �� ī�޶󿡼� ��������
	std::vector<ID3D11RenderTargetView*> LightShadowRenderTargets_;						// ���� �������� ����ϴ� ��� ����(��)�� ������ ����Ÿ���� ���ҽ�				: 
	std::vector<GameEngineDepthBuffer*> LightShadowDepths_;								// ���� �������� ����ϴ� ��� ����(��)�� ������ ����Ÿ���� ����/���ٽ� ���ҽ�	: 

private:
};

template<typename UserEnumType>
inline void GameEngineLevel::PushCollision(GameEngineCollision* _Collision, UserEnumType _Group)
{
	PushCollision(_Collision, static_cast<int>(_Group));
}

template<typename PostProcess, typename ...Parameter>
inline PostProcess* GameEngineLevel::AddPostProcessCameraMergePrev(Parameter ..._Arg)
{
	return AddPostProcess<PostProcess>("CameraMergePrev", _Arg...);
}

template<typename PostProcess, typename ...Parameter>
inline PostProcess* GameEngineLevel::AddPostProcessCameraMergeNext(Parameter ..._Arg)
{
	return AddPostProcess<PostProcess>("CameraMergeNext", _Arg...);
}

template<typename PostProcess, typename ...Parameter>
inline PostProcess* GameEngineLevel::AddPostProcess(const std::string& _Key, Parameter ..._Arg)
{
	PostProcess* NewPost = new PostProcess(_Arg...);
	GameEnginePostProcessRender* ParentType = dynamic_cast<GameEnginePostProcessRender*>(NewPost);
	ParentType->Initialize();
	PostRender[_Key].push_back(NewPost);
	return NewPost;
}

template<typename ActorType>
inline ActorType* GameEngineLevel::CreateActor(int _UpdateOrder)
{
	GameEngineActor* NewActor = new ActorType();
	NewActor->SetLevel(this);
	NewActor->Start();
	NewActor->SetOrder(_UpdateOrder);

	// Insert + Find
	std::list<GameEngineActor*>& List = ActorList_[_UpdateOrder];
	List.push_back(NewActor);
	return dynamic_cast<ActorType*>(NewActor);
}

template<typename ActorType>
inline ActorType* GameEngineLevel::CreateActor(std::string _Name , int _UpdateOrder)
{
	GameEngineActor* NewActor = new ActorType();
	NewActor->SetLevel(this);
	NewActor->Start();
	NewActor->SetOrder(_UpdateOrder);
	NewActor->SetName(_Name);

	// Insert + Find
	std::list<GameEngineActor*>& List = ActorList_[_UpdateOrder];
	List.push_back(NewActor);
	return dynamic_cast<ActorType*>(NewActor);
}
