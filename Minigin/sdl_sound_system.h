#pragma once

#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include "sound_system.h"
#include <SDL_mixer.h>

#include "EnventQueue.h"


class sdl_sound_system final : public sound_system {
public:
    sdl_sound_system();
    ~sdl_sound_system() override;

    sdl_sound_system(sdl_sound_system&&) = delete;
    sdl_sound_system(const sdl_sound_system&) = delete;
    sdl_sound_system& operator=(sdl_sound_system&&) = delete;
    sdl_sound_system& operator=(const sdl_sound_system&) = delete;

    void play(const sound_id id) override;
    void mute() override;
    float getVolume() override;
    void setVolume(float volume) override;
    void process_events();
    void stop();
    void register_sound_file(const std::string& file_path) override;
    sound_id get_sound_id_for_file_path(const std::string& file_path) override;
    void load_sound(sound_id id, const std::string& file_path, bool isBackgroundMusic = false) override;
    void unload_sound(sound_id id) override;

private:

    void onPlaySoundMessage(const dae::Message& message) override;

    struct SoundRequest {
        sound_id id;
        float volume;
    };
    sound_id next_sound_id = 0;
    std::queue<SoundRequest> m_sound_requests;
    std::mutex m_sound_requests_mutex;
    std::condition_variable m_sound_requests_cv;
    std::unique_ptr<std::jthread> m_thread;
    bool m_running;
    bool m_muted;
    int m_volume;
    int m_previousVolume;
    std::map<std::string, sound_id> m_file_path_to_id_map;
    std::map<sound_id, Mix_Music*> m_id_to_music_map;
    std::map<sound_id, Mix_Chunk*> m_id_to_chunk_map;
};


class logging_sound_system final : public sound_system {
public:
    logging_sound_system(std::unique_ptr<sound_system>&& ss) : _real_ss(std::move(ss))
    {
        dae::EventQueue::AddListener(dae::PlaySoundMessageType::Sound, this, &logging_sound_system::onPlaySoundMessage);
    }

    ~logging_sound_system() override = default;


    logging_sound_system(logging_sound_system&&) = delete;
    logging_sound_system(const logging_sound_system&) = delete;
    logging_sound_system& operator=(logging_sound_system&&) = delete;
    logging_sound_system& operator=(const logging_sound_system&) = delete;

    void register_sound_file(const std::string& file_path) override
    {
        _real_ss->register_sound_file(file_path);
    }

    sound_id get_sound_id_for_file_path(const std::string& file_path) override
    {
        return _real_ss->get_sound_id_for_file_path(file_path);
    }
    void load_sound(sound_id id, const std::string& file_path, bool isBackgroundMusic) override
    {
        _real_ss->load_sound(id, file_path, isBackgroundMusic);
    }
    void unload_sound(sound_id id) override
    {
        _real_ss->unload_sound(id);
    }
    void play(const sound_id id) override
    {
        _real_ss->play(id);
    }

    void mute() override
    {
        _real_ss->mute();
    }

    void setVolume(float volume) override {
        _real_ss->setVolume(volume);
    }

    float getVolume() override {
        return _real_ss->getVolume();
    }

private:
    void onPlaySoundMessage(const dae::Message& message) override;
    std::unique_ptr<sound_system> _real_ss;

};
