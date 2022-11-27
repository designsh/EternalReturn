#pragma once
#include <GameEngine/GameEngineActor.h>

// �з� : UI
// �뵵 : 
// ���� : ���� ���ӿ� ������ �÷��̾���� ���� �ε����� ǥ��
class GameEngineProgressBarRenderer;
class GameEngineUIRenderer;
class LoadingLevel_LoadPercent : public GameEngineActor
{
public:
	static LoadingLevel_LoadPercent* Percent;

public:
	void SetConnectPlayerCount();

public:
	void CheckLoadingPercent();

public:
protected:
private:
	void UpdateExplanationImage();

private:
	void Start() override;
	void Update(float _DeltaTime) override;

public:
	LoadingLevel_LoadPercent();
	~LoadingLevel_LoadPercent();

protected:
	LoadingLevel_LoadPercent(const LoadingLevel_LoadPercent& _other) = delete;
	LoadingLevel_LoadPercent(LoadingLevel_LoadPercent&& _other) noexcept = delete;

private:
	LoadingLevel_LoadPercent& operator=(const LoadingLevel_LoadPercent& _other) = delete;
	LoadingLevel_LoadPercent& operator=(const LoadingLevel_LoadPercent&& _other) = delete;

public:
protected:
private:
	int InGameConnectCount_;										// ���� ���ӿ� ������ �÷��̾��
	bool LoadingState_;												// true: �ε��Ϸ�, false: �ε���
	float LoadingPercent_;											// �ε� �ۼ�Ʈ(0~100%)
	float LoadingExplanationTime_;									// ���Ӽ��� �̹��� ���� �ֱ�
	int CurExplanationImageIndex_;									// ���Ӽ��� �̹������ ���� ǥ�õǴ� �ε���

private:
	GameEngineUIRenderer* LoadingCircle_;							// �ε��� ����Ͽ� ȸ��(�Ϸ�� �����)
	GameEngineProgressBarRenderer* LoadingBar_;						// ���� ���ӿ� ������ �÷��̾���� �ε����¸� ǥ��(ProgressBar)
	GameEngineUIRenderer* LoadingText_;								// �ٸ��÷��̾��ٸ�����.. �ؽ�Ʈ ������

private:
	std::vector<GameEngineUIRenderer*> ExplanationImageList_;		// ���Ӽ��� �̹��� ���
};

