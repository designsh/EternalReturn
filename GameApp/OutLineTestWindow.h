#pragma once
#include <GameEngine/GameEngineGUI.h>

// �з� : IMGUI Window
// �뵵 : �׽�Ʈ��
// ���� : �޽� ���� �� �ƿ����� ���� ���, �ƿ����� �β����� �� ����� ����
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

