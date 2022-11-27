#pragma once
#include <GameEngine/GameEngineUIRenderer.h>
#include <GameEngine/GameEngineActor.h>
// �з� : 
// �뵵 : 
// ���� : 
class EndingUIController : public GameEngineActor
{
public:
	EndingUIController(); // default constructer ����Ʈ ������
	~EndingUIController(); // default destructer ����Ʈ �Ҹ���
	EndingUIController(const EndingUIController& _other) = delete; // default Copy constructer ����Ʈ ���������
	EndingUIController(EndingUIController&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������
	EndingUIController& operator=(const EndingUIController& _other) = delete; // default Copy operator ����Ʈ ���� ������
	EndingUIController& operator=(const EndingUIController&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

protected:
	void Start() override;
	void Update(float _DeltaTime) override;



public:
	GameEngineUIRenderer* GetBackGroundRenderer() { return backgroundRenderer_; }
	GameEngineUIRenderer* GetDeskRenderer() { return deskRenderer_; }
	GameEngineUIRenderer* GetPersonRenderer() { return personRenderer_; }
	GameEngineUIRenderer* GetCreditTextRenderer() { return creditTextRenderer_; }
	GameEngineUIRenderer* GetFoxteamRenderer() { return foxTeamRenderer_; }
	void ResourceLoad();
private:
	GameEngineUIRenderer* backgroundRenderer_;
	GameEngineUIRenderer* deskRenderer_;
	GameEngineUIRenderer* personRenderer_;
	GameEngineUIRenderer* creditTextRenderer_;
	GameEngineUIRenderer* foxTeamRenderer_;
};

