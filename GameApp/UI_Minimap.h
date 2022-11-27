#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>
// �뵵 : 
// ���� : 

class UI_CalculateHelper;
class GameEngineUIRenderer;
class UI_Minimap : public GameEngineActor
{
public:
	UI_Minimap(); // default constructer ����Ʈ ������
	~UI_Minimap(); // default destructer ����Ʈ �Ҹ���
	UI_Minimap(const UI_Minimap& _other) = delete; // default Copy constructer ����Ʈ ���������
	UI_Minimap(UI_Minimap&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������
	UI_Minimap& operator=(const UI_Minimap& _other) = delete; // default Copy operator ����Ʈ ���� ������
	UI_Minimap& operator=(const UI_Minimap&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

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

