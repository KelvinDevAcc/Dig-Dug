#pragma once
#include <algorithm>
#include <iostream>
#include <memory>
#include "sound_system.h"

class null_sound_system final : public sound_system
{
public:

    null_sound_system() = default;
    ~null_sound_system() override = default;

    null_sound_system(null_sound_system&&) = delete;
    null_sound_system(const null_sound_system&) = delete;
    null_sound_system& operator=(null_sound_system&&) = delete;
    null_sound_system& operator=(const null_sound_system&) = delete;

    void play(const sound_id id) override
    {
        std::cout << "null_sound_system is not playing id: " << id << std::endl;
    }

    void onPlaySoundMessage(const dae::Message& /*message*/) override
    {
        play(0);
    }

    void register_sound_file(const std::string& /*file_path*/) override {}
    sound_id get_sound_id_for_file_path(const std::string& /*file_path*/) override { return 0; }
    void load_sound(sound_id /*id*/, const std::string& /*file_path*/, bool /*isBackgroundMusic*/) override {}
    void unload_sound(sound_id /*id*/) override {}
    void mute() override{}
    void setVolume(float /*volume*/) override {}
    float getVolume() override { return 0.0f; }
};
class servicelocator final
{
	static std::unique_ptr<sound_system> _ss_instance;
public:
	static sound_system& get_sound_system() { return *_ss_instance; }
	static void register_sound_system(std::unique_ptr<sound_system>&& ss)
	{
		_ss_instance = ss == nullptr ? std::make_unique<null_sound_system>() : std::move(ss);
	}

};

