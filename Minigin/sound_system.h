#pragma once
#include <string>
#include "EnventQueue.h"

using sound_id = unsigned short;
class sound_system
{
public:
	sound_system() = default;
	virtual ~sound_system() = default;

	sound_system(const sound_system&) = delete;
	sound_system& operator=(const sound_system&) = delete;
	sound_system(sound_system&&) = delete;
	sound_system& operator=(sound_system&&) = delete;

	virtual void play(const sound_id id) = 0;
	virtual void mute() = 0;
	virtual void onPlaySoundMessage(const dae::Message& message) = 0;
	virtual void register_sound_file(const std::string& file_path) = 0;
	virtual sound_id get_sound_id_for_file_path(const std::string& file_path) = 0;
	virtual float getVolume() = 0;
	virtual void load_sound(sound_id id, const std::string& file_path, bool isBackgroundMusic = false) = 0;
	virtual void unload_sound(sound_id id) = 0;
	virtual void setVolume(float volume) = 0;
};

