#pragma once
#include "fmod.hpp" 
#include <string>

enum class FADE_EVENT {
	fadeIn = 0, fadeOut, mixSound, Undefined
};
class StereoSound
{
private:
	FMOD::Channel* channel = nullptr;
	FMOD::Sound* sound = nullptr;
	FMOD::Channel* mixChannel = nullptr;
	FADE_EVENT fadeEvent = FADE_EVENT::Undefined;
	float variationVolume = 0.0f;
	float volumeToReach_ = 0.0f;

public:
	StereoSound(FMOD::System* system, std::string filePath, unsigned int flags);
	~StereoSound();

	void playSound();
	void pauseSound();
	void stopSound();
	bool isPlaying() const;
	void setVolume(float newVolume);
	float getVolume()const;
	void setPan(float newPan);
	void fadeIn(int milliseconds, float volumeToReach);
	void fadeOut(int milliseconds, float volumeToReach);
	void mixSounds(FMOD::System* system, FMOD::Sound* newSound, int milliseconds);
	void fadeInFancy(int time);
	void fadeOutFancy(int time);

	void update(float deltaTime);
};

