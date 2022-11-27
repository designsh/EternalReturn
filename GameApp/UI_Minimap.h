#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>
// 용도 : 
// 설명 : 

class UI_CalculateHelper;
class GameEngineUIRenderer;
class UI_Minimap : public GameEngineActor
{
public:
	UI_Minimap(); // default constructer 디폴트 생성자
	~UI_Minimap(); // default destructer 디폴트 소멸자
	UI_Minimap(const UI_Minimap& _other) = delete; // default Copy constructer 디폴트 복사생성자
	UI_Minimap(UI_Minimap&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자
	UI_Minimap& operator=(const UI_Minimap& _other) = delete; // default Copy operator 디폴트 대입 연산자
	UI_Minimap& operator=(const UI_Minimap&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:
	void InitCharList();
	void InitAreaRenderList();

	void Toggle();

	void SetNextProhibitedArea(Location _location);
	void SetProhibitedArea(Location _location);
	
	void CheckCharPos();

	std::vector<GameEngineUIRenderer*> GetIconList() { return charIconList_; }

	void RemoveIcon(int _index);
protected:
	void Start() override;
	void Update(float _deltaTime) override;

	void startSleep();
	void updateSleep(float _deltaTime);

	void startAwake();
	void updateAwake(float _deltaTime);

private:
	std::vector<GameEngineUIRenderer*> charIconList_;
	std::vector<GameEngineUIRenderer*> areaRenderList_;
	GameEngineUIRenderer* minimapRenderer_;

private:
	float4 DEFAULT_MIN_XZ_WORLD = { -17452.0f, 0.0f, -17452.0f };
	float4 DEFAULT_MAX_XZ_WORLD = { 16970.0f, 0.0f, 21425.0f };
	// 34422,   38877
	// 44.6 : 1.0
	// 43.83 : 1.0
	// 386, 444
	// 89 : 87.6
	float4 DEFAULT_MIN_XY_UI = { 0.0f, 0.0f};
	float4 DEFAULT_MAX_XY_UI = { 772.0f, 887.0f };
	UI_CalculateHelper* calhelper_;
private:
	GameEngineFSM renderState_;
	bool bIsOn_;
};

