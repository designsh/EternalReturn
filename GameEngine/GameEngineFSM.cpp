#include "PreCompile.h"
#include "GameEngineFSM.h"
#include <GameEngineBase\GameEngineDebug.h>

GameEngineFSM::GameEngineFSM()
	: Current_(nullptr)
	, Next_(nullptr)
{
}

GameEngineFSM::~GameEngineFSM()
{
	for (auto& State : AllState_)
	{
		delete State.second;
		State.second = nullptr;
	}
}

void GameEngineFSM::Update(float _deltaTime)
{
	if (nullptr != Next_)
	{
		if (nullptr != Current_ && nullptr != Current_->End_)
		{
			Current_->End_();
		}
		Current_ = Next_;
		Current_->Time_ = 0.0f;

		if (nullptr != Current_->Start_)
		{
			Current_->Start_();
		}
		Next_ = nullptr;
	}

	if (nullptr == Current_)
	{
		GameEngineDebug::MsgBoxError("���°� �������� �ʴµ� ���¸ӽ��� �۵���Ű���� �߽��ϴ�.");
		return;
	}

	Current_->Time_ += _deltaTime;
	Current_->Update_(_deltaTime);
}

std::string GameEngineFSM::GetCurrentStateName()
{
	if (Current_ == nullptr)
	{
		return "";
	}

	return Current_->Name_;
}

void GameEngineFSM::operator<<(const std::string& _stateName)
{
	std::map<std::string, State*>::iterator FindIter = AllState_.find(_stateName);

	if (AllState_.end() == FindIter)
	{
		GameEngineDebug::MsgBoxError("�������� �ʴ� ������Ʈ�� ü�����Ϸ��� �߽��ϴ�..");
		return;
	}

	Next_ = FindIter->second;
}

void GameEngineFSM::CreateState(
	const std::string& _Name,
	std::function<void(float)> _Update,
	std::function<void()> _Start /*= nullptr*/,
	std::function<void()> _EndStart /*= nullptr*/,
	std::function<void()> _Init /*= nullptr*/
)
{
	if (AllState_.end() != AllState_.find(_Name))
	{
		GameEngineDebug::MsgBoxError("�̹� �����ϴ� ������Ʈ�� �� ������� �߽��ϴ�.");
		return;
	}

	if (nullptr != _Init)
	{
		_Init();
	}

	AllState_.insert(std::map<std::string, State*>::value_type(_Name, new State(_Name, _Start, _Update, _EndStart)));
}

void GameEngineFSM::ChangeState(const std::string& _Name, bool _bForceChange)
{
	if (nullptr != Current_)
	{
		if (Current_->Name_ == _Name && _bForceChange == false)
		{
			return;
		}
	}

	std::map<std::string, State*>::iterator FindIter = AllState_.find(_Name);

	if (AllState_.end() == FindIter)
	{
		GameEngineDebug::MsgBoxError("�������� �ʴ� ������Ʈ�� ü�����Ϸ��� �߽��ϴ�..");
		return;
	}

	Next_ = FindIter->second;
}