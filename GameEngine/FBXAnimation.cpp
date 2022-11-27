#include "PreCompile.h"

#include "GameEngineFBXMesh.h"
#include "GameEngineFBXMeshManager.h"
#include "GameEngineFBXAnimation.h"
#include "GameEngineFBXAnimationManager.h"
#include "GameEngineTextureManager.h"

void FBXAnimation::Init(int _Index, bool _isLoop, float _frameTime)
{
	Animation->CalFbxExBoneFrameTransMatrix(Mesh, _Index);
	PixAniData = &Animation->AnimationDatas[_Index];
	Start = 0;



	End = static_cast<UINT>(PixAniData->TimeEndCount);
	bEnd_ = false;



	FrameTime = _frameTime;

	// 0805 ������ : �ִϸ��̼� ���� �� ����/�����ũ�� �������� bool ���Դϴ�.
	isLoop_ = _isLoop;
}

void FBXAnimation::Update(float _DeltaTime)
{
	CurFrameTime += _DeltaTime;

	while (CurFrameTime >= FrameTime)
	{
		CurFrameTime -= FrameTime;

		++CurFrame;

		if (CurFrame >= End) // �� �������� �� �����ӿ� �ٴٶ��� ��
		{
			// 0805 ������ : ���� �ִϸ��̼��� �ƴϸ� �� �̻��� ������ ������ ���� �ʽ��ϴ�.
			if (false == isLoop_)
			{
				bEnd_ = true;
				return;
			}

			CurFrame = Start;
		}
	}

	int NextFrame = CurFrame;
	++NextFrame;

	if (CurFrame >= End) // �� �������� �� �����ӿ� �ٴٶ��� ��
	{
		// 0805 ������ : ���� �ִϸ��̼��� �ƴϸ� �� �̻��� ������ ������ ���� �ʽ��ϴ�.
		if (false == isLoop_)
		{
			bEnd_ = true;
			return;
		}

		CurFrame = Start;
	}

	if (NextFrame >= static_cast<int>(End))
	{
		NextFrame = 0;
	}


	for (int o = 0; o < ParentRenderer->RenderSets.size(); o++)
	{
		RenderSet& Render = ParentRenderer->RenderSets[o];

		for (int i = 0; i < Render.BoneData.size(); i++)  // �޽��� ��� ���� for ������ ���� �ִϸ��̼��� ���Ž�����.
		{
			Bone* BoneData = ParentRenderer->FBXMesh->FindBone(Render.Index, i);

			if (true == PixAniData->AniFrameData[Render.Index][i].BoneMatData.empty())
			{
				// ���� �����ӵ����Ͱ� �������� �ֽ��ϴ�.
				// ���� �׵������ �� �ִ� ��찡 �����ϴ�.

				// GameEngineDebug::MsgBoxError("������� ������!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
				Render.BoneData[i] = float4x4::Affine(BoneData->BonePos.GlobalScale, BoneData->BonePos.GlobalRotation, BoneData->BonePos.GlobalTranslation);
				return;
			}

			// ���������Ӱ� 
			FbxExBoneFrameData& CurData = PixAniData->AniFrameData[Render.Index][i].BoneMatData[CurFrame];
			// ������������ ������ �ʿ��ѵ�
			FbxExBoneFrameData& NextData = PixAniData->AniFrameData[Render.Index][i].BoneMatData[NextFrame];

			if (CurData.FrameMat == NextData.FrameMat)
			{
				int a = 0;
				return;
			}


			float4 LerpScale = float4::Lerp(CurData.S, NextData.S, CurFrameTime);
			float4 SLerpQ = float4::SLerp(CurData.Q, NextData.Q, CurFrameTime);
			float4 LerpPos = float4::Lerp(CurData.T, NextData.T, CurFrameTime);


			if (ParentRenderer->parentBoneRenderer_ != nullptr)
			{
				float4x4 mat;
				if (ParentRenderer->parentBoneRenderer_->overrideAnimation_ != nullptr)
				{
					mat = ParentRenderer->parentBoneRenderer_->overrideAnimation_->GetAffine(ParentRenderer->parentBoneIndex_);
				}
				else
				{
					mat = ParentRenderer->parentBoneRenderer_->GetCurrentAffine(ParentRenderer->parentBoneIndex_);
				}
				
				
				float4x4 mat2 = float4x4::Affine(LerpScale, SLerpQ, LerpPos);
				Render.BoneData[i] = BoneData->BonePos.Offset * mat2 * mat;
				Render.BoneData[i].vw += Render.BoneData[i].vx * ParentRenderer->customOffest_.x;
				Render.BoneData[i].vw += Render.BoneData[i].vy * ParentRenderer->customOffest_.y;
				Render.BoneData[i].vw += Render.BoneData[i].vz * ParentRenderer->customOffest_.z;
			}
			else
			{
				Render.BoneData[i] = BoneData->BonePos.Offset * float4x4::Affine(LerpScale, SLerpQ, LerpPos);
			}
		}
	}
}

void FBXAnimation::UpdateOverride(float _deltaTime, FBXAnimation* _overrideAnimation)
{
	FBXAnimation* oa = _overrideAnimation;

	CurFrameTime += _deltaTime;


	while (CurFrameTime >= FrameTime)
	{
		CurFrameTime -= FrameTime;
		++CurFrame;

		if (CurFrame >= End)
		{
			if (false == isLoop_)
			{
				bEnd_ = true;
				return;
			}

			CurFrame = Start;
		}
	}

	int NextFrame = CurFrame;
	++NextFrame;

	if (CurFrame >= End)
	{
		if (false == isLoop_)
		{
			bEnd_ = true;
			return;
		}

		CurFrame = Start;
	}

	if (NextFrame >= static_cast<int>(End))
	{
		NextFrame = 0;
	}

	oa->CurFrameTime += _deltaTime;
	while (oa->CurFrameTime >= oa->FrameTime)
	{
		oa->CurFrameTime -= oa->FrameTime;
		++oa->CurFrame;

		if (oa->CurFrame >= oa->End)
		{
			if (false == oa->isLoop_)
			{
				oa->bEnd_ = true;
				return;
			}

			oa->CurFrame = oa->Start;
		}
	}
	int oaNextFrame = oa->CurFrame;
	++oaNextFrame;

	if (oa->CurFrame >= oa->End)
	{
		if (false == oa->isLoop_)
		{
			oa->bEnd_ = true;
			return;
		}

		oa->CurFrame = oa->Start;
	}

	if (oaNextFrame >= static_cast<int>(oa->End))
	{
		oaNextFrame = 0;
	}


	for (int o = 0; o < ParentRenderer->RenderSets.size(); o++)
	{
		RenderSet& Render = ParentRenderer->RenderSets[o];

		for (int i = 0; i < Render.BoneData.size(); i++)
		{
			Bone* BoneData = ParentRenderer->FBXMesh->FindBone(Render.Index, i);

			if (ParentRenderer->overrideBoneIndexCache_.end() == ParentRenderer->overrideBoneIndexCache_.find(BoneData->Index))
			{
				if (true == PixAniData->AniFrameData[Render.Index][i].BoneMatData.empty())
				{
					Render.BoneData[i] = float4x4::Affine(BoneData->BonePos.GlobalScale, BoneData->BonePos.GlobalRotation, BoneData->BonePos.GlobalTranslation);
					return;
				}

				FbxExBoneFrameData& CurData = PixAniData->AniFrameData[Render.Index][i].BoneMatData[CurFrame];
				FbxExBoneFrameData& NextData = PixAniData->AniFrameData[Render.Index][i].BoneMatData[NextFrame];

				if (CurData.FrameMat == NextData.FrameMat)
				{
					return;
				}

				float4 LerpScale = float4::Lerp(CurData.S, NextData.S, CurFrameTime);
				float4 SLerpQ = float4::SLerp(CurData.Q, NextData.Q, CurFrameTime);
				float4 LerpPos = float4::Lerp(CurData.T, NextData.T, CurFrameTime);

				Render.BoneData[i] = BoneData->BonePos.Offset * float4x4::Affine(LerpScale, SLerpQ, LerpPos);
			}
			else
			{
				if (true == oa->PixAniData->AniFrameData[Render.Index][i].BoneMatData.empty())
				{
					Render.BoneData[i] = float4x4::Affine(BoneData->BonePos.GlobalScale, BoneData->BonePos.GlobalRotation, BoneData->BonePos.GlobalTranslation);
					return;
				}


				FbxExBoneFrameData& oaCurData = oa->PixAniData->AniFrameData[Render.Index][i].BoneMatData[oa->CurFrame];
				FbxExBoneFrameData& oaNextData = oa->PixAniData->AniFrameData[Render.Index][i].BoneMatData[oaNextFrame];

				if (oaCurData.FrameMat == oaNextData.FrameMat)
				{
					return;
				}

				float4 oaLerpScale = float4::Lerp(oaCurData.S, oaNextData.S, oa->CurFrameTime);
				float4 oaSLerpQ = float4::SLerp(oaCurData.Q, oaNextData.Q, oa->CurFrameTime);
				float4 oaLerpPos = float4::Lerp(oaCurData.T, oaNextData.T, oa->CurFrameTime);

				float4 originalPos = PixAniData->AniFrameData[Render.Index][ParentRenderer->overrideBoneIndex_].BoneMatData[CurFrame].T;
				float4 overridePos = oa->PixAniData->AniFrameData[Render.Index][ParentRenderer->overrideBoneIndex_].BoneMatData[oa->CurFrame].T;
				float4 offsetPos = originalPos - overridePos;

				float4x4 matOverrideAnim = float4x4::Affine(oaLerpScale, oaSLerpQ, oaLerpPos);

				// ���� �ִϸ��̼ǿ� �� ������ ����
				matOverrideAnim.vw += offsetPos;

				Render.BoneData[i] = BoneData->BonePos.Offset * matOverrideAnim;

			}
		}
	}
}



// 0805 ������
// ChangeAnimation() �� �������� ��� �������� �ʱ�ȭ���ݴϴ�.
void FBXAnimation::ResetFrame()
{
	CurFrame = Start;
	CurFrameTime = 0.0f;
	End = static_cast<UINT>(PixAniData->TimeEndCount);
	bEnd_ = false;
}

float4x4 FBXAnimation::GetAffine(int _boneIndex, int _renderSetIndex)
{
	RenderSet& Render = ParentRenderer->RenderSets[_renderSetIndex];
	Bone* BoneData = ParentRenderer->FBXMesh->FindBone(Render.Index, _boneIndex);

	if (true == PixAniData->AniFrameData[Render.Index][_boneIndex].BoneMatData.empty())
	{
		Render.BoneData[_boneIndex] = float4x4::Affine(BoneData->BonePos.GlobalScale, BoneData->BonePos.GlobalRotation, BoneData->BonePos.GlobalTranslation);
		return float4x4();
	}

	int NextFrame = CurFrame + 1;

	if (NextFrame >= static_cast<int>(End))
	{
		NextFrame = 0;
	}

	int currentFrame = CurFrame;
	if (currentFrame >= static_cast<int>(End))
	{
		currentFrame = Start;
	}

	FbxExBoneFrameData& CurData = PixAniData->AniFrameData[Render.Index][_boneIndex].BoneMatData[currentFrame];
	FbxExBoneFrameData& NextData = PixAniData->AniFrameData[Render.Index][_boneIndex].BoneMatData[NextFrame];

	if (CurData.FrameMat == NextData.FrameMat)
	{
		return float4x4();
	}

	float4 LerpScale = float4::Lerp(CurData.S, NextData.S, CurFrameTime);
	float4 SLerpQ = float4::SLerp(CurData.Q, NextData.Q, CurFrameTime);
	float4 LerpPos = float4::Lerp(CurData.T, NextData.T, CurFrameTime);

	return float4x4::Affine(LerpScale, SLerpQ, LerpPos);
}
