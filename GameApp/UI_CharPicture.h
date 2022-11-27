#pragma once
#include <GameEngine/GameEngineActor.h>

using namespace std;
// Ό³Έν :
class GameEngineUIRenderer;
class Player;
class UI_CharPicture : public GameEngineActor
{
public:
	// constrcuter destructer
	UI_CharPicture();
	~UI_CharPicture();

	// delete Function
	UI_CharPicture(const UI_CharPicture& _Other) = delete;
	UI_CharPicture(UI_CharPicture&& _Other) noexcept = delete;
	UI_CharPicture& operator=(const UI_CharPicture& _Other) = delete;
	UI_CharPicture& operator=(UI_CharPicture&& _Other) noexcept = delete;

	bool UIOn;

public:
		
	void SetJobType(JobType _myjob);

protected:
	GameEngineUIRenderer* BackGroundRenderer;
	GameEngineUIRenderer* CharRenderer;
	float Time;


private:
	void Start() override;
	void Update(float _Time) override;
	
};

