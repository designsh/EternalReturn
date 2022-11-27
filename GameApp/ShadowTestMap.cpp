#include "PreCompile.h"
#include "ShadowTestMap.h"

#include "NavMesh.h"

void ShadowTestMap::makeAStarNode(float _intervalX, float _intervalZ)
{
	//intervalX_ = _intervalX;
	//intervalZ_ = _intervalZ;
	//gridStartX_ = FLT_MAX;
	//gridStartZ_ = FLT_MAX;
	//float endX = -FLT_MAX;
	//float endZ = -FLT_MAX;

	//// �׺���̼� �޽ø� ���鼭 �ּҰ�, �ִ밪 X, Z �� ���Ѵ�.
	//for (NavFace n : navMesh_->GetAllNavi())
	//{
	//	for (float4& f : n.GetInfo().Vertex)
	//	{
	//		if (f.x < gridStartX_)
	//		{
	//			gridStartX_ = f.x;
	//		}

	//		if (f.z < gridStartZ_)
	//		{
	//			gridStartZ_ = f.z;
	//		}

	//		if (f.x > endX)
	//		{
	//			endX = f.x;
	//		}

	//		if (f.z > endZ)
	//		{
	//			endZ = f.z;
	//		}
	//	}
	//}

	//gridStartX_ *= navMeshRenderer_->GetTransform()->GetLocalScaling().x;
	//gridStartZ_ *= navMeshRenderer_->GetTransform()->GetLocalScaling().z;
	//endX *= navMeshRenderer_->GetTransform()->GetLocalScaling().x;
	//endZ *= navMeshRenderer_->GetTransform()->GetLocalScaling().z;

	//float XLength = endX - gridStartX_;
	//float ZLength = endZ - gridStartZ_;

	//// ���̿� ���͹��� ���� ��ŭ ��� ������ ������ �̴ϴ�.
	//int nodeCountX = static_cast<int>(XLength / intervalX_) + 2;
	//int nodeCountZ = static_cast<int>(ZLength / intervalZ_) + 2;

	//// ��带 ���� ��ŭ�� �迭 ������ Ȯ���մϴ�.
	//allNodes_.reserve(nodeCountZ);

	//for (std::vector<AStarNode>& v : allNodes_)
	//{
	//	v.reserve(nodeCountX);
	//}

	//// ���ο� ��带 ����� �����մϴ�.
	//int index = 0;
	//for (int z = 0; z < nodeCountZ; z++)
	//{
	//	allNodes_.push_back(std::vector<AStarNode>());
	//	for (int x = 0; x < nodeCountX; x++)
	//	{
	//		float posX = gridStartX_ + x * intervalX_;
	//		float posZ = gridStartZ_ + z * intervalZ_;

	//		AStarNode newNode(index++, z, x, float4(posX, 0.0f, posZ));

	//		allNodes_[z].push_back(newNode);
	//	}
	//}

	//tileVertexBuffer_ = new GameEngineVertexBuffer();
	//tileIndexBuffer_ = new GameEngineIndexBuffer();

	//tileVertices_;
	//tileIndices_;
	//size_t allVertexCount = allNodes_.size() * allNodes_[0].size();
	//tileVertices_.reserve(allVertexCount);
	//tileIndices_.reserve(allVertexCount * 7);

	//int counter = 0;
	//gridZCount_ = static_cast<int>(allNodes_.size());
	//gridXCount_ = static_cast<int>(allNodes_[0].size());

	//for (auto n : allNodes_)
	//{
	//	for (auto m : n)
	//	{
	//		GameEngineVertex v;
	//		v.POSITION = m.GetPosition();
	//		v.COLOR = float4::GREEN;
	//		tileVertices_.push_back(v);
	//	}
	//}

	//for (size_t z = 0; z < gridZCount_ - 1; z++)
	//{
	//	for (size_t x = 0; x < gridXCount_ - 1; x++)
	//	{
	//		size_t currentVertexIndex = z * gridXCount_ + x;
	//		tileIndices_.push_back(static_cast<UINT>(currentVertexIndex));
	//		tileIndices_.push_back(static_cast<UINT>(currentVertexIndex + 1));
	//		tileIndices_.push_back(static_cast<UINT>(currentVertexIndex + gridXCount_));

	//		tileIndices_.push_back(static_cast<UINT>(currentVertexIndex + 1));
	//		tileIndices_.push_back(static_cast<UINT>(currentVertexIndex + gridXCount_ + 1));
	//		tileIndices_.push_back(static_cast<UINT>(currentVertexIndex + gridXCount_));
	//	}
	//}
}

void ShadowTestMap::Start()
{
	GameEngineRenderer* Renderer = CreateTransformComponent<GameEngineRenderer>();
	Renderer->SetRenderingPipeLine("DeferredLightMap");
	Renderer->SetMesh("Box");
	Renderer->ShaderHelper.SettingTexture("DiffuseTex", "TestDif.png");
	Renderer->ShaderHelper.SettingTexture("NormalTex", "TestNor.png");
	Renderer->BumpOn();
	Renderer->GetTransform()->SetLocalScaling({ 10000.0f, 10.0f, 10000.0f });
}

void ShadowTestMap::Update(float _deltaTime)
{
}

ShadowTestMap::ShadowTestMap()
	: Renderer_(nullptr)
{
}

ShadowTestMap::~ShadowTestMap()
{
}