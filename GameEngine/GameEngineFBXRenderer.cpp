#include "PreCompile.h"
#include <GameEngine/CameraComponent.h>
#include "GameEngineFBXRenderer.h"
#include "GameEngineLevel.h"
#include "GameEngineRenderingPipeLineManager.h"
#include "GameEngineRenderingPipeLine.h"
#include "GameEngineTransform.h"
#include "GameEngineVertexShader.h"
#include "GameEngineVertexBuffer.h"
#include "GameEngineIndexBuffer.h"
#include "GameEnginePixelShader.h"
#include "CameraComponent.h"

#include "GameEngineFBXMesh.h"
#include "GameEngineFBXMeshManager.h"
#include "GameEngineFBXAnimation.h"
#include "GameEngineFBXAnimationManager.h"
#include "GameEngineTextureManager.h"


GameEngineFBXRenderer::GameEngineFBXRenderer()
	: currentAnimation_(nullptr)
	, overrideAnimation_(nullptr)
	, FBXMesh(nullptr)
	, overrideBoneIndex_(-1)
	, parentBoneIndex_(-1)
	, parentBoneRenderer_(nullptr)
	, customOffest_(0.0f, 0.0f, 0.0f, 0.0f)
{
}

GameEngineFBXRenderer::~GameEngineFBXRenderer()
{
	for (size_t i = 0; i < RenderSets.size(); i++)
	{
		delete RenderSets[i].PipeLine_;
		delete RenderSets[i].ShaderHelper;
	}

	for (auto& Animation : Animations)
	{
		delete Animation.second;
	}
}


void GameEngineFBXRenderer::Render(float _DeltaTime, bool _IsDeferred)
{
	for (size_t i = 0; i < RenderSets.size(); i++)
	{
		if (false == RenderSets[i].isRender)
		{
			continue;
		}

		if (true == _IsDeferred
			&& true == RenderSets[i].PipeLine_->IsDeferred())
		{
			RenderSets[i].ShaderHelper->Setting();
			RenderSets[i].PipeLine_->Rendering();
			RenderSets[i].ShaderHelper->ReSet();
			RenderSets[i].PipeLine_->Reset();
		}
		else if (false == RenderSets[i].PipeLine_->IsDeferred())
		{
			RenderSets[i].ShaderHelper->Setting();
			RenderSets[i].PipeLine_->Rendering();
			RenderSets[i].ShaderHelper->ReSet();
			RenderSets[i].PipeLine_->Reset();
		}
	}
}

void GameEngineFBXRenderer::LightShadowRender(float _DeltaTime)
{
	for (size_t i = 0; i < RenderSets.size(); i++)
	{
		if (false == RenderSets[i].isRender)
		{
			continue;
		}

		// Shadow PipeLine Vertex Buffer & Index Buffer Setting
		DefaultLightShadowPipeLine_->SetInputAssembler1VertexBufferSetting(RenderSets[i].PipeLine_->GetVertexBuffer());
		DefaultLightShadowPipeLine_->SetInputAssembler2IndexBufferSetting(RenderSets[i].PipeLine_->GetIndexBuffer());

		// Shadow ShaderHelper StructuredBuffer Setting & Link
		LightShadowHelper_.SettingStructuredBufferSetting("ArrAniMationMatrix", RenderSets[i].ShaderHelper->GetStructuredBufferSetting("ArrAniMationMatrix"));
		LightShadowHelper_.SettingStructuredBufferLink("ArrAniMationMatrix", &RenderSets[i].BoneData[0], sizeof(float4x4) * RenderSets[i].BoneData.size());
		LightShadowHelper_.Setting();

		// Shadow Rendering
		DefaultLightShadowPipeLine_->Rendering();
	}
}

void GameEngineFBXRenderer::SetFBXMeshRenderSet(const std::string& _Value, std::string _PipeLine, int _MeshIndex)
{
	FBXMesh = GameEngineFBXMeshManager::GetInst().Find(_Value);

	if (nullptr == FBXMesh)
	{
		GameEngineDebug::MsgBoxError("존재하지 않는 fbx매쉬를 세팅했습니다.");
	}

	GameEngineRenderingPipeLine* Pipe = GameEngineRenderingPipeLineManager::GetInst().Find(_PipeLine);

	std::vector<FbxMeshSet>& AllMeshSet = FBXMesh->GetAllMeshMap();

	FbxMeshSet& StartMesh = AllMeshSet[_MeshIndex];


	std::vector<GameEngineVertexBuffer*>& VertexBufferVector = StartMesh.GameEngineVertexBuffers;

	for (int VertexBufferIndex = 0; VertexBufferIndex < VertexBufferVector.size(); VertexBufferIndex++)
	{
		std::vector<GameEngineIndexBuffer*>& IndexBufferVector = StartMesh.GameEngineIndexBuffers[VertexBufferIndex];

		for (int IndexBufferIndex = 0; IndexBufferIndex < IndexBufferVector.size(); IndexBufferIndex++)
		{
			FbxExRenderingPipeLineSettingData* MatData = &(StartMesh.MaterialData[VertexBufferIndex][IndexBufferIndex]);
			GameEngineVertexBuffer* VertexBuffer = VertexBufferVector[VertexBufferIndex];
			GameEngineIndexBuffer* IndexBuffer = IndexBufferVector[IndexBufferIndex];

			RenderSet& RenderSetData = RenderSets.emplace_back();

			float4 AllVtxPos = float4::ZERO;
			float4 MaxVtxValue = float4::ZERO;

			for (size_t i = 0; i < StartMesh.Vertexs.size(); i++)
			{
				float4 Vtx = StartMesh.Vertexs[i].POSITION;

				AllVtxPos += Vtx;

				if (abs(Vtx.x) > abs(MaxVtxValue.x))
				{
					MaxVtxValue.x = Vtx.x;
				}

				if (abs(Vtx.y) > abs(MaxVtxValue.y))
				{
					MaxVtxValue.y = Vtx.y;
				}

				if (abs(Vtx.z) > abs(MaxVtxValue.z))
				{
					MaxVtxValue.z = Vtx.z;
				}
			}

			RenderSetData.LocalPos = AllVtxPos / static_cast<float>(StartMesh.Vertexs.size());

			float4 fScale = MaxVtxValue - RenderSetData.LocalPos;

			RenderSetData.Radius = max(max(abs(fScale.x), abs(fScale.y)), abs(fScale.z));
			RenderSetData.Index = _MeshIndex;

			RenderSetData.PipeLine_ = Pipe->Clone();
			RenderSetData.ShaderHelper = new GameEngineShaderResHelper();

			RenderSetData.ShaderHelper->ShaderResourcesCheck(RenderSetData.PipeLine_->GetPixelShader());
			RenderSetData.ShaderHelper->ShaderResourcesCheck(RenderSetData.PipeLine_->GetVertexShader());

			if (true == RenderSetData.ShaderHelper->IsConstantBuffer("TransformData"))
			{
				RenderSetData.ShaderHelper->SettingConstantBufferLink("TransformData", GetTransform()->GetTransformData());
			}

			if (true == RenderSetData.ShaderHelper->IsConstantBuffer("LightsData"))
			{
				const LightsData& LightData = GetLevel()->GetMainCamera()->GetLightData();
				RenderSetData.ShaderHelper->SettingConstantBufferLink("LightsData", LightData);
			}

			if (true == RenderSetData.ShaderHelper->IsConstantBuffer("RendererData"))
			{
				RenderSetData.ShaderHelper->SettingConstantBufferLink("RendererData", RendererDataInst);
			}

			if (true == RenderSetData.ShaderHelper->IsTextureSetting("DiffuseTex"))
			{
				GameEngineTexture* Tex = GameEngineTextureManager::GetInst().Find(GameEnginePath::GetFileName(MatData->DifTexturePath));

				if ("" != MatData->DifTexturePath)
				{
					if (Tex == nullptr && true == GameEnginePath::IsExist(MatData->DifTexturePath))
					{
						GameEngineTextureManager::GetInst().Load(MatData->DifTexturePath);
						Tex = GameEngineTextureManager::GetInst().Find(GameEnginePath::GetFileName(MatData->DifTexturePath));
					}

					if (Tex != nullptr)
					{
						RenderSetData.ShaderHelper->SettingTexture("DiffuseTex", Tex);
					}
				}
			}


			if (true == RenderSetData.ShaderHelper->IsTextureSetting("NormalTex"))
			{
				GameEngineTexture* Tex = GameEngineTextureManager::GetInst().Find(GameEnginePath::GetFileName(MatData->NorTexturePath));

				if ("" != MatData->NorTexturePath)
				{
					if (Tex == nullptr && true == GameEnginePath::IsExist(MatData->NorTexturePath))
					{
						GameEngineTextureManager::GetInst().Load(MatData->NorTexturePath);
						Tex = GameEngineTextureManager::GetInst().Find(GameEnginePath::GetFileName(MatData->NorTexturePath));
					}

					if (Tex != nullptr)
					{
						RenderSetData.ShaderHelper->SettingTexture("NormalTex", Tex);
						RendererDataInst.IsBump = 1;
					}
				}
			}

			if (true == RenderSetData.ShaderHelper->IsStructuredBuffer("ArrAniMationMatrix"))
			{
				FBXMesh->ImportBone();
				if (0 == RenderSetData.BoneData.size())
				{
					RenderSetData.BoneData.resize(FBXMesh->GetBoneCount(VertexBufferIndex));
				}
				RenderSetData.ShaderHelper->SettingStructuredBufferSetting("ArrAniMationMatrix", FBXMesh->GetAnimationBuffer(VertexBufferIndex));
				RenderSetData.ShaderHelper->SettingStructuredBufferLink("ArrAniMationMatrix", &RenderSetData.BoneData[0], sizeof(float4x4) * RenderSetData.BoneData.size());

				RendererDataInst.IsAni = 1;
			}

			RenderSetData.PipeLine_->SetInputAssembler1VertexBufferSetting(VertexBuffer);
			RenderSetData.PipeLine_->SetInputAssembler2IndexBufferSetting(IndexBuffer);
		}
	}
}

void GameEngineFBXRenderer::SetFBXMeshRenderSetChar(const std::string& _Value, std::string _PipeLine, int _MeshIndex)
{
	FBXMesh = GameEngineFBXMeshManager::GetInst().Find(_Value);

	if (nullptr == FBXMesh)
	{
		GameEngineDebug::MsgBoxError("존재하지 않는 fbx매쉬를 세팅했습니다.");
	}

	GameEngineRenderingPipeLine* Pipe = GameEngineRenderingPipeLineManager::GetInst().Find(_PipeLine);

	std::vector<FbxMeshSet>& AllMeshSet = FBXMesh->GetAllMeshMap();

	FbxMeshSet& StartMesh = AllMeshSet[_MeshIndex];


	std::vector<GameEngineVertexBuffer*>& VertexBufferVector = StartMesh.GameEngineVertexBuffers;

	for (int VertexBufferIndex = 0; VertexBufferIndex < VertexBufferVector.size(); VertexBufferIndex++)
	{
		std::vector<GameEngineIndexBuffer*>& IndexBufferVector = StartMesh.GameEngineIndexBuffers[VertexBufferIndex];

		for (int IndexBufferIndex = 0; IndexBufferIndex < IndexBufferVector.size(); IndexBufferIndex++)
		{
			FbxExRenderingPipeLineSettingData* MatData = &(StartMesh.MaterialData[VertexBufferIndex][IndexBufferIndex]);
			GameEngineVertexBuffer* VertexBuffer = VertexBufferVector[VertexBufferIndex];
			GameEngineIndexBuffer* IndexBuffer = IndexBufferVector[IndexBufferIndex];

			RenderSet& RenderSetData = RenderSets[_MeshIndex];

			float4 AllVtxPos = float4::ZERO;
			float4 MaxVtxValue = float4::ZERO;

			for (size_t i = 0; i < StartMesh.Vertexs.size(); i++)
			{
				float4 Vtx = StartMesh.Vertexs[i].POSITION;

				AllVtxPos += Vtx;

				if (abs(Vtx.x) > abs(MaxVtxValue.x))
				{
					MaxVtxValue.x = Vtx.x;
				}

				if (abs(Vtx.y) > abs(MaxVtxValue.y))
				{
					MaxVtxValue.y = Vtx.y;
				}

				if (abs(Vtx.z) > abs(MaxVtxValue.z))
				{
					MaxVtxValue.z = Vtx.z;
				}
			}

			RenderSetData.LocalPos = AllVtxPos / static_cast<float>(StartMesh.Vertexs.size());

			float4 fScale = MaxVtxValue - RenderSetData.LocalPos;

			RenderSetData.Radius = max(max(abs(fScale.x), abs(fScale.y)), abs(fScale.z));
			RenderSetData.Index = _MeshIndex;

			RenderSetData.PipeLine_ = Pipe->Clone();
			RenderSetData.ShaderHelper = new GameEngineShaderResHelper();

			RenderSetData.ShaderHelper->ShaderResourcesCheck(RenderSetData.PipeLine_->GetPixelShader());
			RenderSetData.ShaderHelper->ShaderResourcesCheck(RenderSetData.PipeLine_->GetVertexShader());

			if (true == RenderSetData.ShaderHelper->IsConstantBuffer("TransformData"))
			{
				RenderSetData.ShaderHelper->SettingConstantBufferLink("TransformData", GetTransform()->GetTransformData());
			}

			if (true == RenderSetData.ShaderHelper->IsConstantBuffer("LightsData"))
			{
				const LightsData& LightData = GetLevel()->GetMainCamera()->GetLightData();
				RenderSetData.ShaderHelper->SettingConstantBufferLink("LightsData", LightData);
			}

			if (true == RenderSetData.ShaderHelper->IsConstantBuffer("RendererData"))
			{
				RenderSetData.ShaderHelper->SettingConstantBufferLink("RendererData", RendererDataInst);
			}

			if (true == RenderSetData.ShaderHelper->IsTextureSetting("DiffuseTex"))
			{
				GameEngineTexture* Tex = GameEngineTextureManager::GetInst().Find(GameEnginePath::GetFileName(MatData->DifTexturePath));

				if ("" != MatData->DifTexturePath)
				{
					if (Tex == nullptr && true == GameEnginePath::IsExist(MatData->DifTexturePath))
					{
						GameEngineTextureManager::GetInst().Load(MatData->DifTexturePath);
						Tex = GameEngineTextureManager::GetInst().Find(GameEnginePath::GetFileName(MatData->DifTexturePath));
					}

					if (Tex != nullptr)
					{
						RenderSetData.ShaderHelper->SettingTexture("DiffuseTex", Tex);
					}
				}
			}

			if (true == RenderSetData.ShaderHelper->IsTextureSetting("NormalTex"))
			{
				GameEngineTexture* Tex = GameEngineTextureManager::GetInst().Find(GameEnginePath::GetFileName(MatData->NorTexturePath));

				if ("" != MatData->NorTexturePath)
				{
					if (Tex == nullptr && true == GameEnginePath::IsExist(MatData->NorTexturePath))
					{
						GameEngineTextureManager::GetInst().Load(MatData->NorTexturePath);
						Tex = GameEngineTextureManager::GetInst().Find(GameEnginePath::GetFileName(MatData->NorTexturePath));
					}

					if (Tex != nullptr)
					{
						RenderSetData.ShaderHelper->SettingTexture("NormalTex", Tex);
						RendererDataInst.IsBump = 1;
					}
				}
			}

			if (true == RenderSetData.ShaderHelper->IsStructuredBuffer("ArrAniMationMatrix"))
			{
				FBXMesh->ImportBone();
				if (0 == RenderSetData.BoneData.size())
				{
					RenderSetData.BoneData.resize(FBXMesh->GetBoneCount(VertexBufferIndex));
				}
				RenderSetData.ShaderHelper->SettingStructuredBufferSetting("ArrAniMationMatrix", FBXMesh->GetAnimationBuffer(VertexBufferIndex));
				RenderSetData.ShaderHelper->SettingStructuredBufferLink("ArrAniMationMatrix", &RenderSetData.BoneData[0], sizeof(float4x4) * RenderSetData.BoneData.size());

				RendererDataInst.IsAni = 1;
			}

			RenderSetData.PipeLine_->SetInputAssembler1VertexBufferSetting(VertexBuffer);
			RenderSetData.PipeLine_->SetInputAssembler2IndexBufferSetting(IndexBuffer);
		}
	}
}

void GameEngineFBXRenderer::SetFBXMesh(const std::string& _Value, std::string _PipeLine, bool _bChar)
{
	FBXMesh = GameEngineFBXMeshManager::GetInst().Find(_Value);

	if (nullptr == FBXMesh)
	{
		GameEngineDebug::MsgBoxError("존재하지 않는 fbx매쉬를 세팅했습니다.");
	}

	GameEngineRenderingPipeLine* Pipe = GameEngineRenderingPipeLineManager::GetInst().Find(_PipeLine);

	std::vector<FbxMeshSet>& AllMeshSet = FBXMesh->GetAllMeshMap();

	if (_bChar)
	{
		RenderSets.resize(AllMeshSet.size());
		for (int i = 0; i < AllMeshSet.size(); ++i)
		{
			SetFBXMeshRenderSetChar(_Value, _PipeLine, i);
		}
	}
	else
	{
		for (int i = 0; i < AllMeshSet.size(); ++i)
		{
			SetFBXMeshRenderSet(_Value, _PipeLine, i);
		}
	}

}

float4x4 GameEngineFBXRenderer::GetCurrentAffine(int _boneIndex, int _renderSetIndex)
{
	if (currentAnimation_ == nullptr)
	{
		return float4x4();
	}
	return currentAnimation_->GetAffine(_boneIndex, _renderSetIndex);
}

float4x4 GameEngineFBXRenderer::GetParentAffine()
{
	if (parentBoneRenderer_ == nullptr)
	{
		return float4x4();
	}
	return parentBoneRenderer_->GetCurrentAffine(parentBoneIndex_);
}

void GameEngineFBXRenderer::SetParentBoneIndex(GameEngineFBXRenderer* _fbxRenderer, int _boneIndex)
{
	parentBoneRenderer_ = _fbxRenderer;
	parentBoneIndex_ = _boneIndex;
}

void GameEngineFBXRenderer::SetParentBoneName(GameEngineFBXRenderer* _fbxRenderer, const std::string& _boneNameToAffect)
{
	parentBoneRenderer_ = _fbxRenderer;


	std::vector<Bone>& bones = _fbxRenderer->FBXMesh->GetAllBones();
	int boneCount = static_cast<int>(bones.size());

	parentBoneIndex_ = -1;
	for (int i = 0; i < boneCount; i++)
	{
		if (bones[i].Name == _boneNameToAffect)
		{
			parentBoneIndex_ = i;
			break;
		}
	}

	if (parentBoneIndex_ == -1)
	{
		GameEngineDebug::MsgBoxError("적용시킬 본이 없습니다.");
		return;
	}

}

void GameEngineFBXRenderer::ClearParentBone()
{
	parentBoneRenderer_ = nullptr;
	parentBoneIndex_ = -1;
}

void GameEngineFBXRenderer::SetPreprocessingRenderer(GameEnginePreprocessingRenderer* _PreprocessingRenderer)
{
	// 카메라 렌더링 목록에 추가
	GetLevel()->GetMainCamera()->PushPreprocessingRenderer(this, _PreprocessingRenderer);

	// 전처리 렌더러 사용
	PreprocessingOn();
}

void GameEngineFBXRenderer::SetShadowRenderer(GameEngineShadowRenderer* _ShadowRenderer)
{
	// 카메라 렌더링 목록에 추가
	GetLevel()->GetMainCamera()->PushShadowRenderer(this, _ShadowRenderer);

	// 그림자 렌더러 사용
	RendererShadowOn();
}

void GameEngineFBXRenderer::Start()
{
	GetLevel()->GetMainCamera()->PushRenderer(GetOrder(), this);
}

void GameEngineFBXRenderer::Update(float _DeltaTime)
{
	if (nullptr != currentAnimation_)
	{
		if (nullptr != overrideAnimation_)
		{
			currentAnimation_->UpdateOverride(_DeltaTime, overrideAnimation_);
		}
		else
		{
			currentAnimation_->Update(_DeltaTime);
		}
	}
}


void GameEngineFBXRenderer::CreateFBXAnimation(const std::string& _AnimationName, const std::string& _AnimationFBXName, int _AnimationIndex, bool _isLoop, float _frameTime)
{   // 0805 애니메이션 루프여부 추가
	if (nullptr == FBXMesh)
	{
		GameEngineDebug::MsgBoxError("매쉬를 세팅하지 않은 랜더러에 애니메이션을 만들 수가 없습니다.");
		return;
	}

	if (Animations.end() != Animations.find(_AnimationName))
	{
		GameEngineDebug::MsgBoxError("같은 이름의 애니메이션을 또 만들수는 없습니다.");
		return;
	}

	GameEngineFBXAnimation* Animation = GameEngineFBXAnimationManager::GetInst().Find(_AnimationFBXName);

	if (nullptr == Animation)
	{
		GameEngineDebug::MsgBoxError("로드하려는 애니메이션 정보가 없습니다.");
		return;
	}

	FBXAnimation* NewFBXAnimation = new FBXAnimation();

	NewFBXAnimation->Mesh = FBXMesh;
	NewFBXAnimation->Animation = Animation;
	NewFBXAnimation->ParentRenderer = this;
	NewFBXAnimation->AnimationName_ = _AnimationName;
	NewFBXAnimation->Init(_AnimationIndex, _isLoop, _frameTime);


	Animations.insert(std::make_pair(_AnimationName, NewFBXAnimation));

}

void GameEngineFBXRenderer::ChangeFBXAnimation(const std::string& _AnimationName, bool _bForceChange)
{
	std::map<std::string, FBXAnimation*>::iterator FindIter = Animations.find(_AnimationName);

	if (Animations.end() == FindIter)
	{
		GameEngineDebug::MsgBoxError("존재하지 않는 애니메이션으로 체인지 하려고 했습니다.");
		return;
	}

	if (_bForceChange == false && currentAnimation_ == FindIter->second)
	{
		return;
	}

	currentAnimation_ = FindIter->second;
	currentAnimation_->ResetFrame();
}

void GameEngineFBXRenderer::OverrideFBXAnimation(const std::string& _AnimationName, const std::string& _boneNameToAffect, bool _bForceChange)
{
	if (currentAnimation_ == nullptr)
	{
		GameEngineDebug::MsgBoxError("덮어씌울 애니메이션이 없습니다.");
		return;
	}

	std::map<std::string, FBXAnimation*>::iterator FindIter = Animations.find(_AnimationName);

	if (Animations.end() == FindIter)
	{
		GameEngineDebug::MsgBoxError("존재하지 않는 애니메이션으로 체인지 하려고 했습니다.");
		return;
	}

	if (_bForceChange == false && overrideAnimation_ == FindIter->second)
	{
		return;
	}

	overrideAnimation_ = FindIter->second;
	overrideAnimation_->ResetFrame();

	// 본 세팅
	std::vector<Bone>& bones = FBXMesh->GetAllBones();
	int boneCount = static_cast<int>(bones.size());

	int boneIndex = -1;
	for (int i = 0; i < boneCount; i++)
	{
		if (bones[i].Name == _boneNameToAffect)
		{
			boneIndex = i;
			break;
		}
	}

	if (boneIndex == -1)
	{
		GameEngineDebug::MsgBoxError("적용시킬 본이 없습니다.");
		return;
	}

	if (overrideBoneIndexCache_.empty() || overrideBoneIndexCache_.end() == overrideBoneIndexCache_.find(boneIndex))
	{
		overrideBoneIndexCache_.clear();
		overrideBoneIndexCache_.insert(std::pair(boneIndex, boneIndex));

		std::vector<int> temp;
		temp.push_back(boneIndex);

		while (!temp.empty())
		{
			int currentIndex = temp.back();
			temp.pop_back();

			for (int i = 0; i < boneCount; i++)
			{
				Bone* currentBone = &bones[i];
				if (currentBone->ParentIndex == currentIndex)
				{
					overrideBoneIndexCache_.insert(std::pair(currentBone->Index, currentBone->Index));

					if (temp.end() == std::find(temp.begin(), temp.end(), currentBone->Index))
					{
						temp.push_back(currentBone->Index);
					}
				}
			}
		}
	}

	overrideBoneIndex_ = boneIndex;
}

void GameEngineFBXRenderer::ClearOverrideAnimation()
{
	overrideAnimation_ = nullptr;
}

bool GameEngineFBXRenderer::IsCurrentAnimationEnd()
{
	if (nullptr == currentAnimation_)
	{
		return false;
	}

	return currentAnimation_->bEnd_;
}

bool GameEngineFBXRenderer::IsOverrideAnimationEnd()
{
	if (nullptr == overrideAnimation_)
	{
		return false;
	}

	return overrideAnimation_->bEnd_;
}


bool GameEngineFBXRenderer::CheckIntersects(const float4& _Position,
	const float4& _Direction, float& _Distance)
{
	std::vector<FbxMeshSet>& vecMeshMap = FBXMesh->GetAllMeshMap();
	std::vector<FbxExMeshInfo>& vecMeshInfos = FBXMesh->GetMeshInfos();

	bool Check = false;

	float4 Dir = _Direction.NormalizeReturn3D();

	for (size_t i = 0; i < vecMeshMap.size(); i++)
	{
		for (size_t j = 0; j < vecMeshMap[i].Indexs[0].size(); ++j)
		{
			for (size_t k = 0; k < vecMeshInfos[i].FaceNum; k++)
			{
				float4 V0 = vecMeshMap[i].Vertexs[vecMeshMap[i].Indexs[0][j][k * 3 + 0]].POSITION * GetTransform()->GetTransformData().WorldWorld_;
				float4 V1 = vecMeshMap[i].Vertexs[vecMeshMap[i].Indexs[0][j][k * 3 + 1]].POSITION * GetTransform()->GetTransformData().WorldWorld_;
				float4 V2 = vecMeshMap[i].Vertexs[vecMeshMap[i].Indexs[0][j][k * 3 + 2]].POSITION * GetTransform()->GetTransformData().WorldWorld_;

				Check = DirectX::TriangleTests::Intersects(_Position.DirectVector,
					Dir.DirectVector,
					V0.DirectVector,
					V1.DirectVector,
					V2.DirectVector,
					_Distance);

				if (true == Check)
				{
					return true;
				}
			}
		}
	}

	return false;
}

bool GameEngineFBXRenderer::CheckMeshToPointCollision(const float4& _Position,
	const float4& _Direction, const float _Range)
{
	float dist = 0.0f;

	if (false == CheckIntersects(_Position, _Direction, dist))
	{
		return false;
	}

	if (_Range >= dist
		&& 0.0f < dist)
	{
		return true;
	}

	return false;
}

inline std::vector<std::string> GameEngineFBXRenderer::GetAnimationNameList()
{
	std::vector<std::string> ReturnNameList;

	for (auto AnimName : Animations)
	{
		ReturnNameList.push_back(AnimName.first);
	}

	return ReturnNameList;
}
