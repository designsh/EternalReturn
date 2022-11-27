#include "Precompile.h"
#include "UI_HyperMap.h"
#include "GameEngine/GameEngineInput.h"
#include "GameEngine/GameEngineCollision.h"
#include "GameEngineBase/GameEngineDebug.h"

UI_HyperMap::UI_HyperMap()
	: SelectedLocation(Location::NONE), MapOnFlag(false)
{
}

UI_HyperMap::~UI_HyperMap()
{
}

void UI_HyperMap::Start()
{
	MapPos = { 0.f, 0.f, -1.f };
		
	{
		fullMapRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::UIPANEL1);
		fullMapRenderer_->SetImage("Map_Full.png", "PointSmp");
		fullMapRenderer_->GetTransform()->SetLocalPosition(MapPos);
		fullMapRenderer_->GetTransform()->SetLocalScaling(fullMapRenderer_->GetCurrentTexture()->GetTextureSize());
	}

	{
		selectAreaRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::UIICON);
		selectAreaRenderer_->SetImage("Map_Empty.png", "PointSmp");
		selectAreaRenderer_->GetTransform()->SetLocalPosition(MapPos + float4{0.f,0.f,-1.f,0.f});
		selectAreaRenderer_->GetTransform()->SetLocalScaling(selectAreaRenderer_->GetCurrentTexture()->GetTextureSize());
	}


	{
		//무조건 덮어씌워지고 안보여야함
		areaChoiceMapRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), -1);
		areaChoiceMapRenderer_->SetImage("Map_Resize_Color.png", "PointSmp");
		areaChoiceMapRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 0.0f, -101.0f });
		areaChoiceMapRenderer_->GetTransform()->SetLocalScaling(areaChoiceMapRenderer_->GetCurrentTexture()->GetTextureSize());
	}

	{
		textureCollision_ = CreateTransformComponent<GameEngineCollision>();
		textureCollision_->GetTransform()->SetLocalPosition(MapPos);
		textureCollision_->GetTransform()->SetLocalScaling(fullMapRenderer_->GetCurrentTexture()->GetTextureSize());
		textureCollision_->SetCollisionInfo(static_cast<int>(eCollisionGroup::UI), CollisionType::Rect);
	}
}

void UI_HyperMap::Update(float _DeltaTime)
{
	if (MapOnFlag == false)
	{
		//맵UI가 커져있지 않으면 업데이트를 하지 않음
		return;
	}

	if (textureCollision_->Collision(eCollisionGroup::MousePointer))
	{
		GetSelectLocation(GameEngineInput::GetInst().GetMouse3DPos());

		if (false == GameEngineInput::GetInst().Down("LButton"))
		{
			SelectedLocation = Location::NONE;
		}
	}



}


Location UI_HyperMap::GetSelectLocation(float4 _Position)
{


	float4 ImagePos = fullMapRenderer_->GetTransform()->GetWorldPosition();
	float4 ImagehalfSize = { -155.0f, 178.0f, 0.0f, 0.0f };

	float4 ImageLeftTop = ImagePos + ImagehalfSize;


	float4 returnPos = { (_Position.x - ImageLeftTop.x), (ImageLeftTop.y - _Position.y) };
	float4 Color = areaChoiceMapRenderer_->GetCurrentTexture()->GetPixel(returnPos.ix(), (returnPos.iy()));


	SelectedAreaRender(_Position);

	if (Color.z < 0.1f && Color.y < 0.1f && Color.z < 0.1f)
	{
		SelectedLocation = Location::NONE;
		return SelectedLocation;
	}

	if (Color.z > 0.95f)
	{

		if (Color.y < 0.1f && Color.x < 0.1f)
		{
			//255, 0, 0
			SelectedLocation = Location::ALLEY;
		}

		if (Color.y > 0.95f && Color.x < 0.1f)
		{
			//255, 255, 0
			SelectedLocation = Location::ARCHERY_RANGE;
		}

		if (Color.y < 0.1f && Color.x > 0.95f)
		{
			//255, 0, 255
			SelectedLocation = Location::FACTORY;
		}

		if (Color.y > 0.95f && Color.x > 0.95f)
		{
			//255, 255, 255
			SelectedLocation = Location::CEMETERY;
		}

		if (Color.y < 0.6f && Color.y > 0.4f && Color.x < 0.1f)
		{
			//255, 128, 0
			SelectedLocation = Location::RESERCH_CENTRE;
		}

		if (Color.y < 0.6f && Color.y > 0.4f && Color.x > 0.95f)
		{
			//255, 128, 255
			SelectedLocation = Location::POND;
		}

		if (Color.y < 0.1f && Color.x < 0.6f && Color.x > 0.4f)
		{
			//255, 0, 128
			SelectedLocation = Location::TEMPLE;
		}

		if (Color.y > 0.95f && Color.x < 0.6f && Color.x > 0.4f)
		{
			//255, 255, 128
			SelectedLocation = Location::UPTOWN;
		}
	}
	else if (Color.z > 0.4f && Color.z < 0.6f)
	{
		// 128, *, *

		if (Color.y < 0.1f && Color.x < 0.1f)
		{
			//128, 0, 0
			SelectedLocation = Location::FOREST;
		}

		if (Color.y > 0.95f && Color.x < 0.1f)
		{
			//128, 255, 0
			SelectedLocation = Location::DOCK;
		}

		if (Color.y > 0.95f && Color.x > 0.95f)
		{
			//128, 255, 255
			SelectedLocation = Location::HOSPITAL;
		}
	}
	else if (Color.z < 0.05f)
	{
		//0, *, *

		if (Color.y > 0.95f && Color.x < 0.1f)
		{
			//0, 255, 0
			SelectedLocation = Location::CHAPEL;
		}

		if (Color.y > 0.95f && Color.x > 0.95f)
		{
			//0, 255, 255
			SelectedLocation = Location::AVENUE;
		}

		if (Color.y < 0.05f && Color.x > 0.95f)
		{
			//0, 0, 255
			SelectedLocation = Location::SCHOOL;
		}


		if (Color.y < 0.6f && Color.y > 0.4f && Color.x < 0.1f)
		{
			//0, 128, 0
			SelectedLocation = Location::HOTEL;
		}

		if (Color.y < 0.6f && Color.y > 0.4f && Color.x > 0.95f)
		{
			//0, 128, 255
			SelectedLocation = Location::BEACH;
		}

		if (Color.y < 0.6f && Color.y > 0.4f && Color.x < 0.6f && Color.x > 0.4f)
		{
			//0, 128, 128
			SelectedLocation = Location::SCHOOL;
		}
	}
	else
	{
		SelectedLocation = Location::NONE;
	}


	return SelectedLocation;
}

Location UI_HyperMap::ReturnSelectedLocation()
{
	//if (SelectedLocation == Location::NONE)
	//{
	//	//하이퍼루프가 먼저 업데이트 되서 맵에 정보가 없으면
	//	GetSelectLocation(GameEngineInput::GetInst().GetMouse3DPos());
	//}

	return SelectedLocation;
}

void UI_HyperMap::MapOff()
{
	fullMapRenderer_->Off();
	areaChoiceMapRenderer_->Off();
	selectAreaRenderer_->Off();

	textureCollision_->Off();
	SelectedLocation = Location::NONE;
	MapOnFlag = false;
}

void UI_HyperMap::MapOn()
{
	fullMapRenderer_->On();
	areaChoiceMapRenderer_->On();
	selectAreaRenderer_->On();

	textureCollision_->On();
	//SelectedLocation = Location::NONE;
	MapOnFlag = true;
}

void UI_HyperMap::SelectedAreaRender(float4 _Position)
{
	float4 ImagePos = fullMapRenderer_->GetTransform()->GetWorldPosition();
	float4 ImagehalfSize = { -155.0f, 178.0f, 0.0f, 0.0f };

	float4 ImageLeftTop = ImagePos + ImagehalfSize;


	float4 returnPos = { (_Position.x - ImageLeftTop.x), (ImageLeftTop.y - _Position.y) };
	float4 Color = areaChoiceMapRenderer_->GetCurrentTexture()->GetPixel(returnPos.ix(), (returnPos.iy()));

	{
		if (Color.z < 0.1f && Color.y < 0.1f && Color.z < 0.1f)
		{
			selectAreaRenderer_->SetImage("Map_Empty.png", "PointSmp");
		}

		if (Color.z > 0.95f)
		{

			if (Color.y < 0.1f && Color.x < 0.1f)
			{
				//255, 0, 0
				selectAreaRenderer_->SetImage("Map_Alley_Pin.png", "PointSmp");
			}

			if (Color.y > 0.95f && Color.x < 0.1f)
			{
				//255, 255, 0
				selectAreaRenderer_->SetImage("Map_Archery_Pin.png", "PointSmp");
			}

			if (Color.y < 0.1f && Color.x > 0.95f)
			{
				//255, 0, 255
				selectAreaRenderer_->SetImage("Map_Factory_Pin.png", "PointSmp");
			}

			if (Color.y > 0.95f && Color.x > 0.95f)
			{
				//255, 255, 255
				selectAreaRenderer_->SetImage("Map_Cemetery_Pin.png", "PointSmp");
			}

			if (Color.y < 0.6f && Color.y > 0.4f && Color.x < 0.1f)
			{
				//255, 128, 0
				selectAreaRenderer_->SetImage("Map_Laboratory_Pin.png", "PointSmp");
			}

			if (Color.y < 0.6f && Color.y > 0.4f && Color.x > 0.95f)
			{
				//255, 128, 255
				selectAreaRenderer_->SetImage("Map_Pond_Pin.png", "PointSmp");
			}

			if (Color.y < 0.1f && Color.x < 0.6f && Color.x > 0.4f)
			{
				//255, 0, 128
				selectAreaRenderer_->SetImage("Map_Temple_Pin.png", "PointSmp");
			}

			if (Color.y > 0.95f && Color.x < 0.6f && Color.x > 0.4f)
			{
				//255, 255, 128
				selectAreaRenderer_->SetImage("Map_UpTown_Pin.png", "PointSmp");
			}
		}
		else if (Color.z > 0.4f && Color.z < 0.6f)
		{
			// 128, *, *

			if (Color.y < 0.1f && Color.x < 0.1f)
			{
				//128, 0, 0
				selectAreaRenderer_->SetImage("Map_Forest_Pin.png", "PointSmp");
			}

			if (Color.y > 0.95f && Color.x < 0.1f)
			{
				//128, 255, 0
				selectAreaRenderer_->SetImage("Map_Harbor_Pin.png", "PointSmp");
			}

			if (Color.y > 0.95f && Color.x > 0.95f)
			{
				//128, 255, 255
				selectAreaRenderer_->SetImage("Map_Hospital_Pin.png", "PointSmp");
			}
		}
		else if (Color.z < 0.05f)
		{
			//0, *, *

			if (Color.y > 0.95f && Color.x < 0.1f)
			{
				//0, 255, 0
				selectAreaRenderer_->SetImage("Map_Church_Pin.png", "PointSmp");
			}

			if (Color.y > 0.95f && Color.x > 0.95f)
			{
				//0, 255, 255
				selectAreaRenderer_->SetImage("Map_DownTown_Pin.png", "PointSmp");
			}

			if (Color.y < 0.05f && Color.x > 0.95f)
			{
				//0, 0, 255
				selectAreaRenderer_->SetImage("Map_School_Pin.png", "PointSmp");
			}


			if (Color.y < 0.6f && Color.y > 0.4f && Color.x < 0.1f)
			{
				//0, 128, 0
				selectAreaRenderer_->SetImage("Map_Hotel_Pin.png", "PointSmp");
			}

			if (Color.y < 0.6f && Color.y > 0.4f && Color.x > 0.95f)
			{
				//0, 128, 255
				selectAreaRenderer_->SetImage("Map_SandyBeach_Pin.png", "PointSmp");
			}

			if (Color.y < 0.6f && Color.y > 0.4f && Color.x < 0.6f && Color.x > 0.4f)
			{
				//0, 128, 128
				selectAreaRenderer_->SetImage("Map_School_Pin.png", "PointSmp");
			}
		}
		else
		{
			selectAreaRenderer_->SetImage("Map_Empty.png", "PointSmp");
		}
	}
}
