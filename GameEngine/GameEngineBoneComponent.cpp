#include "PreCompile.h"
#include "GameEngineBoneComponent.h"

#include "GameEngineFBXMesh.h"

GameEngineBoneComponent::GameEngineBoneComponent()
	: parentBoneRenderer_(nullptr)
	, parentBoneIndex_(-1)
{

}

GameEngineBoneComponent::~GameEngineBoneComponent()
{

}

void GameEngineBoneComponent::Start()
{
}

void GameEngineBoneComponent::Update(float _deltaTime)
{
	if (parentBoneRenderer_ == nullptr || parentBoneIndex_ < 0)
	{
		return;
	}


}

void GameEngineBoneComponent::SetParentBoneName(GameEngineFBXRenderer* _fbxRenderer, const std::string& _boneNameToAffect)
{
	parentBoneRenderer_ = _fbxRenderer;


	std::vector<Bone>& bones = _fbxRenderer->GetMesh()->GetAllBones();
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
