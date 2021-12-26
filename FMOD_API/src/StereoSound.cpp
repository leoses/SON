#include "StereoSound.h"
#include "Utils.h"

StereoSound::StereoSound(FMOD::System* system, std::string filePath, unsigned int flags)
{
	ERRCHECK(system->createSound(filePath.c_str(), flags, 0, &sound));
	ERRCHECK(system->playSound(sound, 0, true, &channel));
}

StereoSound::~StereoSound()
{
	if (sound != nullptr)ERRCHECK(sound->release());
}

void StereoSound::playSound()
{
	ERRCHECK(channel->setPaused(false));
}

void StereoSound::pauseSound()
{
	ERRCHECK(channel->setPaused(true));
}

void StereoSound::stopSound()
{
	ERRCHECK(channel->stop());
}

bool StereoSound::isPlaying()const
{
	bool isPlaying;
	ERRCHECK(channel->isPlaying(&isPlaying));
	return isPlaying;
}

void StereoSound::setVolume(float newVolume)
{
	if (newVolume >= 0.0f) ERRCHECK(channel->setVolume(newVolume));
}

float StereoSound::getVolume() const
{
	float volume;
	ERRCHECK(channel->getVolume(&volume));
	return volume;
}

void StereoSound::setPan(float newPan)
{
	if (newPan >= -1.0f && newPan <= 1.0f)ERRCHECK(channel->setPan(newPan));
}

void StereoSound::fadeIn(int milliseconds, float volumeToReach)
{
	float volume = getVolume();
	volumeToReach_ = volumeToReach;
	variationVolume = (volumeToReach - volume) / milliseconds;
	fadeEvent = FADE_EVENT::fadeIn;
}

void StereoSound::fadeOut(int milliseconds, float volumeToReach)
{
	float volume = getVolume();
	variationVolume = (volume - volumeToReach) / milliseconds;
	fadeEvent = FADE_EVENT::fadeOut;
}


void StereoSound::mixSounds(FMOD::System* system, FMOD::Sound* newSound, int milliseconds)
{
	ERRCHECK(system->playSound(newSound, 0, true, &mixChannel));
	ERRCHECK(mixChannel->setVolume(0.0f));
	float volume = getVolume();
	variationVolume = (volume) / milliseconds;
	fadeEvent = FADE_EVENT::mixSound;
	ERRCHECK(mixChannel->setPaused(false));
}

void StereoSound::fadeInFancy(int time)
{
	unsigned long long dspclock;
	FMOD::System* sys;
	int rate;
	pauseSound();

	ERRCHECK(channel->getSystemObject(&sys));                        // OPTIONAL : Get System object
	ERRCHECK(sys->getSoftwareFormat(&rate, 0, 0));				   // Get mixer rate

	ERRCHECK(channel->getDSPClock(0, &dspclock));                    // Get the reference clock, which is the parent channel group

	ERRCHECK(channel->addFadePoint(dspclock, 0.0f));                 // Add a fade point at 'now' with full volume.
	ERRCHECK(channel->addFadePoint(dspclock + (rate * time), 1.0f));    // Add a fade point 5 seconds later at 0 volume.
	playSound();
}

void StereoSound::fadeOutFancy(int time)
{
	unsigned long long dspclock;
	FMOD::System* sys;
	int rate;
	pauseSound();

	ERRCHECK(channel->getSystemObject(&sys));                        // OPTIONAL : Get System object

	ERRCHECK(sys->getSoftwareFormat(&rate, 0, 0));                // Get mixer rate

	ERRCHECK(channel->getDSPClock(0, &dspclock));                    // Get the reference clock, which is the parent channel group
	ERRCHECK(channel->addFadePoint(dspclock, 1.0f));                 // Add a fade point at 'now' with full volume.
	ERRCHECK(channel->addFadePoint(dspclock + (rate * time), 0.0f));    // Add a fade point 5 seconds later at 0 volume.
	ERRCHECK(channel->setDelay(0, dspclock + (rate * time), true));     // Add a delayed stop command at 5 seconds ('stopchannels = true')
	playSound();
}

void StereoSound::update(float deltaTime)
{
	if (fadeEvent == FADE_EVENT::fadeIn) {
		//FadeIn
		setVolume(getVolume() + variationVolume * deltaTime);
		if (getVolume() >= volumeToReach_) {
			fadeEvent = FADE_EVENT::Undefined;
		}

	}
	else if (fadeEvent == FADE_EVENT::fadeOut) {
		setVolume(getVolume() - variationVolume * deltaTime);
		if (getVolume() <= 0) {
			fadeEvent = FADE_EVENT::Undefined;
		}
	}
	else if (fadeEvent == FADE_EVENT::mixSound) {
		setVolume(getVolume() - variationVolume * deltaTime);
		if (mixChannel != nullptr) {
			float volume; mixChannel->getVolume(&volume);
			ERRCHECK(mixChannel->setVolume(volume + variationVolume * deltaTime));
		}

		if (getVolume() <= 0) {
			channel = mixChannel;
			mixChannel = nullptr;
			ERRCHECK(sound->release());
			sound = nullptr;
			fadeEvent = FADE_EVENT::Undefined;
		}

	}
}

