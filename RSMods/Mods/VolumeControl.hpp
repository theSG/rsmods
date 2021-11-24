#include "Windows.h"
#include <string>
#include <iostream>
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <Functiondiscoverykeys_devpkey.h>

#include "../WwiseHijack.hpp"
#include "../MemHelpers.hpp"
#include "../Offsets.hpp"


namespace VolumeControl {
	void IncreaseVolume(int amountToIncrease, std::string mixerToIncrease);
	void DecreaseVolume(int amountToDecrease, std::string mixerToDecrease);
	void DisableSongPreviewAudio();
	void EnableSongPreviewAudio();
	void SetupMicrophones();
	void SetMicrophoneVolume(std::string microphoneName,int volume);
	int GetMicrophoneVolume(std::string microphoneName);
	inline bool disabledSongPreviewAudio = false;
	inline std::map<std::string, LPWSTR> activeMicrophones;
}

inline std::vector<std::string> mixerNames = {
	{"Master_Volume"}, // Master Volume
	{"Mixer_Music"}, // Song Volume
	{"Mixer_Player1"}, // Player 1 Guitar & Bass (both are handled with this singular name)
	{"Mixer_Player2"}, // Player 2 Guitar & Bass (both are handled with this singular name)
	{"Mixer_Mic"}, // My Microphone Volume
	{"Mixer_VO"}, // Rocksmith Dad Voice Over
	{"Mixer_SFX"}, // Menu SFX Volume
};