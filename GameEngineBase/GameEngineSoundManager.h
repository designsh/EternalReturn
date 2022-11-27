#pragma once

#pragma warning(disable:26812)

#include "../ThirdParty/include/FMOD/fmod.hpp"

#ifdef _DEBUG
#pragma comment(lib, "../ThirdParty/x64/Debug/FMOD/fmodL_vc.lib")
#else
#pragma comment(lib, "../ThirdParty/x64/Release/FMOD/fmod_vc.lib")
#endif


#include <unordered_map>

class GameEngineSoundPlayer;
class GameEngineSoundManager
{
	friend GameEngineSoundPlayer;
public:
	~GameEngineSoundManager();

	GameEngineSoundManager(const GameEngineSoundManager& _other) = delete;
	GameEngineSoundManager(GameEngineSoundManager&& _other) = delete;

	GameEngineSoundManager& operator=(const GameEngineSoundManager& _other) = delete;
	GameEngineSoundManager& operator=(const GameEngineSoundManager&& _other) = delete;

public:
	static GameEngineSoundManager* GetInstance() { static GameEngineSoundManager instance; return &instance; }

	void Initialize();
	void Update();

	void CreateSound(const std::string& _name, const std::string& _path, bool _bLoop = false);
	void ReleaseSound(const std::string& _name);

	void PlaySoundByName(const std::string& _name);
	void SetGlobalVolume(float _volume);
	void StopSound();

	float GetGlobalVolume() const { return globalVolume_; }

private:
	GameEngineSoundManager();

	FMOD::Sound* getSound(const std::string& _name);

private:
	static float globalVolume_;

private:
	FMOD::System* system_;
	FMOD::Channel* channel_;

	std::unordered_map<std::string, FMOD::Sound*> allSounds_;
};

