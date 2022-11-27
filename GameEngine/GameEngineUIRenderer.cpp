#include "PreCompile.h"
#include "GameEngineUIRenderer.h"
#include "GameEngineLevel.h"
#include "CameraComponent.h"
#include "GameEngineFont.h"
#include "GameEngineFontManager.h"
#include "GameEngineWindow.h"
#include "GameEngineRenderTarget.h"

GameEngineRenderTarget* GameEngineUIRenderer::FontTarget_ = nullptr;
int GameEngineUIRenderer::UIRendererCount = 0;

GameEngineUIRenderer::GameEngineUIRenderer()
	: IsText_(false)
	, FontName_("�ü�")
	, PrintText_("")
	, FontSize_(20)
	, FontPivot_(float4::ZERO)
	, Color_(float4::WHITE)
	, Flags_(FW1_CENTER | FW1_VCENTER)
	, MaxLen_(0)
{
	++UIRendererCount;
}

GameEngineUIRenderer::~GameEngineUIRenderer()
{
	--UIRendererCount;
	if (0 == UIRendererCount && nullptr != FontTarget_)
	{
		delete FontTarget_;
		FontTarget_ = nullptr;
	}
}

void GameEngineUIRenderer::Start()
{
	GetLevel()->GetUICamera()->PushRenderer(GetOrder(), this);
	SetRenderingPipeLine("TextureAtlas");
	GetGameEngineRenderingPipeLine()->SetOutputMergerDepthStencil("BaseDepthOff");

	if (nullptr == FontTarget_ && UIRendererCount == 1)
	{
		FontTarget_ = new GameEngineRenderTarget();
		FontTarget_->Create(GameEngineWindow::GetInst().GetSize(), float4::NONE);
	}
}

void GameEngineUIRenderer::SetRenderGroup(int _Order)
{
	GetLevel()->GetUICamera()->ChangeRendererGroup(_Order, this);;
}

void GameEngineUIRenderer::TextSetting(std::string _FontName, std::string _PrintText, float _FontSize, unsigned int _Flags, float4 _Color, const float4& _FontPivot, int _MaxLen)
{
	// �ش� UIRenderer�� �ؽ�Ʈ�� ����ϹǷ� Flag On
	IsText_ = true;

	// �ؽ�Ʈ �������� �ʿ��� �⺻������ ����
	FontName_ = _FontName;
	PrintText_ = _PrintText;
	FontSize_ = _FontSize;
	Color_ = _Color;
	Flags_ = _Flags;
	FontPivot_ = _FontPivot;
	MaxLen_ = _MaxLen;
}

void GameEngineUIRenderer::Render(float _DeltaTime, bool _IsDeferred)
{
	if (true == _IsDeferred)
	{
		return;
	}

	GameEngineRenderer::Render(_DeltaTime, _IsDeferred);

	// �ؽ�Ʈ ������ �����ʾҴٸ� ó������
	if (false == IsText_)
	{
		return;
	}

	// Font ���� ����Ÿ�� �������� ���� �� UI RenderTaret���� ����ȯ�ϱ����� ���� ������ ����Ÿ�� ����
	GameEngineRenderTarget* UIRenderTarget = GameEngineRenderTarget::GetLastRenderTarget();

	FontTarget_->Clear();
	FontTarget_->Setting();

	// UIRenderer�� ȭ���� �߾��� ����(0,0,0)�� �������� �����Ǿ��ְ�,
	// GameEngineFont�� ��ũ����ǥ(��������ǥ)�� �������� �����Ǿ������Ƿ�,
	// �������� ũ��� UIRenderer�� ��ġ�� �̿��Ͽ� ��ũ����ǥ�迡���� Font�� ��ġ�� 
	// ����Ѵ�.

	// ������ ũ�� Get
	float4 ScreenSize = GameEngineWindow::GetInst().GetSize();
	ScreenSize = ScreenSize.halffloat4();

	// UIRenderer ������ġ Get
	float4 UIPos = GetTransform()->GetWorldPosition();

	// ��Ʈ ������ ��ġ ���
	// UIPos�� �����̸� x = ScreenSize - UIPos
	// UIPos�� ����̸� x = ScreenSize + UIPos
	float4 RenderPos = float4::ZERO;
	RenderPos.x = ScreenSize.x + UIPos.x;
	RenderPos.y = ScreenSize.y - UIPos.y;

	GameEngineFont* Font = GameEngineFontManager::GetInst().Find(FontName_);
	if (nullptr != Font)
	{
		Font->DrawFont(PrintText_, FontSize_, RenderPos, FontPivot_, Color_, Flags_);
	}

	// Shader Reset
	GameEngineDevice::ShaderReset();

	// Font RenderTarget Merge To UI Render Target
	UIRenderTarget->Merge(FontTarget_);

	// RenderTarget�� UIRender Target���� ����
	UIRenderTarget->Setting();
}