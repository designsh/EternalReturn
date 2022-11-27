#pragma once
#include <functional>
#include <map>

// ���� :
class GameEngineFSM
{
private:
	class State 
	{
	public:
		std::string Name_;
		// �� ���°� ���۵ƴ�
		std::function<void()> Start_;
		// �� ���°� �������̴�.
		std::function<void(float)> Update_;
		// �� ���°� ������.
		std::function<void()> End_;

		float Time_;

		State(
			std::string _Name,
			std::function<void()> _Start,
			std::function<void(float)> _Update,
			std::function<void()> _End
		) 
			: Name_(_Name)
			, Start_(_Start)
			, Update_(_Update)
			, End_(_End)
			, Time_(0.0f)
		{

		}
	};


public:
	// constrcuter destructer
	GameEngineFSM();
	~GameEngineFSM();

	// delete Function
	GameEngineFSM(const GameEngineFSM& _Other) = delete;
	GameEngineFSM(GameEngineFSM&& _Other) noexcept = delete;
	GameEngineFSM& operator=(const GameEngineFSM& _Other) = delete;
	GameEngineFSM& operator=(GameEngineFSM&& _Other) noexcept = delete;

	void CreateState(const std::string& _Name, std::function<void(float)> _Update, std::function<void()> _Start = nullptr, std::function<void()> _EndStart = nullptr, std::function<void()> _Init = nullptr);
	void ChangeState(const std::string& _Name, bool _bForceChange = false);

	inline bool IsCurrentState(const std::string& _Name) const
	{
		return Current_->Name_ == _Name;
	}

	void Update(float _deltaTime);

	State* GetCurrentState() 
	{
		return Current_;
	}

	float GetTime() { return Current_->Time_; }

	std::string GetCurrentStateName();

	void operator<<(const std::string& _stateName);

protected:

private:
	std::map<std::string, State*> AllState_;

	State* Current_;
	State* Next_;

public:
	template <typename T>
	void CreateStateTemplate(
		const std::string& _Name,
		T* objptr,
		void (T::*_Start)(),
		void (T::*_Update)(float),
		void (T::*_End)()
		)
	{
		if (AllState_.end() != AllState_.find(_Name))
		{
			GameEngineDebug::MsgBoxError("�̹� �����ϴ� ������Ʈ�� �� ������� �߽��ϴ�.");
			return;
		}

		std::function<void()> Start = nullptr;
		std::function<void(float)> Update = nullptr;
		std::function<void()> End = nullptr;

		if (_Start != nullptr)
			Start = std::bind(_Start, objptr);

		if (_Update != nullptr)
			Update = std::bind(_Update, objptr, std::placeholders::_1);

		if (_End != nullptr)
			End = std::bind(_End, objptr);

		AllState_.insert(std::map<std::string, State*>::value_type(_Name, new State{ _Name,Start,Update,End }));
		//AllState_.insert(std::map<std::string, State*>::value_type(_Name, new State{ _Name, std::bind(_Start,objptr), std::bind(_Update ,objptr,std::placeholders::_1), std::bind(_End,objptr) }));
	}
};

