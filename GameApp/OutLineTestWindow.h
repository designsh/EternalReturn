#pragma once
#include <GameEngine/GameEngineGUI.h>

// 분류 : IMGUI Window
// 용도 : 테스트용
// 설명 : 메쉬 생성 및 아웃라인 생성 기능, 아웃라인 두께조절 등 기능을 제공
class OutLineTestWindow : public GameEngineGUIWindow
{
public:
protected:
	void OnGUI() override;

private:

public:
	OutLineTestWindow();
	~OutLineTestWindow();

protected:
	OutLineTestWindow(const OutLineTestWindow& _other) = delete;
	OutLineTestWindow(OutLineTestWindow&& _other) noexcept = delete;

private:
	OutLineTestWindow& operator=(const OutLineTestWindow& _other) = delete;
	OutLineTestWindow& operator=(const OutLineTestWindow&& _other) = delete;

public:
protected:
private:
};

