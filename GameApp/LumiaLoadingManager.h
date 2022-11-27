#pragma once

// �з� : 
// �뵵 : 
// ���� : 
class LumiaLoadingManager
{
public:
	LumiaLoadingManager(); // default constructer ����Ʈ ������
	~LumiaLoadingManager(); // default destructer ����Ʈ �Ҹ���
	LumiaLoadingManager(const LumiaLoadingManager& _other) = delete; // default Copy constructer ����Ʈ ���������
	LumiaLoadingManager(LumiaLoadingManager&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������
	LumiaLoadingManager& operator=(const LumiaLoadingManager& _other) = delete; // default Copy operator ����Ʈ ���� ������
	LumiaLoadingManager& operator=(const LumiaLoadingManager&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:
	static LumiaLoadingManager* GetInstance() { static LumiaLoadingManager instance; return &instance; }

	//void LoadAnimals();
	//void LoadItemBoxes();


public:


protected:


private:


};

