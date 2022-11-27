#include "PreCompile.h"
#include "HyperLoop.h"

#include <GameEngine/GameEngineCollision.h>
#include <GameEngine/GameEngineFBXRenderer.h>

#include "LumiaLevel.h"
#include "LumiaMap.h"
#include "PlayerInfoManager.h"

HyperLoop::HyperLoop()
	: renderer_(nullptr)
	, collision_(nullptr)

{

}

HyperLoop::~HyperLoop()
{

}

void HyperLoop::Start()
{
	static bool bLoad = false;

	if (false == bLoad)
	{
		GameEngineDirectory dir;

		dir.MoveParent("EternalReturn");
		dir / "Resources" / "FBX" / "Map";

		GameEngineFBXMesh* mesh = GameEngineFBXMeshManager::GetInst().Load(dir.PathToPlusFileName("Hyperloop.fbx"));
		mesh->CreateRenderingBuffer();

		bLoad = true;
	}

	renderer_ = CreateTransformComponent<GameEngineFBXRenderer>();
	renderer_->SetFBXMesh("Hyperloop.fbx", "TextureDeferredLight");
	renderer_->GetTransform()->SetLocalScaling(100.0f);

	collision_ = CreateTransformComponent<GameEngineCollision>();
	collision_->GetTransform()->SetLocalScaling({ 150.0f , 250.0f, 150.0f });
	collision_->GetTransform()->SetLocalMove({ 0.0f , 125.0f, 0.0f });
	collision_->SetCollisionGroup(eCollisionGroup::HyperLoop);
	collision_->SetCollisionType(CollisionType::OBBBox3D);

	//이건호 : 맵UI CreateActor 로 초기화
	mapUI_ = GetLevel()->CreateActor<UI_HyperMap>();
	//초기화후 Off해서 모든 기능을 꺼줌
	mapUI_->MapOff();
}

void HyperLoop::Update(float _DeltaTime)
{
	SelectedArea = Location::NONE;

	if (nullptr != collision_ &&
		collision_->IsUpdate())
	{
		GetLevel()->PushDebugRender(collision_->GetTransform(), CollisionType::OBBBox3D, float4::BLACK);
	}

	if (true == collision_->Collision(eCollisionGroup::MouseRay) &&
		true == collision_->Collision(eCollisionGroup::Player))
	{
		// 하이퍼루프 UI가 나오고, 특정 지역을 선택하면 해당 지역으로 이동

	}

	std::list<GameEngineCollision*> list = collision_->GetCollisionList(eCollisionGroup::Player);

	if (list.size() <= 0)
	{
		mapUI_->MapOff();
		SelectedArea = Location::NONE;
	}


	for (GameEngineCollision* col : list)
	{
		Character* colCharacter = static_cast<Character*>(col->GetActor());
		Character* player = PlayerInfoManager::GetInstance()->GetMainCharacter();
		if (colCharacter == player && !player->IsOperating())
		{
			//이건호 : 맵 UI를 킨다 
			// MapOff(); 함수를 사용하면 Map뜬거를 끌수 있습니다
			mapUI_->MapOn();

			//이건호 : 내가 맵에서 우클릭으로 클릭한 지역을 enum class Location으로 받을 수 있습니다
			SelectedArea = mapUI_->ReturnSelectedLocation();
		}
		else
		{
			mapUI_->MapOff();
			SelectedArea = Location::NONE;
		}
	}



	if (Location::NONE != SelectedArea)
	{
		Character* player = PlayerInfoManager::GetInstance()->GetMainCharacter();

		std::vector<float4> spawnPoints = GetLevelConvert<LumiaLevel>()->GetMap()->GetCharacterSpawnPoints(static_cast<int>(SelectedArea));
		GameEngineRandom random;
		int point = random.RandomInt(0, static_cast<int>(spawnPoints.size()) - 1);

		//player->GetTransform()->SetWorldPosition(spawnPoints[point]);

		player->Hyperloop(spawnPoints[point], SelectedArea);

		mapUI_->MapOff();
		SelectedArea = Location::NONE;
	}
}

