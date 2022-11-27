#include "PreCompile.h"
#include "ProhibitedArea.h"
#include "Character.h"
#include "CharStatPacket.h"
#include "PlayerUIController.h"
ProhibitedArea::ProhibitedArea() // default constructer 디폴트 생성자
	: locationName_(" "), location_(Location::NONE)
{

}

ProhibitedArea::~ProhibitedArea() // default destructer 디폴트 소멸자
{

}

ProhibitedArea::ProhibitedArea(ProhibitedArea&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}

void ProhibitedArea::Init(Location _location, const std::string& _fbxName, float4 _pos, const std::string& _name)
{
	location_ = _location;
	locationName_ = _name;
	renderer_ = CreateTransformComponent<GameEngineFBXRenderer>();
	renderer_->SetFBXMesh(_fbxName, "ColorProhibitedArea", false);
	renderer_->GetRenderSet(0).ShaderHelper->SettingConstantBufferSet("ResultColor", float4{ 1.0f, 0.0f, 0.0f, 0.1f });
	renderer_->GetTransform()->SetLocalPosition(_pos);
	renderer_->GetTransform()->SetLocalScaling(100.0f);

}


void ProhibitedArea::SetProhibited(bool _tf)
{
	isProhibited_ = _tf;

	if (true == _tf)
	{
		renderer_->On();
	}
	else
	{
		renderer_->Off();
	}

}

void ProhibitedArea::Start()
{
}

void ProhibitedArea::Update(float _DeltaTime)
{


}

