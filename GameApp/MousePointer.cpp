#include "PreCompile.h"
#include "MousePointer.h"

#include <GameEngine/GameEngineUIRenderer.h>
#include <GameEngine/GameEngineCollision.h>

#include "Enums.h"
#include "Character.h"
#include "Monsters.h"

MousePointer* MousePointer::InGameMouse = nullptr;

float4 MousePointer::GetIntersectionYAxisPlane(float _height, float _rayLength)
{
	return DirectX::XMPlaneIntersectLine({ 0.0f, 1.0f, 0.0f, -_height }, rayOrigin_.DirectVector, (rayOrigin_ + rayDirection_ * _rayLength).DirectVector);;
}

GameEngineCollision* MousePointer::GetPickCollision(int _Order)
{
	// 현재 레벨의 모든 충돌체 목록 Get
	std::map<int, std::list<GameEngineCollision*>> AllList = GetLevel()->GetAllCollision();

	// 해당 광선과 교차하는 충돌체 탐색
	std::list<GameEngineCollision*>::iterator StartIter = AllList[_Order].begin();
	std::list<GameEngineCollision*>::iterator EndIter = AllList[_Order].end();

	for (; StartIter != EndIter; ++StartIter)
	{
		// 광선의 시작지점에서부터 선택된 충돌체와 교차하는 지점까지의 거리
		float Dist = 0.0f;
		if (true == (*StartIter)->BoundingToRayCollision((*StartIter)->GetCollisionType(), rayOrigin_, rayDirection_, Dist))
		{
			return (*StartIter);
		}
	}

	return nullptr;
}

void MousePointer::GrabItem(ItemBase* _item, GameEngineUIRenderer* _icon)
{
	item_ = _item;

	itemIconRenderer_->SetImage(_icon->GetName());
	itemIconRenderer_->GetTransform()->SetLocalScaling(_icon->GetTransform()->GetLocalScaling());
	itemIconRenderer_->On();

	isGrabbed_ = true;
}

ItemBase* MousePointer::PutItem()
{
	itemIconRenderer_->Off();
	isGrabbed_ = false;

	return item_;
}

void MousePointer::updateMouseRay()
{
	// 뷰포트 개수를 가져옵니다. 뷰포트는 하나만 사용했고, 하나만 가져올 것이기 때문에 1 로 초기화합니다.
	UINT numViewport = 1;
	D3D11_VIEWPORT viewports = { 0, };

	// 입력된 첫 번째 인자 수 만큼 뷰포트를 가져옵니다.
	// 만약 2번째 인자가 NULL 이면, 첫번째 인자로 넣은 UINT에 값이 채워집니다.
	// 채워진 값은 현재 래스터라이저에 바인딩 된 뷰포트 수 입니다.
	//dc->RSGetViewports(&numViewport, nullptr);
	GameEngineDevice::GetContext()->RSGetViewports(&numViewport, &viewports);

	if (viewports.Width == 0.0f || viewports.Height == 0.0f)
	{
		return;
	}

	// 인풋 클래스에서 마우스를 위치를 가져옵니다.
	float4 mousePosition = GameEngineInput::GetInst().GetMousePos();
	float pointX = mousePosition.x;
	float pointY = mousePosition.y;

	// 마우스의 위치는 뷰포트를 벗어날 수 없습니다.
	if (pointX <= 0.0f)
	{
		pointX = 0.0f;
	}
	else if (pointX >= viewports.Width)
	{
		pointX = viewports.Width;
	}

	if (pointY <= 0.0f)
	{
		pointY = 0.0f;
	}
	else if (pointY >= viewports.Height)
	{
		pointY = viewports.Height;
	}

	// 마우스 커서 좌표를 -1에서 +1 범위로 바꿉니다. (NDC)
	pointX = ((2.0f * pointX) / viewports.Width) - 1.0f;
	pointY = (((2.0f * pointY) / viewports.Height) - 1.0f) * -1.0f;

	// 뷰포트의 종횡비를 고려하여 투영 행렬을 사용해 점을 조정합니다
	float4x4 matProjection = GetLevel()->GetMainCameraActor()->GetProjectionMatrix();

	pointX = pointX / matProjection._11;
	pointY = pointY / matProjection._22;

	// 뷰 행렬의 역행렬을 구합니다.
	float4x4 inverseViewMatrix = GetLevel()->GetMainCameraActor()->GetViewMatrix().InverseReturn();

	// 뷰 역행렬을 곱해 방향을 알아냅니다.
	rayDirection_.x = pointX;
	rayDirection_.y = pointY;
	rayDirection_.z = 1.0f;
	rayDirection_.w = 0.0f;

	rayDirection_ = rayDirection_ * inverseViewMatrix;
	rayDirection_.Normalize3D();

	// 뷰 역행렬의 4행은 곧 카메라의 위치입니다.
	rayOrigin_ = inverseViewMatrix.vw;
}

void MousePointer::CheckCollision()
{
	// 캐릭터와의 충돌체크
	CheckCharacterCollision();

	// 몬스터와의 충돌체크
	CheckMonsterCollision();
}

void MousePointer::CheckCharacterCollision()
{
	// Check Player Group Collision Check
	GameEngineCollision* CurCollisionCharacter = rayCollision_->GetCollision(static_cast<int>(eCollisionGroup::Player));
	if (nullptr != CurCollisionCharacter)
	{
		// 현재 충돌한 플레이어와 이전에 충돌한 플레이어를 비교하여 동일하다면 처리없음
		// 다르다면 이전충돌한 플레이어는 외곽선해제, 현재 충돌한 플레이어는 외곽선활성화 및 이전충돌플레이어에 저장

		// Current Collider Get Actor
		GameEngineActor* Actor = CurCollisionCharacter->GetActor();

		// Convert Character
		Character* CurCharacter = dynamic_cast<Character*>(Actor);

		// Check Prev Character & Current Character
		if (nullptr != PrevColCharacter_)										// 이전에 충돌한 플레이어가 존재할때
		{
			if (PrevColCharacter_->GetIndex() == CurCharacter->GetIndex())
			{
				// 동일한 플레이어이므로 외곽선 활성
				CurCharacter->PickingOutLineOn();
			}
			else
			{
				// 이전 충돌한 플레이어 외곽선해제
				PrevColCharacter_->PickingOutLineOff();

				// 현재 충돌한 플레이어 외곽선활성 및 저장
				CurCharacter->PickingOutLineOn();
				PrevColCharacter_ = CurCharacter;
			}
		}
		else																	// 이전에 충돌한 플레이거가 존재하지않을때
		{
			CurCharacter->PickingOutLineOn();
			PrevColCharacter_ = CurCharacter;
		}
	}
	else // 현재 충돌한 플레이어가 존재하지않으므로 이전 충돌한 플레이어 외곽선해제
	{
		if (nullptr != PrevColCharacter_)
		{
			PrevColCharacter_->PickingOutLineOff();
		}
	}
}

void MousePointer::CheckMonsterCollision()
{
	// Check Monster Group Collision Check
	GameEngineCollision* CurCollisionMonster = rayCollision_->GetCollision(static_cast<int>(eCollisionGroup::Monster));
	if (nullptr != CurCollisionMonster)
	{
		// 현재 충돌한 몬스터와 이전에 충돌한 몬스터를 비교하여 동일하다면 처리없음
		// 다르다면 이전충돌한 몬스터는 외곽선해제, 현재 충돌한 몬스터는 외곽선활성화 및 이전충돌몬스터에 저장

		// Current Collider Get Actor
		GameEngineActor* Actor = CurCollisionMonster->GetActor();

		// Convert Monster
		Monsters* CurMonster = dynamic_cast<Monsters*>(Actor);

		// Check Prev Character & Current Character
		if (nullptr != PrevColMonster_)										// 이전에 충돌한 몬스터가 존재할때
		{
			if (PrevColMonster_->GetIndex() == CurMonster->GetIndex())
			{
				// 동일한 몬스터이므로 외곽선 활성
				CurMonster->PickingOutLineOn();
			}
			else
			{
				// 이전 충돌한 몬스터 외곽선해제
				PrevColMonster_->PickingOutLineOff();

				// 현재 충돌한 몬스터 외곽선활성 및 저장
				CurMonster->PickingOutLineOn();
				PrevColMonster_ = CurMonster;
			}
		}
		else																	// 이전에 충돌한 몬스터가 존재하지않을때
		{
			CurMonster->PickingOutLineOn();
			PrevColMonster_ = CurMonster;
		}
	}
	else // 현재 충돌한 몬스터가 존재하지않으므로 이전 충돌한 플레이어 외곽선해제
	{
		if (nullptr != PrevColMonster_)
		{
			PrevColMonster_->PickingOutLineOff();
		}
	}
}

void MousePointer::Start()
{
	// 광선 충돌체 생성
	rayCollision_ = CreateTransformComponent<GameEngineCollision>();
	rayCollision_->SetCollisionType(CollisionType::Ray);
	rayCollision_->SetCollisionGroup(eCollisionGroup::MouseRay);

	// 마우스 렌더러 생성
	MouseRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(static_cast<int>(UIRenderOrder::MOUSE));
	MouseRenderer_->SetImage(GameEngineString::toupper("Cursor_01.png"));
	MouseRenderer_->GetTransform()->SetLocalPosition({ 12.0f, -10.0f, 0.0f });
	MouseRenderer_->GetTransform()->SetLocalScaling({ 30.0f, 30.0f, 30.0f });

	itemIconRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(static_cast<int>(UIRenderOrder::UIICON));
	itemIconRenderer_->SetImage(GameEngineString::toupper("Cursor_01.png"));
	itemIconRenderer_->Off();

	// 마우스 충돌체 생성
	MouseCollider_ = CreateTransformComponent<GameEngineCollision>(static_cast<int>(UIRenderOrder::MOUSE));
	MouseCollider_->GetTransform()->SetLocalScaling(float4{ 30.0f, 30.0f, 30.0f });
	MouseCollider_->SetCollisionInfo(static_cast<int>(eCollisionGroup::MousePointer), CollisionType::AABBBox3D);

#pragma region 키생성
	if (false == GameEngineInput::GetInst().IsKey("LButton"))
	{
		GameEngineInput::GetInst().CreateKey("LButton", VK_LBUTTON);
	}

	if (false == GameEngineInput::GetInst().IsKey("RButton"))
	{
		GameEngineInput::GetInst().CreateKey("RButton", VK_RBUTTON);
	}
#pragma endregion
}

void MousePointer::Update(float _deltaTime)
{
	// Update Ray
	updateMouseRay();
	rayCollision_->SetRayData(rayOrigin_, rayDirection_);

	// 마우스액터 위치 갱신 : 현재 마우스위치
	GetTransform()->SetWorldPosition(GameEngineInput::GetInst().GetMouse3DPos());

	// 충돌체크
	CheckCollision();

	// 키체크
	if (true == GameEngineInput::GetInst().Down("LBUTTON"))
	{

	}

	if (true == GameEngineInput::GetInst().Down("RBUTTON"))
	{

	}
}

void MousePointer::LevelChangeStartEvent(GameEngineLevel* _PrevLevel)
{
}

void MousePointer::LevelChangeEndEvent(GameEngineLevel* _NextLevel)
{
	// 엔딩레벨에서 사용안함
	if (std::string::npos != _NextLevel->GetName().find("Ending"))
	{
		return;
	}
	// 각종 테스트레벨에서 사용안함
	else if (std::string::npos != _NextLevel->GetName().find("MeshLoadLevel"))
	{
		return;
	}
	else if (std::string::npos != _NextLevel->GetName().find("OutLineTestLevel"))
	{
		return;
	}
	else if (std::string::npos != _NextLevel->GetName().find("TestLevel"))
	{
		return;
	}

	// 마우스 레벨이동
	GetLevel()->SetLevelActorMove(_NextLevel, this);
}

MousePointer::MousePointer()
	: rayDirection_(0.0f, 0.0f, 1.0f, 1.0f)
	, rayOrigin_(float4::ZERO)
	, rayCollision_(nullptr)
	, MouseRenderer_(nullptr)
	, MouseCollider_(nullptr)
	, PrevColCharacter_(nullptr)
	, PrevColMonster_(nullptr)
	, isGrabbed_(false)
	, item_(nullptr)
	, itemIconRenderer_(nullptr)
{
}

MousePointer::~MousePointer()
{
}
