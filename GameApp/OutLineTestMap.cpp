#include "Precompile.h"
#include "OutLineTestMap.h"

#include "NavMesh.h"

void OutLineTestMap::Start()
{
	GameEngineDirectory NaviMeshDir;
	NaviMeshDir.MoveParent("EternalReturn");
	NaviMeshDir / "Resources" / "FBX" / "Map";
	if (nullptr == GameEngineFBXMeshManager::GetInst().Find(NaviMeshDir.PathToPlusFileName("Bg_Map_Cobalt.fbx")))
	{
		GameEngineFBXMesh* Mesh = GameEngineFBXMeshManager::GetInst().Load(NaviMeshDir.PathToPlusFileName("Bg_Map_Cobalt.fbx"));
		Mesh->CreateRenderingBuffer();
	}

	navMeshRenderer_ = CreateTransformComponent<GameEngineFBXRenderer>();
	navMeshRenderer_->SetFBXMesh("Bg_Map_Cobalt.fbx", "TextureDeferredLight");

	for (UINT i = 0; i < navMeshRenderer_->GetRenderSetCount(); i++)
	{
		navMeshRenderer_->GetRenderSet(i).ShaderHelper->SettingTexture("DiffuseTex", "Red.png");
		navMeshRenderer_->GetRenderSet(i).PipeLine_->SetRasterizer("EngineBaseRasterizerWireFrame");
		//navMeshRenderer_->GetRenderSet(i).PipeLine_->SetRasterizer("EngineBaseRasterizerNone");
	}

	navMeshRenderer_->GetTransform()->SetLocalScaling(100.0f);
	navMeshRenderer_->GetTransform()->SetLocalPosition({ 0.0f, -20.0f });

	navMesh_ = CreateComponent<NavMesh>();

	// 모든 메쉬를 네비메쉬로
	navMesh_->CreateNaviMesh(navMeshRenderer_, "Bg_Map_Cobalt");

	makeAStarNode(50.f, 50.f);
	checkASterNodeObstacle();
}

void OutLineTestMap::Update(float _deltaTime)
{
}

void OutLineTestMap::makeAStarNode(float _intervalX, float _intervalZ)
{
	intervalX_ = _intervalX;
	intervalZ_ = _intervalZ;
	gridStartX_ = FLT_MAX;
	gridStartZ_ = FLT_MAX;
	float endX = -FLT_MAX;
	float endZ = -FLT_MAX;

	// 네비게이션 메시를 돌면서 최소값, 최대값 X, Z 를 구한다.
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

	// 길이에 인터벌을 나눈 만큼 노드 갯수를 생성할 겁니다.
	int nodeCountX = static_cast<int>(XLength / intervalX_) + 2;
	int nodeCountZ = static_cast<int>(ZLength / intervalZ_) + 2;

	// 노드를 넣을 만큼의 배열 공간을 확보합니다.
	allNodes_.reserve(nodeCountZ);

	for (std::vector<AStarNode>& v : allNodes_)
	{
		v.reserve(nodeCountX);
	}

	// 새로운 노드를 만들어 삽입합니다.
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
}

OutLineTestMap::OutLineTestMap()
	: Renderer_(nullptr)
{
}

OutLineTestMap::~OutLineTestMap()
{
}
