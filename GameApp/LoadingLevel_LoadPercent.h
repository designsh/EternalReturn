#pragma once
#include <GameEngine/GameEngineActor.h>

// 분류 : UI
// 용도 : 
// 설명 : 현재 게임에 참여한 플레이어들의 정보 로딩상태 표시
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
	int InGameConnectCount_;										// 현재 게임에 참여한 플레이어수
	bool LoadingState_;												// true: 로딩완료, false: 로딩중
	float LoadingPercent_;											// 로딩 퍼센트(0~100%)
	float LoadingExplanationTime_;									// 게임설명 이미지 갱신 주기
	int CurExplanationImageIndex_;									// 게임설명 이미지목록 현재 표시되는 인덱스

private:
	GameEngineUIRenderer* LoadingCircle_;							// 로딩중 계속하여 회전(완료시 사라짐)
	GameEngineProgressBarRenderer* LoadingBar_;						// 현재 게임에 입장한 플레이어들의 로딩상태를 표시(ProgressBar)
	GameEngineUIRenderer* LoadingText_;								// 다른플레이어기다리는중.. 텍스트 렌더러

private:
	std::vector<GameEngineUIRenderer*> ExplanationImageList_;		// 게임설명 이미지 목록
};

