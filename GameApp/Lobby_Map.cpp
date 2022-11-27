#include "Precompile.h"
#include "Lobby_Map.h"
#include "GameEngine/GameEngineInput.h"
#include "GameEngine/GameEngineCollision.h"
#include "GameEngineBase/GameEngineDebug.h"
#include "PlayerInfoManager.h"
#include <GameEngine/GameEngineUIRenderer.h>


Lobby_Map::Lobby_Map()
{
}

Lobby_Map::~Lobby_Map()
{
}

LocationNode::LocationNode()
	: SelectedLocation(Location::NONE), nodeRenderer_(nullptr)
{

}

LocationNode::~LocationNode()
{
}

void LocationNode::Start()
{
}

void LocationNode::Update(float _DeltaTime)
{
}

void Lobby_Map::Start()
{
	{
		fullMapRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), static_cast<int>(UIRenderOrder::UI));
		fullMapRenderer_->SetImage("Map_Full.png", "PointSmp");
		fullMapRenderer_->GetTransform()->SetLocalPosition({ 420.0f, 90.0f});
		fullMapRenderer_->GetTransform()->SetLocalScaling(fullMapRenderer_->GetCurrentTexture()->GetTextureSize());
	}


	{
		if (selectAreaNodeList_.size() == 0)
		{
			for (int i = 0; i < 6; i++)
			{
				GameEngineUIRenderer* selectAreaRenderer0_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), static_cast<int>(UIRenderOrder::UIICON));
				selectAreaRenderer0_->SetImage("Map_Laboratory_Pin.png", "PointSmp");
				selectAreaRenderer0_->GetTransform()->SetLocalPosition({ 420.0f, 90.0f});
				selectAreaRenderer0_->GetTransform()->SetLocalScaling(selectAreaRenderer0_->GetCurrentTexture()->GetTextureSize());
				selectAreaRenderer0_->Off();

				LocationNode* node = GetLevel()->CreateActor<LocationNode>();
				node->SetRenderer(selectAreaRenderer0_);
				node->ChangeLocation(-1);

				selectAreaNodeList_.emplace_back(node);
			}
		}
	}

	{
		for (int i = 0; i < 15; i++)
		{
			NodeCapacityCounter* capacityCounter = GetLevel()->CreateActor<NodeCapacityCounter>();
			capacityCounter->Off();
			capacityCounterList_.emplace_back(capacityCounter);
		}

	}


	{
		areaChoiceMapRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), static_cast<int>(UIRenderOrder::UNSEEN));
		areaChoiceMapRenderer_->SetImage("Map_Resize_Color.png", "PointSmp");
		//�׻� �ڿ� ������ �Ⱥ��̰���
		areaChoiceMapRenderer_->GetTransform()->SetLocalPosition({ 420.0f, 90.0f});
		areaChoiceMapRenderer_->GetTransform()->SetLocalScaling(areaChoiceMapRenderer_->GetCurrentTexture()->GetTextureSize());
		//areaChoiceMapRenderer_->Off();
	}

	{
		textureCollision_ = CreateTransformComponent<GameEngineCollision>(GetTransform(), static_cast<int>(UIRenderOrder::UI));
		textureCollision_->GetTransform()->SetLocalPosition({ 420.0f, 90.0f });
		textureCollision_->GetTransform()->SetLocalScaling(fullMapRenderer_->GetCurrentTexture()->GetTextureSize());
		textureCollision_->SetCollisionInfo(static_cast<int>(eCollisionGroup::UI), CollisionType::Rect);
	}
}

void Lobby_Map::Update(float _DeltaTime)
{
}


int Lobby_Map::GetSelectLocation(float4 _Position)
{
	//�÷������� ���� �� ���콺�� ��ġ�� �޾ƿͼ� �� ��ǥ�� �÷����� �޾ƿ´��� ���� ���������� �Ǵ��ϴ� �Լ�

	//Gimp�� �̿�� RGB�� �ƴ϶� BGR�� �� ���� ��?
	//->���ʿ� ���ȼ��ϸ� BGRA�� ���ִ�. xyz = bgr

	float4 ImagePos = fullMapRenderer_->GetTransform()->GetWorldPosition();
	float4 ImagehalfSize = {-155.0f, 178.0f, 0.0f, 0.0f};

	//�̹��� ��������� ��ǥ�� ���Ѵ�
	float4 ImageLeftTop = ImagePos + ImagehalfSize;

	//���ҽ��� �Ǻ��� ����
	float4 returnPos = { (_Position.x - ImageLeftTop.x), (ImageLeftTop.y - _Position.y)};
	float4 Color = areaChoiceMapRenderer_->GetCurrentTexture()->GetPixel(returnPos.ix(), (returnPos.iy()));

	//Red�� = z
	//���繮��->�̹��� ũ�������� ���� �� �׵θ��� ���ذ���
	//�׵θ� ���� + ���� �׵θ��� ���󰪺��� ����

	if (Color.z < 0.1f && Color.y < 0.1f && Color.z < 0.1f)
	{
		//0, 0, 0 �ϰ��
		selectAreaNodeList_[PlayerInfoManager::GetInstance()->GetMyNumber()]->ChangeLocation(-1);
		PlayerInfoManager::GetInstance()->GetPlayerList()[PlayerInfoManager::GetInstance()->GetMyNumber()].startPoint_ = selectAreaNodeList_[PlayerInfoManager::GetInstance()->GetMyNumber()]->GetSelectedLocation();
		return selectAreaNodeList_[PlayerInfoManager::GetInstance()->GetMyNumber()]->GetSelectedLocation();
	}

	if (Color.z > 0.95f)
	{
		//255, *, * �ΰ��

		if (Color.y < 0.1f && Color.x < 0.1f)
		{
			//255, 0, 0
			selectAreaNodeList_[PlayerInfoManager::GetInstance()->GetMyNumber()]->ChangeLocation(static_cast<int>(Location::ALLEY));
			fullMapRenderer_->TextSetting("����", "���� ���� : ����", 20.0f, FW1_LEFT, { 0.85f,0.85f,0.85f,1.0f }, { 0.0f, -180.0f });
		}

		if (Color.y > 0.95f && Color.x < 0.1f)
		{
			//255, 255, 0
			selectAreaNodeList_[PlayerInfoManager::GetInstance()->GetMyNumber()]->ChangeLocation(static_cast<int>(Location::ARCHERY_RANGE));
			fullMapRenderer_->TextSetting("����", "���� ���� : �����", 20.0f, FW1_LEFT, { 0.85f,0.85f,0.85f,1.0f }, { 0.0f, -180.0f });
		}

		if (Color.y < 0.1f && Color.x > 0.95f)
		{
			//255, 0, 255
			selectAreaNodeList_[PlayerInfoManager::GetInstance()->GetMyNumber()]->ChangeLocation(static_cast<int>(Location::FACTORY));
			fullMapRenderer_->TextSetting("����", "���� ���� : ����", 20.0f, FW1_LEFT, { 0.85f,0.85f,0.85f,1.0f }, { 0.0f, -180.0f });
		}

		if (Color.y > 0.95f && Color.x > 0.95f)
		{
			//255, 255, 255
			selectAreaNodeList_[PlayerInfoManager::GetInstance()->GetMyNumber()]->ChangeLocation(static_cast<int>(Location::CEMETERY));
			fullMapRenderer_->TextSetting("����", "���� ���� : ����", 20.0f, FW1_LEFT, { 0.85f,0.85f,0.85f,1.0f }, { 0.0f, -180.0f });
		}

		if (Color.y < 0.6f && Color.y > 0.4f && Color.x < 0.1f)
		{
			//255, 128, 0
			//selectAreaNodeList_[PlayerInfoManager::GetInstance()->GetMyNumber()]->ChangeLocation(static_cast<int>(Location::RESERCH_CENTRE));
		}

		if (Color.y < 0.6f && Color.y > 0.4f && Color.x > 0.95f)
		{
			//255, 128, 255
			selectAreaNodeList_[PlayerInfoManager::GetInstance()->GetMyNumber()]->ChangeLocation(static_cast<int>(Location::POND));
			fullMapRenderer_->TextSetting("����", "���� ���� : ����", 20.0f, FW1_LEFT, { 0.85f,0.85f,0.85f,1.0f }, { 0.0f, -180.0f });
		}

		if (Color.y < 0.1f && Color.x < 0.6f && Color.x > 0.4f)
		{
			//255, 0, 128
			selectAreaNodeList_[PlayerInfoManager::GetInstance()->GetMyNumber()]->ChangeLocation(static_cast<int>(Location::TEMPLE));
			fullMapRenderer_->TextSetting("����", "���� ���� : ��", 20.0f, FW1_LEFT, { 0.85f,0.85f,0.85f,1.0f }, { 0.0f, -180.0f });
		}

		if (Color.y > 0.95f && Color.x < 0.6f && Color.x > 0.4f)
		{
			//255, 255, 128
			selectAreaNodeList_[PlayerInfoManager::GetInstance()->GetMyNumber()]->ChangeLocation(static_cast<int>(Location::UPTOWN));
			fullMapRenderer_->TextSetting("����", "���� ���� : ��� ���ð�", 20.0f, FW1_LEFT, { 0.85f,0.85f,0.85f,1.0f }, { 0.0f, -180.0f });
		}
	}
	else if (Color.z > 0.4f && Color.z < 0.6f)
	{
		// 128, *, *

		if (Color.y < 0.1f && Color.x < 0.1f)
		{
			//128, 0, 0
			selectAreaNodeList_[PlayerInfoManager::GetInstance()->GetMyNumber()]->ChangeLocation(static_cast<int>(Location::FOREST));
			fullMapRenderer_->TextSetting("����", "���� ���� : ��", 20.0f, FW1_LEFT, { 0.85f,0.85f,0.85f,1.0f }, { 0.0f, -180.0f });
		}

		if (Color.y > 0.95f && Color.x < 0.1f)
		{
			//128, 255, 0
			selectAreaNodeList_[PlayerInfoManager::GetInstance()->GetMyNumber()]->ChangeLocation(static_cast<int>(Location::DOCK));
			fullMapRenderer_->TextSetting("����", "���� ���� : �ױ�", 20.0f, FW1_LEFT, { 0.85f,0.85f,0.85f,1.0f }, { 0.0f, -180.0f });
		}

		if (Color.y > 0.95f && Color.x > 0.95f)
		{
			//128, 255, 255
			selectAreaNodeList_[PlayerInfoManager::GetInstance()->GetMyNumber()]->ChangeLocation(static_cast<int>(Location::HOSPITAL));
			fullMapRenderer_->TextSetting("����", "���� ���� : ����", 20.0f, FW1_LEFT, { 0.85f,0.85f,0.85f,1.0f }, { 0.0f, -180.0f });
		}
	}
	else if (Color.z < 0.05f)
	{
		//0, *, *

		if (Color.y > 0.95f && Color.x < 0.1f)
		{
			//0, 255, 0
			//��ȸ
			selectAreaNodeList_[PlayerInfoManager::GetInstance()->GetMyNumber()]->ChangeLocation(static_cast<int>(Location::CHAPEL));
			fullMapRenderer_->TextSetting("����", "���� ���� : ����", 20.0f, FW1_LEFT, { 0.85f,0.85f,0.85f,1.0f }, { 0.0f, -180.0f });
		}

		if (Color.y > 0.95f && Color.x > 0.95f)
		{
			//0, 255, 255
			//�ٿ�Ÿ��
			selectAreaNodeList_[PlayerInfoManager::GetInstance()->GetMyNumber()]->ChangeLocation(static_cast<int>(Location::AVENUE));
			fullMapRenderer_->TextSetting("����", "���� ���� : ��ȭ��", 20.0f, FW1_LEFT, { 0.85f,0.85f,0.85f,1.0f }, { 0.0f, -180.0f });
		}

		if (Color.y < 0.05f && Color.x > 0.95f)
		{
			//0, 0, 255
			selectAreaNodeList_[PlayerInfoManager::GetInstance()->GetMyNumber()]->ChangeLocation(static_cast<int>(Location::SCHOOL));
			fullMapRenderer_->TextSetting("����", "���� ���� : �б�", 20.0f, FW1_LEFT, { 0.85f,0.85f,0.85f,1.0f }, { 0.0f, -180.0f });
		}


		if (Color.y < 0.6f && Color.y > 0.4f && Color.x < 0.1f)
		{
			//0, 128, 0
			selectAreaNodeList_[PlayerInfoManager::GetInstance()->GetMyNumber()]->ChangeLocation(static_cast<int>(Location::HOTEL));
			fullMapRenderer_->TextSetting("����", "���� ���� : ȣ��", 20.0f, FW1_LEFT, { 0.85f,0.85f,0.85f,1.0f }, { 0.0f, -180.0f });
		}

		if (Color.y < 0.6f && Color.y > 0.4f && Color.x > 0.95f)
		{
			//0, 128, 255
			selectAreaNodeList_[PlayerInfoManager::GetInstance()->GetMyNumber()]->ChangeLocation(static_cast<int>(Location::BEACH));
			fullMapRenderer_->TextSetting("����", "���� ���� : �𷡻���", 20.0f, FW1_LEFT, { 0.85f,0.85f,0.85f,1.0f }, { 0.0f, -180.0f });
		}

		if (Color.y < 0.6f && Color.y > 0.4f && Color.x < 0.6f && Color.x > 0.4f)
		{
			//0, 128, 128
			selectAreaNodeList_[PlayerInfoManager::GetInstance()->GetMyNumber()]->ChangeLocation(static_cast<int>(Location::SCHOOL));
			fullMapRenderer_->TextSetting("����", "���� ���� : �б�", 20.0f, FW1_LEFT, { 0.85f,0.85f,0.85f,1.0f }, { 0.0f, -180.0f });
		}
	}


	return selectAreaNodeList_[PlayerInfoManager::GetInstance()->GetMyNumber()]->GetSelectedLocation();
}

void Lobby_Map::ResizeMyPin()
{
	GameEngineImageRenderer* renderer = selectAreaNodeList_[PlayerInfoManager::GetInstance()->GetMyNumber()]->GetRenderer();
	renderer->GetTransform()->SetLocalPosition({420.0f, 90.0f});
}

void Lobby_Map::ArrangeCounter()
{
	for (int i = 0; i < capacityCounterList_.size(); i++)
	{
		capacityCounterList_[i]->On();
		switch (static_cast<Location>(i))
		{
		case Location::NONE:
			break;
		case Location::DOCK:
			capacityCounterList_[i]->GetRenderer()->GetTransform()->SetLocalPosition({ 424.0f, -45.0f });
			break;
		case Location::POND:
			capacityCounterList_[i]->GetRenderer()->GetTransform()->SetLocalPosition({ 486.0f, 109.0f });
			break;
		case Location::BEACH:
			capacityCounterList_[i]->GetRenderer()->GetTransform()->SetLocalPosition({ 313.0f, 44.0f });
			break;
		case Location::UPTOWN:
			capacityCounterList_[i]->GetRenderer()->GetTransform()->SetLocalPosition({ 362.0f, 0.0f });
			break;
		case Location::ALLEY:
			capacityCounterList_[i]->GetRenderer()->GetTransform()->SetLocalPosition({ 406.0f, 225.0f });
			break;
		case Location::HOTEL:
			capacityCounterList_[i]->GetRenderer()->GetTransform()->SetLocalPosition({ 317.0f, 106.0f });
			break;
		case Location::AVENUE:
			capacityCounterList_[i]->GetRenderer()->GetTransform()->SetLocalPosition({ 439.0f, 169.0f });
			break;
		case Location::HOSPITAL:
			capacityCounterList_[i]->GetRenderer()->GetTransform()->SetLocalPosition({ 532.0f, 74.0f });
			break;
		case Location::TEMPLE:
			capacityCounterList_[i]->GetRenderer()->GetTransform()->SetLocalPosition({ 523.0f, 175.0f });
			break;
		case Location::ARCHERY_RANGE:
			// 347 180
			capacityCounterList_[i]->GetRenderer()->GetTransform()->SetLocalPosition({ 347.0f, 180.0f });
			break;
		case Location::CEMETERY:
			capacityCounterList_[i]->GetRenderer()->GetTransform()->SetLocalPosition({ 465.0f, 55.0f });
			break;
		case Location::FOREST:
			capacityCounterList_[i]->GetRenderer()->GetTransform()->SetLocalPosition({ 380.0f, 68.0f });
			break;
		case Location::FACTORY:
			capacityCounterList_[i]->GetRenderer()->GetTransform()->SetLocalPosition({ 508.0f, -12.0f });
			break;
		case Location::CHAPEL:
			capacityCounterList_[i]->GetRenderer()->GetTransform()->SetLocalPosition({ 443.0f, 7.0f });
			break;
		case Location::SCHOOL:
			capacityCounterList_[i]->GetRenderer()->GetTransform()->SetLocalPosition({ 375.0f, 141.0f });
			break;
		case Location::RESERCH_CENTRE:
			break;
		case Location::ESCAPE_DOCK:
			break;
		case Location::MAX:
			break;
		default:
			break;
		}


	}

}

NodeCapacityCounter::NodeCapacityCounter()
	: count_("0")
{
}

NodeCapacityCounter::~NodeCapacityCounter()
{
}


void NodeCapacityCounter::SetCounter(int _count)
{
	count_ = std::to_string(_count);
}

void NodeCapacityCounter::Start()
{
	counterRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), static_cast<int>(UIRenderOrder::FONT));
	counterRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 0.0f, 0.0f });
}

void NodeCapacityCounter::Update(float _DeltaTime)
{

	//if ("0" == count_ || "1" == count_)
	//{
	//	counterRenderer_->Off();
	//}
	//else
	{
		counterRenderer_->TextSetting("����", count_, 20, FW1_CENTER);
	}

}
