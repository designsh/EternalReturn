#include "PreCompile.h"
#include "AyaESpectrum.h"

#include <GameEngine/GameEngineFBXRenderer.h>

AyaESpectrum::AyaESpectrum() // default constructer 디폴트 생성자
	: count_(0), isStopped_(false)
{

}

AyaESpectrum::~AyaESpectrum() // default destructer 디폴트 소멸자
{

}

AyaESpectrum::AyaESpectrum(AyaESpectrum&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}

void AyaESpectrum::PlayAwake()
{
	count_++;
	renderState_ << "Awake";
}

void AyaESpectrum::Reset()
{
	count_ = 0;
	isStopped_ = true;
}

void AyaESpectrum::Start()
{
	GameEngineFBXRenderer* renderer0 = CreateTransformComponent<GameEngineFBXRenderer>();
	renderer0->SetFBXMesh("skillE_01.fbx", "Color", false);
	renderers_.push_back(renderer0);

	GameEngineFBXRenderer* renderer1 = CreateTransformComponent<GameEngineFBXRenderer>();
	renderer1->SetFBXMesh("skillE_02.fbx", "Color", false);
	renderers_.push_back(renderer1);

	GameEngineFBXRenderer* renderer2 = CreateTransformComponent<GameEngineFBXRenderer>();
	renderer2->SetFBXMesh("skillE_03.fbx", "Color", false);
	renderers_.push_back(renderer2);

	GameEngineFBXRenderer* renderer3 = CreateTransformComponent<GameEngineFBXRenderer>();
	renderer3->SetFBXMesh("skillE_04.fbx", "Color", false);
	renderers_.push_back(renderer3);

	GameEngineFBXRenderer* renderer4 = CreateTransformComponent<GameEngineFBXRenderer>();
	renderer4->SetFBXMesh("skillE_05.fbx", "Color", false);

	renderers_.push_back(renderer4);

	for (int i = 0; i < renderers_.size(); i++)
	{
		renderers_[i]->GetRenderSet(0).ShaderHelper->SettingConstantBufferSet("ResultColor", float4{ 0.5f, 0.5f, 1.0f, 0.1f});
		renderers_[i]->GetTransform()->SetLocalPosition({ 0.0f,0.0f, 70.0f * i });
		renderers_[i]->GetTransform()->SetLocalRotationDegree({90.0f ,0.0f, 0.0f});
		renderers_[i]->Off();
	}

	// 0 1 2 3 4 4 3 2 1
	renderState_.CreateState(MakeState(AyaESpectrum, Sleep));
	renderState_.CreateState(MakeState(AyaESpectrum, Awake));
	renderState_.CreateState(MakeState(AyaESpectrum, Fade));
	renderState_ << "Sleep";
}

void AyaESpectrum::Update(float _deltaTime)
{
	renderState_.Update(_deltaTime);
}

void AyaESpectrum::startSleep()
{
	revealTime_ = 0.0f;
	isStopped_ = false;
	for (int i = 0; i < renderers_.size(); i++)
	{
		renderers_[i]->Off();
	}
}

void AyaESpectrum::updateSleep(float _deltaTime)
{
}


void AyaESpectrum::startAwake()
{
	// 첫 위치 지정

}

void AyaESpectrum::updateAwake(float _deltaTime)
{
	revealTime_ += _deltaTime;
	if (0.5f <= revealTime_)
	{
		revealTime_ = 0.0f;
		count_ = 0;
		renderState_ << "Fade";
		return;
	}
	if (false == isStopped_)
	{


		if (0 == count_)
		{
			renderers_[0]->On();
			count_ = 1;
		}

		if (1 == count_ && 0.1f <= revealTime_)
		{
			renderers_[1]->On();
			count_ = 2;
		}

		if (2 == count_ && 0.2f <= revealTime_)
		{
			renderers_[2]->On();
			count_ = 3;
		}

		if (3 == count_ && 0.3f <= revealTime_)
		{
			renderers_[3]->On();
			count_ = 4;
		}

		if (4 == count_ && 0.4f <= revealTime_)
		{
			renderers_[4]->On();
			count_ = 5;
		}
	}

}



void AyaESpectrum::startFade()
{
}

void AyaESpectrum::updateFade(float _deltaTime)
{
	revealTime_ += _deltaTime;

	if (0.5f <= revealTime_)
	{
		revealTime_ = 0.0f;
		count_ = 0;
		renderState_ << "Sleep";
		return;
	}

	if (0 == count_)
	{
		renderers_[0]->Off();
		count_ = 1;
	}


	if (1 == count_ && 0.1f <= revealTime_)
	{
		renderers_[1]->Off();
		count_ = 2;
	}

	if (2 == count_ && 0.2f <= revealTime_)
	{
		renderers_[2]->Off();
		count_ = 3;
	}

	if (3 == count_ && 0.3f <= revealTime_)
	{
		renderers_[3]->Off();
		count_ = 4;
	}

	if (4 == count_ && 0.4f <= revealTime_)
	{
		renderers_[4]->Off();
		count_ = 5;
	}
}
