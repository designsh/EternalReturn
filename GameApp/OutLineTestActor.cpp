#include "Precompile.h"
#include "OutLineTestActor.h"

#include <GameEngine/GameEnginePreprocessingRenderer.h>

void OutLineTestActor::TestResourceLoad()
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

void OutLineTestActor::Start()
{
	TestBaseRenderer_ = CreateTransformComponent<GameEngineFBXRenderer>();
	TestBaseRenderer_->SetFBXMesh("Rio_Short_Run.fbx", "TextureDeferredLightAni", true);
	TestBaseRenderer_->CreateFBXAnimation("Run_Short", "Rio_Short_Run.UserAnimation", 0);
	TestBaseRenderer_->CreateFBXAnimation("Wait_Short", "Rio_Short_Wait.UserAnimation", 0);
	TestBaseRenderer_->ChangeFBXAnimation("Run_Short");
	TestBaseRenderer_->GetTransform()->SetLocalScaling({ 100.f, 100.f, 100.f });
	TestBaseRenderer_->GetTransform()->SetLocalRotationDegree({ -90.f, 0.0f });

	TestRenderer_ = CreateTransformComponent<GameEnginePreprocessingRenderer>();
	TestRenderer_->SetBaseRenderer(TestBaseRenderer_, "PreprocessingAni", true);

	if (false == GameEngineInput::GetInst().IsKey("TestChangeKey"))
	{
		GameEngineInput::GetInst().CreateKey("TestChangeKey", VK_LCONTROL);
	}
}

void OutLineTestActor::Update(float _deltaTime)
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
	}
}

OutLineTestActor::OutLineTestActor()
	: IsChange_(false)
	, TestBaseRenderer_(nullptr)
	, TestRenderer_(nullptr)
{
}

OutLineTestActor::~OutLineTestActor()
{
}
