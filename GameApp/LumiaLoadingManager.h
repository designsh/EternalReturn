#pragma once

// 분류 : 
// 용도 : 
// 설명 : 
class LumiaLoadingManager
{
public:
	LumiaLoadingManager(); // default constructer 디폴트 생성자
	~LumiaLoadingManager(); // default destructer 디폴트 소멸자
	LumiaLoadingManager(const LumiaLoadingManager& _other) = delete; // default Copy constructer 디폴트 복사생성자
	LumiaLoadingManager(LumiaLoadingManager&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자
	LumiaLoadingManager& operator=(const LumiaLoadingManager& _other) = delete; // default Copy operator 디폴트 대입 연산자
	LumiaLoadingManager& operator=(const LumiaLoadingManager&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:
	static LumiaLoadingManager* GetInstance() { static LumiaLoadingManager instance; return &instance; }

	//void LoadAnimals();
	//void LoadItemBoxes();


public:


protected:


private:


};

