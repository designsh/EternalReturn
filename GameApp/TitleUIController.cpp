#include "PreCompile.h"
#include "TitleUIController.h"
#include "TitleIDInput.h"
#include "TitleBG.h"
#include "TitleStartButton.h"
#include "TitlePrologueBG.h"
#include "TitleFoxLogo.h"
TitleUIController::TitleUIController() // default constructer 디폴트 생성자
	: idInput_(nullptr), titlePrologueBG_(nullptr), titleBG_(nullptr), startButton_(nullptr), logo_(nullptr)
{

}

TitleUIController::~TitleUIController() // default destructer 디폴트 소멸자
{

}

void TitleUIController::InitUI()
{
	idInput_ = GetLevel()->CreateActor<TitleIDInput>();
	titlePrologueBG_ = GetLevel()->CreateActor<TitlePrologueBG>();
	titleBG_ = GetLevel()->CreateActor<TitleBG>();
	startButton_ = GetLevel()->CreateActor<TitleStartButton>();
	logo_ = GetLevel()->CreateActor<TitleFoxLogo>();
}

void TitleUIController::Start()
{
	InitUI();
}

void TitleUIController::Update(float _DeltaTime)
{

}

