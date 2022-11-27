#include "PreCompile.h"
#include "LumiaMap.h"

#include <GameEngine/GameEngineFBXRenderer.h>
#include <GameEngine/EngineVertex.h>
#include <GameEngine/GameEngineCollision.h>
#include <GameEngine/GameEngineCore.h>
#include <GameEngine/GameEngineEffectRenderer.h>
#include "LumiaLevel.h"

#include "NavMesh.h"
#include "PlayerInfoManager.h"

LumiaMap::LumiaMap()
	: navMeshRenderer_(nullptr)
	, downTownRenderer_(nullptr)
	, tileVertexBuffer_(nullptr)
	, tileIndexBuffer_(nullptr)
	, intervalX_(0.0f)
	, intervalZ_(0.0f)
	, gridStartX_(0.0f)
	, gridStartZ_(0.0f)
	, gridZCount_(0)
	, navMesh_(nullptr)
	, gridXCount_(0)
	, mapScale_(100.0f)
{
		enum_MapName.push_back("HARBOR");			// �ױ�			
		enum_MapName.push_back("POND");				// ����
		enum_MapName.push_back("BEACH");			// �𷡻���
		enum_MapName.push_back("UPTOWN");			// ��� ���ð�
		enum_MapName.push_back("ALLEY");			// ����
		enum_MapName.push_back("HOTEL");			// ȣ��
		enum_MapName.push_back("DOWNTOWN");			// ��ȭ��		
		enum_MapName.push_back("HOSPITAL");			// ����
		enum_MapName.push_back("TEMPLE");			// ��
		enum_MapName.push_back("ARCHERY");			// �����
		enum_MapName.push_back("CEMETERY");			// ����
		enum_MapName.push_back("FOREST");			// ��
		enum_MapName.push_back("FACTORY");			// ����
		enum_MapName.push_back("CHURCH");			// ����			
		enum_MapName.push_back("SCHOOL");			// �б�
		enum_MapName.push_back("RESERCH_CENTRE");	// ������
}

LumiaMap::~LumiaMap()
{
	if (nullptr != tileVertexBuffer_)
	{
		delete tileVertexBuffer_;
	}

	if (nullptr != tileIndexBuffer_)
	{
		delete tileIndexBuffer_;
	}
}

void LumiaMap::Start()
{
	navMeshRenderer_ = CreateTransformComponent<GameEngineFBXRenderer>();
	//navMeshRenderer_->SetFBXMesh("DowntownNavMesh.fbx", "TextureDeferredLight");
	navMeshRenderer_->SetFBXMesh("Bg_NaviMesh.fbx", "TextureDeferredLight");

	for (UINT i = 0; i < navMeshRenderer_->GetRenderSetCount(); i++)
	{
		navMeshRenderer_->GetRenderSet(i).ShaderHelper->SettingTexture("DiffuseTex", "Red.png");
		navMeshRenderer_->GetRenderSet(i).PipeLine_->SetRasterizer("EngineBaseRasterizerWireFrame");
		//navMeshRenderer_->GetRenderSet(i).PipeLine_->SetRasterizer("EngineBaseRasterizerNone");
	}

	navMeshRenderer_->GetTransform()->SetLocalScaling(mapScale_);
	navMeshRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 0.0f });
	navMeshRenderer_->Off();

	navMesh_ = CreateComponent<NavMesh>();

	// ��� �޽��� �׺�޽���
	navMesh_->CreateNaviMesh(navMeshRenderer_);

	makeAStarNode(100.f, 100.f);
	checkASterNodeObstacle();
	//UpdateAStarNodeVertexInfo();

	GameEngineDirectory tempDir;

	tempDir.MoveParent("EternalReturn");
	tempDir / "Resources" / "FBX" / "UserMesh" / "Map";

	std::vector<GameEngineFile> vecFile = tempDir.GetAllFile(".UserMesh");

	mapRenderers.resize(vecFile.size());

	for (size_t i = 0; i < mapRenderers.size(); i++)
	{
		mapRenderers[i] = CreateTransformComponent<GameEngineFBXRenderer>(GetTransform());
		mapRenderers[i]->SetFBXMesh(vecFile[i].GetFileName(), "DeferredLightMap");
		mapRenderers[i]->GetTransform()->SetLocalScaling(mapScale_);
	}

	tempDir.MoveParent("UserMesh");
	tempDir.MoveChild("ItemBox");
	vecFile = tempDir.GetAllFile(".UserMesh");
	for (size_t i = 0; i < vecFile.size(); i++)
	{
		GameEngineFBXRenderer* FBX = CreateTransformComponent<GameEngineFBXRenderer>(GetTransform());
		FBX->SetFBXMesh(vecFile[i].GetFileName(), "DeferredLightMap");
		FBX->GetTransform()->SetLocalScaling(mapScale_);

		mapRenderers.push_back(FBX);
	}

	setAllItem();

	GameEngineDirectory spawnPointDir;
	spawnPointDir.MoveParent("EternalReturn");
	spawnPointDir / "Resources" / "FBX" / "Map";

	setCharacterSpawnPoints(spawnPointDir);
	setHyperLoopSpawnPoints(spawnPointDir);

	/*GameEngineDirectory MeshDir;
	MeshDir.MoveParent("EternalReturn");
	MeshDir / "Resources" / "Texture" / "Map";

	std::string fileName = "Forest.fbx";
	if (nullptr == GameEngineFBXMeshManager::GetInst().Find(MeshDir.PathToPlusFileName(fileName)))
	{
		GameEngineFBXMesh* Mesh = GameEngineFBXMeshManager::GetInst().Load(MeshDir.PathToPlusFileName(fileName));
		Mesh->CreateRenderingBuffer();
	}
	{
		GameEngineFBXRenderer* FBX = CreateTransformComponent<GameEngineFBXRenderer>(GetTransform());
		FBX->SetFBXMesh(fileName, "TextureDeferredLight");
		FBX->GetTransform()->SetLocalScaling(mapScale_);
	}*/
}

void LumiaMap::Update(float _deltaTime)
{
	if (level_->GetMainCamera()->IsDebugCheck() == true)
	{
		navTileRenderer_->Off();
	}
	else
	{
		navTileRenderer_->On();
	}

	if (nullptr != PlayerInfoManager::GetInstance()->GetMainCharacter())
	{
		float4 PlayerPos = PlayerInfoManager::GetInstance()->
			GetMainCharacter()->GetTransform()->GetWorldPosition();

		std::list<float> tmp;

		for (size_t i = 0; i < mapRenderers.size(); i++)
		{
			std::vector<RenderSet>& RenderSets = mapRenderers[i]->GetAllRenderSet();

			for (size_t j = 0; j < RenderSets.size(); j++)
			{
				//float Length = float4::Calc_Len3D(PlayerPos, RenderSets[j].LocalPos * mapRenderers[i]->GetTransform()->GetTransformData().WorldWorld_);
				float4 RenderSetPos = RenderSets[j].LocalPos * mapRenderers[i]->GetTransform()->GetTransformData().WorldWorld_;

				float Length = float4::Calc_Len2D(PlayerPos.x, PlayerPos.z,
					RenderSetPos.x, RenderSetPos.z);

				tmp.push_back(RenderSets[j].Radius);

				if (RenderSets[j].Radius > 1.0f)
				{
					if (Length >= RENDER_RANGE * RenderSets[j].Radius)
					{
						RenderSets[j].isRender = false;
					}
					else
					{
						RenderSets[j].isRender = true;
					}
				}
				else
				{
					if (Length >= RENDER_RANGE)
					{
						RenderSets[j].isRender = false;
					}
					else
					{
						RenderSets[j].isRender = true;
					}
				}
			}
		}
		tmp;
		int a = 0;
	}
}

std::vector<float4> LumiaMap::FindPath(const float4& _startPosition, const float4& _endPosition)
{
	std::vector<float4> result;

	// �׸����� ��ŸƮ ����Ʈ �ε����� ���� ����Ʈ �ε����� ���Ѵ�.
	int startIndexX = static_cast<int>((_startPosition.x - gridStartX_) / intervalX_ + 0.5f); // �ݿø�ó��
	int startIndexZ = static_cast<int>((_startPosition.z - gridStartZ_) / intervalZ_ + 0.5f);

	int endIndexX = static_cast<int>((_endPosition.x - gridStartX_) / intervalX_ + 0.5f);
	int endIndexZ = static_cast<int>((_endPosition.z - gridStartZ_) / intervalZ_ + 0.5f);

	// �������� �׸��带 ����� ���, Ȥ�� �������� �̵� �Ұ����� ���� ��� ���� ó��
	if (endIndexX < 0 || endIndexZ < 0 || endIndexX >= gridXCount_ || endIndexZ >= gridZCount_)
	{
		result.push_back(_endPosition);
		return result;
	}
	else if (allNodes_[endIndexZ][endIndexX].IsObstacle())
	{
		result.push_back(_endPosition);
		return result;
	}

	// ���ٷ� �� �Լ��� �ۼ�. F���� ���� ���ϰ�, F���� ������ H���� ���Ѵ�.
	// �ڽ�Ʈ�� ���� ���� �༮�� ������ �ڷ� ���� �����̴�. pop_back �� �ϱ� ����.
	auto cmp = [&](AStarNode* _left, AStarNode* _right)
	{
		float leftFCost = _left->GetFCost();
		float rightFCost = _right->GetFCost();

		if (leftFCost > rightFCost)
		{
			return true;
		}
		if (leftFCost == rightFCost)
		{
			return _left->GetHCost() > _right->GetHCost();
		}

		return false;
	};

	std::vector<AStarNode*> open;
	std::vector<AStarNode*> close;
	AStarNode* endNode = &allNodes_[endIndexZ][endIndexX];

	open.push_back(&allNodes_[startIndexZ][startIndexX]);

	while (true)
	{
		std::sort(open.begin(), open.end(), cmp);
		AStarNode* current = open.back();
		open.pop_back();
		close.push_back(current);

		// ���� ��尡 ������ �����
		if (current == endNode)
		{
			return current->GetPath();
		}

		// ���ϰ��� false �� ��� ��ֹ� �������� ����.
		// ���� �Լ��� �̿� üũ
		auto checkNeighbour = [&](int _xIndex, int _zIndex)->bool
		{
			if (_zIndex < 0 || _zIndex >= gridZCount_)
			{
				return true;
			}

			if (_xIndex < 0 || _xIndex >= gridXCount_)
			{
				return true;
			}

			AStarNode* neighbour = &allNodes_[_zIndex][_xIndex];

			// �̿� ��尡 ��ֹ� �̶��
			if (neighbour->IsObstacle())
			{
				return false;
			}

			// �̿���尡 CLOSED ����Ʈ�� ���ִٸ�
			if (close.end() != std::find(close.begin(), close.end(), neighbour))
			{
				return true;
			}

			// // open list �� ��尡 ������
			if (open.end() == std::find(open.begin(), open.end(), neighbour))
			{
				neighbour->EvaluateCost(*current, *endNode);
				neighbour->SetParent(current);
				open.push_back(neighbour);
			}
			else if (current->GetEvaluatedGCostToOhterNode(*neighbour) < neighbour->GetGCost()) // ���� ��忡�� �̿����� ���� ��ΰ� �̿������ ���� ��κ��� ª�ٸ�(�θ� ������� ��, gCost�� �۴ٸ�)
			{
				neighbour->EvaluateCost(*current, *endNode);
				neighbour->SetParent(current);
			}

			return true;
		};

		int currentXIndex = current->GetXIndex();
		int currentZIndex = current->GetZIndex();

		// �����¿���� üũ�Ѵ�.
		bool bPassableUp = checkNeighbour(currentXIndex, currentZIndex + 1);
		bool bPassableDown = checkNeighbour(currentXIndex, currentZIndex - 1);
		bool bPassableLeft = checkNeighbour(currentXIndex - 1, currentZIndex);
		bool bPassableRight = checkNeighbour(currentXIndex + 1, currentZIndex);

		// �»�, ���, ����, ���� ������ üũ�Ѵ�.
		// �����¿찡 ���������� ����Ұ��� ����.
		if (bPassableUp)
		{
			if (bPassableLeft)
			{
				checkNeighbour(currentXIndex - 1, currentZIndex + 1);
			}

			if (bPassableRight)
			{
				checkNeighbour(currentXIndex + 1, currentZIndex + 1);
			}
		}

		if (bPassableDown)
		{
			if (bPassableLeft)
			{
				checkNeighbour(currentXIndex - 1, currentZIndex - 1);
			}

			if (bPassableRight)
			{
				checkNeighbour(currentXIndex + 1, currentZIndex - 1);
			}
		}


		// ��ΰ� ������ �극��ũ (��, ������ �� ����.)
		if (open.empty())
		{
			break;
		}
	}

	return result;
}

std::vector<float4> LumiaMap::GetCharacterSpawnPoints(int _spawnArea)
{
	std::string areaName = enum_MapName[_spawnArea];

	auto iter = characterSpawnPoints_.find(areaName);

	if (characterSpawnPoints_.end() == iter)
	{
		GameEngineDebug::MsgBoxError("if (characterSpawnPoints_.end() == iter)");

		return std::vector<float4>();
	}

	return (*iter).second;
}

std::vector<float4> LumiaMap::GetEyeSightPolygon(const float4& _position, const float _length)
{
	std::vector<float4> result;
	if (nullptr == navMeshRenderer_)
	{
		return std::vector<float4>();
	}

	float4 direction = { 0.0f, 0.0f, 1.0f };
	float degree = 0.f;
	float interval = _length / 20.f;
	float degreeInterval = 10.f;
	int count = 360 / static_cast<int>(degreeInterval);

	result.resize(count);

	for (size_t i = 0; i < count; i++)
	{
		float length = 0.0f;
		float4 checkPosition;
		degree += degreeInterval;
		direction = { 0.0f, 0.0f, 1.0f };
		direction.RotateYDegree(degree);
		while (length <= _length)
		{

			length += interval;
			float4 temp = direction * length;
			checkPosition = _position + temp;
			float height;
			NavFace* face = navMesh_->GetNavFaceFromPositionXZ(checkPosition, float4::DOWN, height);
			//checkPosition.y = 100.0f;
			if (nullptr == face)
			{
				break;
			}
		}

		result[i] = checkPosition;
	}

	return result;
}

void LumiaMap::ToggleNavMeshRender()
{
	if (navMeshRenderer_->IsUpdate())
	{
		navMeshRenderer_->Off();
	}
	else
	{
		navMeshRenderer_->On();
	}
}

void LumiaMap::makeAStarNode(float _intervalX, float _intervalZ)
{
	intervalX_ = _intervalX;
	intervalZ_ = _intervalZ;
	gridStartX_ = FLT_MAX;
	gridStartZ_ = FLT_MAX;
	float endX = -FLT_MAX;
	float endZ = -FLT_MAX;

	// �׺���̼� �޽ø� ���鼭 �ּҰ�, �ִ밪 X, Z �� ���Ѵ�.
	for (NavFace n : navMesh_->GetAllNavi())
	{
		for (float4& f : n.GetInfo().Vertex)
		{
			if (f.x < gridStartX_)
			{
				gridStartX_ = f.x;
			}

			if (f.z < gridStartZ_)
			{
				gridStartZ_ = f.z;
			}

			if (f.x > endX)
			{
				endX = f.x;
			}

			if (f.z > endZ)
			{
				endZ = f.z;
			}
		}
	}

	gridStartX_ *= navMeshRenderer_->GetTransform()->GetLocalScaling().x;
	gridStartZ_ *= navMeshRenderer_->GetTransform()->GetLocalScaling().z;
	endX *= navMeshRenderer_->GetTransform()->GetLocalScaling().x;
	endZ *= navMeshRenderer_->GetTransform()->GetLocalScaling().z;

	float XLength = endX - gridStartX_;
	float ZLength = endZ - gridStartZ_;

	// ���̿� ���͹��� ���� ��ŭ ��� ������ ������ �̴ϴ�.
	int nodeCountX = static_cast<int>(XLength / intervalX_) + 2;
	int nodeCountZ = static_cast<int>(ZLength / intervalZ_) + 2;

	// ��带 ���� ��ŭ�� �迭 ������ Ȯ���մϴ�.
	allNodes_.reserve(nodeCountZ);

	for (std::vector<AStarNode>& v : allNodes_)
	{
		v.reserve(nodeCountX);
	}

	// ���ο� ��带 ����� �����մϴ�.
	int index = 0;
	for (int z = 0; z < nodeCountZ; z++)
	{
		allNodes_.push_back(std::vector<AStarNode>());
		for (int x = 0; x < nodeCountX; x++)
		{
			float posX = gridStartX_ + x * intervalX_;
			float posZ = gridStartZ_ + z * intervalZ_;

			AStarNode newNode(index++, z, x, float4(posX, 0.0f, posZ));

			allNodes_[z].push_back(newNode);
		}
	}

	tileVertexBuffer_ = new GameEngineVertexBuffer();
	tileIndexBuffer_ = new GameEngineIndexBuffer();

	tileVertices_;
	tileIndices_;
	size_t allVertexCount = allNodes_.size() * allNodes_[0].size();
	tileVertices_.reserve(allVertexCount);
	tileIndices_.reserve(allVertexCount * 7);

	int counter = 0;
	gridZCount_ = static_cast<int>(allNodes_.size());
	gridXCount_ = static_cast<int>(allNodes_[0].size());

	for (auto n : allNodes_)
	{
		for (auto m : n)
		{
			GameEngineVertex v;
			v.POSITION = m.GetPosition();
			v.COLOR = float4::GREEN;
			tileVertices_.push_back(v);
		}
	}

	for (size_t z = 0; z < gridZCount_ - 1; z++)
	{
		for (size_t x = 0; x < gridXCount_ - 1; x++)
		{
			size_t currentVertexIndex = z * gridXCount_ + x;
			tileIndices_.push_back(static_cast<UINT>(currentVertexIndex));
			tileIndices_.push_back(static_cast<UINT>(currentVertexIndex + 1));
			tileIndices_.push_back(static_cast<UINT>(currentVertexIndex + gridXCount_));

			tileIndices_.push_back(static_cast<UINT>(currentVertexIndex + 1));
			tileIndices_.push_back(static_cast<UINT>(currentVertexIndex + gridXCount_ + 1));
			tileIndices_.push_back(static_cast<UINT>(currentVertexIndex + gridXCount_));
		}
	}


	tileVertexBuffer_->Create(tileVertices_, D3D11_USAGE::D3D11_USAGE_DYNAMIC);
	tileIndexBuffer_->Create(tileIndices_, D3D11_USAGE::D3D11_USAGE_DEFAULT);

	navTileRenderer_ = CreateTransformComponent<GameEngineRenderer>(GetTransform());
	navTileRenderer_->SetRenderingPipeLine("DeferredNavTile");
	navTileRenderer_->SetMesh(tileVertexBuffer_, tileIndexBuffer_);
	navTileRenderer_->GetGameEngineRenderingPipeLine()->SetRasterizer("EngineBaseRasterizerWireFrame");
	//navTileRenderer_->GetTransform()->SetLocalScaling({ 100.f, 100.f, 100.f });
}

void LumiaMap::checkASterNodeObstacle()
{
	GameEngineTime timer;
	timer.TimeCheckReset();
	timer.TimeCheck();

	std::vector<std::thread> threads;
	for (std::vector<AStarNode>& v : allNodes_)
	{
		auto func = [&]()
		{
			for (AStarNode& n : v)
			{
				float height = 0.0f;
				NavFace* face = navMesh_->GetNavFaceFromPositionXZ(n.GetPosition(), float4::DOWN, height);
				if (nullptr == face)
				{
					n.SetObstacle(true);
					tileVertices_[n.GetIndex()].COLOR = float4::RED;
				}
				else
				{
					tileVertices_[n.GetIndex()].POSITION.y = height;
				}
			}
		};

		threads.emplace_back(func);
	}

	int i = 0;
	while (i < threads.size())
	{
		// �ش� ������ ������̶��
		if (true == threads[i].joinable())
		{
			// �ش� ������ ������� ���
			threads[i].join();
			i = 0;
		}
		else
		{
			i++;
		}
	}
	threads.clear();

	timer.TimeCheck();
	float elapsedTime = timer.GetDeltaTime();
	GameEngineDebug::OutPutDebugString("checkAStarNodeObstacle time elapsed : " + std::to_string(elapsedTime) + "\n");
}

void LumiaMap::UpdateAStarNodeVertexInfo()
{
	ID3D11DeviceContext* dc = GameEngineDevice::GetContext();

	D3D11_MAPPED_SUBRESOURCE subResource;
	ZeroMemory(&subResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	dc->Map(tileVertexBuffer_->Buffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	memcpy(subResource.pData, &tileVertices_[0], subResource.RowPitch);
	dc->Unmap(tileVertexBuffer_->Buffer_, 0);
}

void LumiaMap::setAllItem()
{
	ItemBoxManager* itembox = GetLevelConvert<LumiaLevel>()->GetItemBoxManager();

	if (nullptr == itembox)
	{
		GameEngineDebug::MsgBoxError("��̾Ʒ������� �����۹ڽ��Ŵ����� �������� �ʾҽ��ϴ�.");
		return;
	}

	itembox->PushRandomItem("Alley", "Bandage", 14);
	itembox->PushRandomItem("Alley", "Binoculars", 7);
	itembox->PushRandomItem("Alley", "Bracelet", 11);
	itembox->PushRandomItem("Alley", "Cross", 8);
	itembox->PushRandomItem("Alley", "Garlic", 6);
	itembox->PushRandomItem("Alley", "Glue", 9);
	itembox->PushRandomItem("Alley", "Hammer", 13);
	itembox->PushRandomItem("Alley", "Honey", 8);
	itembox->PushRandomItem("Alley", "Lighter", 13);
	itembox->PushRandomItem("Alley", "Needle", 9);
	itembox->PushRandomItem("Alley", "Rubber", 9);
	itembox->PushRandomItem("Alley", "RunningShoes", 11);
	itembox->PushRandomItem("Alley", "Scissors", 13);
	itembox->PushRandomItem("Alley", "StallionMedal", 9);
	itembox->PushRandomItem("Alley", "SteelChain", 14);
	itembox->PushRandomItem("Alley", "Wetsuit", 11);

	itembox->PushRandomItem("Temple", "Bamboo", 9);
	itembox->PushRandomItem("Temple", "KitchenKnife", 6);
	itembox->PushRandomItem("Temple", "Ramen", 5);
	itembox->PushRandomItem("Temple", "ShortSpear", 6);
	itembox->PushRandomItem("Temple", "Hairband", 11);
	itembox->PushRandomItem("Temple", "MonksRobe", 13);
	itembox->PushRandomItem("Temple", "FabricArmor", 12);
	itembox->PushRandomItem("Temple", "Feather", 9);
	itembox->PushRandomItem("Temple", "BuddhistScripture", 14);
	itembox->PushRandomItem("Temple", "StallionMedal", 9);
	itembox->PushRandomItem("Temple", "Cloth", 13);
	itembox->PushRandomItem("Temple", "Gemstone", 14);
	itembox->PushRandomItem("Temple", "Paper", 13);
	itembox->PushRandomItem("Temple", "Gunpowder", 9);
	itembox->PushRandomItem("Temple", "Garlic", 8);
	itembox->PushRandomItem("Temple", "OrientalHerb", 6);

	itembox->PushRandomItem("Downtown", "GlassBottle", 12);
	itembox->PushRandomItem("Downtown", "PlayingCards", 14);
	itembox->PushRandomItem("Downtown", "FountainPen", 12);
	itembox->PushRandomItem("Downtown", "Hairband", 13);
	itembox->PushRandomItem("Downtown", "Watch", 9);
	itembox->PushRandomItem("Downtown", "Slippers", 12);
	itembox->PushRandomItem("Downtown", "Tights", 11);
	itembox->PushRandomItem("Downtown", "Fan", 13);
	itembox->PushRandomItem("Downtown", "Nail", 13);
	itembox->PushRandomItem("Downtown", "Battery", 14);
	itembox->PushRandomItem("Downtown", "Oil", 9);
	itembox->PushRandomItem("Downtown", "Cloth", 14);
	itembox->PushRandomItem("Downtown", "Can", 8);
	itembox->PushRandomItem("Downtown", "Chocolate", 4);
	itembox->PushRandomItem("Downtown", "Honey", 8);
	itembox->PushRandomItem("Downtown", "Milk", 8);
	//itembox->PushRandomItem("Downtown", "SurveillanceCamera", 7);

	itembox->PushRandomItem("Pond", "ShortRod", 9);
	itembox->PushRandomItem("Pond", "Bamboo", 9);
	itembox->PushRandomItem("Pond", "Hammer", 12);
	itembox->PushRandomItem("Pond", "Pickaxe", 14);
	itembox->PushRandomItem("Pond", "Hatchet", 6);
	itembox->PushRandomItem("Pond", "ShortSpear", 5);
	itembox->PushRandomItem("Pond", "Hat", 11);
	itembox->PushRandomItem("Pond", "BikeHelmet", 11);
	itembox->PushRandomItem("Pond", "Bracelet", 12);
	itembox->PushRandomItem("Pond", "Flower", 6);
	itembox->PushRandomItem("Pond", "Ribbon", 11);
	itembox->PushRandomItem("Pond", "Box", 9);
	//itembox->PushRandomItem("Pond", "Snare", 4);
	//itembox->PushRandomItem("Pond", "Mousetrap", 7);
	itembox->PushRandomItem("Pond", "TurtleShell", 13);
	itembox->PushRandomItem("Pond", "Gemstone", 14);
	itembox->PushRandomItem("Pond", "OrientalHerb", 7);

	itembox->PushRandomItem("Hospital", "CottonGloves", 8);
	itembox->PushRandomItem("Hospital", "Razor", 8);
	itembox->PushRandomItem("Hospital", "Scissors", 12);
	itembox->PushRandomItem("Hospital", "Needle", 9);
	itembox->PushRandomItem("Hospital", "Bandage", 13);
	itembox->PushRandomItem("Hospital", "Tights", 11);
	//itembox->PushRandomItem("Hospital", "SurveillanceCamera", 5);
	itembox->PushRandomItem("Hospital", "ScrapMetal", 13);
	itembox->PushRandomItem("Hospital", "LaserPointer", 9);
	itembox->PushRandomItem("Hospital", "Alcohol", 14);
	itembox->PushRandomItem("Hospital", "Glue", 12);
	itembox->PushRandomItem("Hospital", "Lemon", 5);
	itembox->PushRandomItem("Hospital", "Bread", 7);
	itembox->PushRandomItem("Hospital", "Ice", 9);
	itembox->PushRandomItem("Hospital", "Milk", 5);

	itembox->PushRandomItem("Archery", "Bamboo", 8);
	itembox->PushRandomItem("Archery", "IronBall", 12);
	itembox->PushRandomItem("Archery", "Bow", 6);
	itembox->PushRandomItem("Archery", "Hat", 11);
	itembox->PushRandomItem("Archery", "MonksRobe", 12);
	itembox->PushRandomItem("Archery", "FabricArmor", 11);
	itembox->PushRandomItem("Archery", "RunningShoes", 12);
	//itembox->PushRandomItem("Archery", "SurveillanceCamera", 4);
	//itembox->PushRandomItem("Archery", "Snare", 4);
	itembox->PushRandomItem("Archery", "Nail", 13);
	itembox->PushRandomItem("Archery", "Rubber", 9);
	itembox->PushRandomItem("Archery", "Oil", 11);
	itembox->PushRandomItem("Archery", "Paper", 13);
	itembox->PushRandomItem("Archery", "Gunpowder", 8);
	itembox->PushRandomItem("Archery", "RustySword", 8);
	itembox->PushRandomItem("Archery", "Chocolate", 5);


	itembox->PushRandomItem("School", "Whip", 4);
	itembox->PushRandomItem("School", "Razor", 8);
	itembox->PushRandomItem("School", "Chalk", 8);
	itembox->PushRandomItem("School", "Scissors", 12);
	itembox->PushRandomItem("School", "FountainPen", 11);
	itembox->PushRandomItem("School", "Hat", 12);
	itembox->PushRandomItem("School", "BikeHelmet", 11);
	itembox->PushRandomItem("School", "Windbreaker", 12);
	itembox->PushRandomItem("School", "Bandage", 13);
	itembox->PushRandomItem("School", "Slippers", 12);
	itembox->PushRandomItem("School", "Ribbon", 9);
	itembox->PushRandomItem("School", "Lighter", 13);
	itembox->PushRandomItem("School", "LaserPointer", 11);
	itembox->PushRandomItem("School", "Alcohol", 9);
	itembox->PushRandomItem("School", "Can", 8);
	itembox->PushRandomItem("School", "Bread", 6);
	itembox->PushRandomItem("School", "Milk", 4);

	// CEMETERY
	itembox->PushRandomItem("Cemetary", "BrassKnuckles", 5);
	itembox->PushRandomItem("Cemetary", "Bamboo", 13);
	itembox->PushRandomItem("Cemetary", "Pickaxe", 14);
	itembox->PushRandomItem("Cemetary", "SteelChain", 14);
	itembox->PushRandomItem("Cemetary", "Hairband", 11);
	itembox->PushRandomItem("Cemetary", "FabricArmor", 11);
	itembox->PushRandomItem("Cemetary", "Feather", 14);
	itembox->PushRandomItem("Cemetary", "Flower", 5);
	//itembox->PushRandomItem("Cemetary", "SurveillanceCamera", 6);
	//itembox->PushRandomItem("Cemetary", "Mousetrap", 9);
	itembox->PushRandomItem("Cemetary", "IronOre", 9);
	itembox->PushRandomItem("Cemetary", "Gunpowder", 8);
	itembox->PushRandomItem("Cemetary", "Garlic", 9);
	itembox->PushRandomItem("Cemetary", "Egg", 4);
	itembox->PushRandomItem("Cemetary", "Ice", 9);
	itembox->PushRandomItem("Cemetary", "Coffee", 7);

	itembox->PushRandomItem("Factory", "IronBall", 14);
	itembox->PushRandomItem("Factory", "Chalk", 7);
	itembox->PushRandomItem("Factory", "Whip", 4);
	itembox->PushRandomItem("Factory", "WaltherPPK", 8);
	itembox->PushRandomItem("Factory", "Hatchet", 6);
	itembox->PushRandomItem("Factory", "Binoculars", 9);
	itembox->PushRandomItem("Factory", "Nail", 14);
	itembox->PushRandomItem("Factory", "ScrapMetal", 14);
	itembox->PushRandomItem("Factory", "Lighter", 14);
	itembox->PushRandomItem("Factory", "Battery", 14);
	itembox->PushRandomItem("Factory", "Alcohol", 11);
	itembox->PushRandomItem("Factory", "Oil", 9);
	itembox->PushRandomItem("Factory", "Glue", 13);
	itembox->PushRandomItem("Factory", "Ramen", 7);
	itembox->PushRandomItem("Factory", "Baseball", 4);
	itembox->PushRandomItem("Factory", "Fedorova", 5);

	itembox->PushRandomItem("Hotel", "CottonGloves", 9);
	itembox->PushRandomItem("Hotel", "PlayingCards", 12);
	itembox->PushRandomItem("Hotel", "WaltherPPK", 7);
	itembox->PushRandomItem("Hotel", "Fedorova", 8);
	itembox->PushRandomItem("Hotel", "KitchenKnife", 6);
	itembox->PushRandomItem("Hotel", "Needle", 9);
	itembox->PushRandomItem("Hotel", "Windbreaker", 11);
	itembox->PushRandomItem("Hotel", "Watch", 11);
	itembox->PushRandomItem("Hotel", "Binoculars", 6);
	itembox->PushRandomItem("Hotel", "PianoWire", 14);
	itembox->PushRandomItem("Hotel", "ScrapMetal", 13);
	itembox->PushRandomItem("Hotel", "Cloth", 14);
	itembox->PushRandomItem("Hotel", "IronOre", 9);
	itembox->PushRandomItem("Hotel", "Lemon", 5);
	itembox->PushRandomItem("Hotel", "Ice", 9);
	itembox->PushRandomItem("Hotel", "Whiskey", 5);
	itembox->PushRandomItem("Hotel", "CarbonatedWater", 9);
	itembox->PushRandomItem("Hotel", "Battery", 11);


	itembox->PushRandomItem("Forest", "BrassKnuckles", 6);
	itembox->PushRandomItem("Forest", "Bamboo", 11);
	itembox->PushRandomItem("Forest", "IronBall", 14);
	itembox->PushRandomItem("Forest", "ShortCrossbow", 8);
	itembox->PushRandomItem("Forest", "LongRifle", 4);
	itembox->PushRandomItem("Forest", "Pickaxe", 14);
	itembox->PushRandomItem("Forest", "ShortSpear", 5);
	itembox->PushRandomItem("Forest", "Tights", 11);
	itembox->PushRandomItem("Forest", "Feather", 14);
	itembox->PushRandomItem("Forest", "Flower", 5);
	itembox->PushRandomItem("Forest", "Fan", 13);
	//itembox->PushRandomItem("Forest", "Snare", 4);
	itembox->PushRandomItem("Forest", "Gemstone", 14);
	itembox->PushRandomItem("Forest", "IronOre", 9);
	itembox->PushRandomItem("Forest", "Egg", 8);
	itembox->PushRandomItem("Forest", "OrientalHerb", 7);
	itembox->PushRandomItem("Forest", "Honey", 9);
	itembox->PushRandomItem("Forest", "FabricArmor", 11);

	itembox->PushRandomItem("CHURCH", "GlassBottle", 9);
	itembox->PushRandomItem("CHURCH", "Razor", 7);
	itembox->PushRandomItem("CHURCH", "Chalk", 5);
	itembox->PushRandomItem("CHURCH", "Bow", 5);
	itembox->PushRandomItem("CHURCH", "RustySword", 7);
	itembox->PushRandomItem("CHURCH", "BikeHelmet", 12);
	itembox->PushRandomItem("CHURCH", "Fan", 11);
	itembox->PushRandomItem("CHURCH", "Box", 12);
	itembox->PushRandomItem("CHURCH", "HolyGrail", 14);
	itembox->PushRandomItem("CHURCH", "Cross", 14);
	itembox->PushRandomItem("CHURCH", "PianoWire", 13);
	itembox->PushRandomItem("CHURCH", "Paper", 13);
	itembox->PushRandomItem("CHURCH", "ShortCrossbow", 7);
	itembox->PushRandomItem("CHURCH", "Milk", 6);
	itembox->PushRandomItem("CHURCH", "Whiskey", 7);

	itembox->PushRandomItem("SANDY_BEACH", "WaltherPPK", 4);
	itembox->PushRandomItem("SANDY_BEACH", "Hammer", 12);
	itembox->PushRandomItem("SANDY_BEACH", "Hatchet", 5);
	itembox->PushRandomItem("SANDY_BEACH", "SteelChain", 13);
	itembox->PushRandomItem("SANDY_BEACH", "BikeHelmet", 11);
	itembox->PushRandomItem("SANDY_BEACH", "Wetsuit", 11);
	itembox->PushRandomItem("SANDY_BEACH", "Binoculars", 6);
	//itembox->PushRandomItem("SANDY_BEACH", "Snare", 4);
	//itembox->PushRandomItem("SANDY_BEACH", "Mousetrap", 9);
	itembox->PushRandomItem("SANDY_BEACH", "PianoWire", 14);
	itembox->PushRandomItem("SANDY_BEACH", "TurtleShell", 13);
	itembox->PushRandomItem("SANDY_BEACH", "StallionMedal", 9);
	itembox->PushRandomItem("SANDY_BEACH", "Can", 9);
	itembox->PushRandomItem("SANDY_BEACH", "CarbonatedWater", 9);
	itembox->PushRandomItem("SANDY_BEACH", "Gemstone", 14);
	itembox->PushRandomItem("SANDY_BEACH", "Pickaxe", 13);
	itembox->PushRandomItem("SANDY_BEACH", "GlassBottle", 11);

	itembox->PushRandomItem("Uptown", "FountainPen", 12);
	itembox->PushRandomItem("Uptown", "Windbreaker", 11);
	itembox->PushRandomItem("Uptown", "Watch", 9);
	itembox->PushRandomItem("Uptown", "Bracelet", 11);
	itembox->PushRandomItem("Uptown", "RunningShoes", 12);
	itembox->PushRandomItem("Uptown", "Flower", 6);
	itembox->PushRandomItem("Uptown", "Ribbon", 11);
	//itembox->PushRandomItem("Uptown", "SurveillanceCamera", 4);
	itembox->PushRandomItem("Uptown", "PianoWire", 14);
	itembox->PushRandomItem("Uptown", "LaserPointer", 9);
	itembox->PushRandomItem("Uptown", "Oil", 13);
	itembox->PushRandomItem("Uptown", "Lemon", 4);
	itembox->PushRandomItem("Uptown", "Chocolate", 6);
	itembox->PushRandomItem("Uptown", "Fedorova", 7);
	itembox->PushRandomItem("Uptown", "Whiskey", 9);
	itembox->PushRandomItem("Uptown", "Coffee", 4);
	itembox->PushRandomItem("Uptown", "CarbonatedWater", 9);
	itembox->PushRandomItem("Uptown", "Baseball", 4);
	itembox->PushRandomItem("Uptown", "Hairband", 11);


	itembox->PushRandomItem("HARBOR", "ShortRod", 9);
	itembox->PushRandomItem("HARBOR", "GlassBottle", 11);
	itembox->PushRandomItem("HARBOR", "LongRifle", 4);
	itembox->PushRandomItem("HARBOR", "KitchenKnife", 6);
	itembox->PushRandomItem("HARBOR", "RustySword", 7);
	itembox->PushRandomItem("HARBOR", "Wetsuit", 11);
	itembox->PushRandomItem("HARBOR", "Bandage", 14);
	itembox->PushRandomItem("HARBOR", "Box", 9);
	//itembox->PushRandomItem("HARBOR", "SurveillanceCamera", 4);
	//itembox->PushRandomItem("HARBOR", "Snare", 4);
	itembox->PushRandomItem("HARBOR", "TurtleShell", 14);
	itembox->PushRandomItem("HARBOR", "Rubber", 9);
	itembox->PushRandomItem("HARBOR", "ScrapMetal", 14);
	itembox->PushRandomItem("HARBOR", "Lighter", 14);
	itembox->PushRandomItem("HARBOR", "Battery", 14);
	itembox->PushRandomItem("HARBOR", "Slippers", 11);
	itembox->PushRandomItem("HARBOR", "Ramen", 6);
	itembox->PushRandomItem("HARBOR", "Coffee", 7);

}

void LumiaMap::setCharacterSpawnPoints(GameEngineDirectory _dir)
{
	if (nullptr != GameEngineFBXMeshManager::GetInst().Find(_dir.PathToPlusFileName("CharacterSpawnPoints.fbx")))
	{
		return;
	}

	GameEngineFBXMesh* Mesh = GameEngineFBXMeshManager::GetInst().Load(_dir.PathToPlusFileName("CharacterSpawnPoints.fbx"));
	std::vector<FbxNodeData> nodeDatas = Mesh->GetAllNodeData();

	for (const auto& areaName : enum_MapName)
	{
		std::vector<float4> vecTrans;

		for (auto& data : nodeDatas)
		{
			std::string UpperName = GameEngineString::toupper(data.name);

			if (std::string::npos != UpperName.find("(") ||
				std::string::npos != UpperName.find(")"))
			{
				continue;
			}

			if (std::string::npos != UpperName.find(areaName) &&
				float4::ZERO != data.translation)
			{
				data.translation *= { 1.0f, 1.0f, -1.0f };
				vecTrans.push_back(data.translation * mapScale_);
			}
		}

		characterSpawnPoints_.insert(std::pair(areaName, vecTrans));
	}
	
}

void LumiaMap::setHyperLoopSpawnPoints(GameEngineDirectory _dir)
{
	if (nullptr == GameEngineFBXMeshManager::GetInst().Find(_dir.PathToPlusFileName("HyperloopSpawnPoints.fbx")))
	{
		GameEngineFBXMesh* Mesh = GameEngineFBXMeshManager::GetInst().Load(_dir.PathToPlusFileName("HyperloopSpawnPoints.fbx"));
		std::vector<FbxNodeData> nodeDatas = Mesh->GetAllNodeData();

		for (auto& data : nodeDatas)
		{
			std::string UpperName = GameEngineString::toupper(data.name);

			if (float4::ZERO != data.translation)
			{
				data.translation *= { 1.0f, 1.0f, -1.0f };
				hyperLoopSpawnPoints_.push_back(data.translation * mapScale_);
			}
		}
	}
}