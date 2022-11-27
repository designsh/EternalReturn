#include "PreCompile.h"
#include "ShadowTestActor.h"

#include <GameEngine/GameEngineWindow.h>
#include <GameEngine/GameEngineRenderWindow.h>
#include <GameEngine/GameEngineShadowRenderer.h>

void ShadowTestActor::TestResourceLoad()
{
	GameEngineDirectory dir;

	dir.MoveParent("EternalReturn");
	dir / "Resources" / "FBX" / "Character" / "Rio";
	GameEngineFBXMesh* mesh = GameEngineFBXMeshManager::GetInst().Load(dir.PathToPlusFileName("Rio_Short_Run.fbx"));
	mesh->CreateRenderingBuffer();

	std::vector<GameEngineFile> allFile = dir.GetAllFile("UserAnimation");
	for (GameEngineFile& file : allFile)
	{
		GameEngineFBXAnimationManager::GetInst().LoadUser(file.GetFullPath());
	}
}

void ShadowTestActor::Start()
{
	TestBaseRenderer_ = CreateTransformComponent<GameEngineFBXRenderer>();
	TestBaseRenderer_->SetFBXMesh("Rio_Short_Run.fbx", "TextureDeferredLightAni", true);
	TestBaseRenderer_->CreateFBXAnimation("Run_Short", "Rio_Short_Run.UserAnimation", 0);
	TestBaseRenderer_->CreateFBXAnimation("Wait_Short", "Rio_Short_Wait.UserAnimation", 0);
	TestBaseRenderer_->ChangeFBXAnimation("Run_Short");
	TestBaseRenderer_->GetTransform()->SetLocalScaling({ 100.f, 100.f, 100.f });
	TestBaseRenderer_->GetTransform()->SetLocalRotationDegree({ -90.f, 0.0f });
	TestBaseRenderer_->GetRenderSet(1).isRender = false;
	TestBaseRenderer_->LightShadowOn();

	//ShadowRenderer_ = CreateTransformComponent<GameEngineShadowRenderer>();
	//ShadowRenderer_->SetBaseRenderer(TestBaseRenderer_, "RendererShadow");

	GameEngineRenderer* Renderer = CreateTransformComponent<GameEngineRenderer>(GetTransform());
	Renderer->SetRenderingPipeLine("DeferredColor");
	Renderer->SetMesh("Sphere");
	Renderer->GetTransform()->SetLocalScaling({ 100.0f, 100.0f, 100.0f });
	Renderer->GetTransform()->SetLocalPosition({ 100.0f, 300.0f, 100.0f });
	Renderer->ShaderHelper.SettingConstantBufferSet("ResultColor", float4(0.0f, 1.0f, 0.0f));
	Renderer->LightShadowOn();

	if (false == GameEngineInput::GetInst().IsKey("TestChangeKey"))
	{
		GameEngineInput::GetInst().CreateKey("TestChangeKey", VK_LCONTROL);
	}
}

void ShadowTestActor::Update(float _deltaTime)
{
	if (true == GameEngineInput::GetInst().Down("TestChangeKey"))
	{
		if (false == IsChange_)
		{
			TestBaseRenderer_->ChangeFBXAnimation("Wait_Short");
			IsChange_ = true;
		}
		else
		{
			TestBaseRenderer_->ChangeFBXAnimation("Run_Short");
			IsChange_ = false;
		}

		if (false == First_)
		{
			ShadowRenderTarget_ = GetLevel()->GetMainCamera()->GetLightShadowRenderTarget();
			if (nullptr != ShadowRenderTarget_)
			{
				GameEngineRenderWindow* Window = GameEngineGUI::GetInst()->FindGUIWindowConvert<GameEngineRenderWindow>("RenderWindow");
				if (Window != nullptr)
				{
					float4 Size = { 128, 72 };
					Window->PushRenderTarget("Shadow", ShadowRenderTarget_, Size * 3);
					Window->PushRenderTarget("메인 카메라 타겟", GetLevel()->GetMainCamera()->GetCameraRenderTarget(), Size * 3);
					Window->PushRenderTarget("UI 카메라 타겟", GetLevel()->GetUICamera()->GetCameraRenderTarget(), Size * 3);
					Window->PushRenderTarget("메인 카메라 G-Buffer", GetLevel()->GetMainCamera()->GetCameraDeferredGBufferTarget(), Size * 3);
					Window->PushRenderTarget("메인 카메라 디퍼드 라이트", GetLevel()->GetMainCamera()->GetCameraDeferredLightTarget(), Size * 3);
					Window->PushRenderTarget("메인 카메라 디퍼드 타겟", GetLevel()->GetMainCamera()->GetCameraDeferredTarget(), Size * 3);
				}
			}
			First_ = true;
		}
	}
}

ShadowTestActor::ShadowTestActor()
	: IsChange_(false)
	, First_(false)
	, TestBaseRenderer_(nullptr)
	, ShadowRenderer_(nullptr)
	, ShadowRenderTarget_(nullptr)
{
}

ShadowTestActor::~ShadowTestActor()
{
}
