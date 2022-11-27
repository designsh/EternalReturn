#pragma once

#include "GameEngineSoundManager.h"

class GameEngineSoundPlayer
{
public:
	// SoundManager 에 로드된 사운드만 재생할 수 있습니다.
	GameEngineSoundPlayer();
	GameEngineSoundPlayer(const std::string& _soundName);
	~GameEngineSoundPlayer();
	GameEngineSoundPlayer(const GameEngineSoundPlayer& _other) = delete;
	GameEngineSoundPlayer(GameEngineSoundPlayer&& _other) = delete;
	GameEngineSoundPlayer& operator=(const GameEngineSoundPlayer& _other) = delete;
	GameEngineSoundPlayer& operator=(const GameEngineSoundPlayer&& _other) = delete;

public:
	void ChangeSound(const std::string& _soundName);

	void Play();
	void Stop();

	bool IsPlaying();
	bool IsPaused();
	void SetPaused(bool _bPause);
	void SetVolume(float _volume);
	void SetPitch(float _pitch);
	void SetPosition(unsigned int _positionMilliSec);
	unsigned int GetPositionMillisecond();
	unsigned int GetLengthMillisecond();

	float GetVolume() const { return volume_; }

private:
	FMOD::Sound* sound_;
	FMOD::Channel* channel_;

	float volume_;
};

