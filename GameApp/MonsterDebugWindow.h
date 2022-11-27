#pragma once
#include <GameEngine/GameEngineGUI.h>

// 분류 : IMGUI Window
// 용도 : 디버그(확인용)
// 설명 : 몬스터 기본정보 체크를 위한 디버그 윈도우
class MonsterDebugWindow : public GameEngineGUIWindow
{
public:
	void AddText(const std::string& _DebugText);

protected:
	void OnGUI() override;

private:
public:
	MonsterDebugWindow();
	~MonsterDebugWindow();

protected:
	MonsterDebugWindow(const MonsterDebugWindow& _Other) = delete;
	MonsterDebugWindow(MonsterDebugWindow&& _Other) noexcept = delete;

private:
	MonsterDebugWindow& operator=(const MonsterDebugWindow& _Other) = delete;
	MonsterDebugWindow& operator=(MonsterDebugWindow&& _Other) noexcept = delete;

public:
protected:
private:
	std::vector<std::string> DebugTexts_;
};

