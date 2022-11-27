#include "Precompile.h"
#include "MonsterDebugWindow.h"

void MonsterDebugWindow::AddText(const std::string& _DebugText)
{
	DebugTexts_.push_back(_DebugText);
}

void MonsterDebugWindow::OnGUI()
{
	// Debug Text View
	for (auto& Text : DebugTexts_)
	{
		ImGui::Text(Text.c_str());
		ImGui::NextColumn();
	}
	DebugTexts_.clear();

	// ...

}

MonsterDebugWindow::MonsterDebugWindow()
{
}

MonsterDebugWindow::~MonsterDebugWindow()
{
}
