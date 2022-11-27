#pragma once
#include <GameEngine/GameEngineGUI.h>

// �з� : IMGUI Window
// �뵵 : �����(Ȯ�ο�)
// ���� : ���� �⺻���� üũ�� ���� ����� ������
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

