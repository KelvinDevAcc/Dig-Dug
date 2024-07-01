#include "sdl_sound_system.h"


sdl_sound_system::sdl_sound_system() : m_running(true),m_muted(false), m_volume(0), m_previousVolume(0) {
    // Initialize SDL mixer
    if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_FLAC | MIX_INIT_MOD | MIX_INIT_OGG) < 0) {
        std::cerr << "SDL mixer initialization failed: " << Mix_GetError() << std::endl;
        return;
    }

    // Open the audio device
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL audio device opening failed: " << Mix_GetError() << std::endl;
        return;
    }

    m_thread = std::make_unique<std::jthread>([this] { process_events(); });

    dae::EventQueue::AddListener(dae::PlaySoundMessageType::Sound, this, &sdl_sound_system::onPlaySoundMessage);
}

sdl_sound_system::~sdl_sound_system() {

    stop();
}


void sdl_sound_system::onPlaySoundMessage(const dae::Message& message) {
    try {
        if (message.arguments.size() != 1) {
            std::cerr << "Invalid number of arguments for PlaySound message" << std::endl;
            return;
        }

        const sound_id soundID = std::any_cast<decltype(soundID)>(message.arguments[0]);

        play(soundID);
    }
    catch (const std::bad_any_cast& e) {
        std::cerr << "Error extracting arguments from PlaySound message: " << e.what() << std::endl;
    }
}

void sdl_sound_system::play(const sound_id id) {
    std::unique_lock<std::mutex> lock(m_sound_requests_mutex);
    m_sound_requests.push({ id, static_cast<float>(m_volume) }); // Use the stored volume
    lock.unlock();
    m_sound_requests_cv.notify_one();
}

void sdl_sound_system::process_events() {
    while (m_running) {
        // Lock the mutex before accessing the sound requests queue
        std::unique_lock<std::mutex> lock(m_sound_requests_mutex);
        // Wait until there's a sound request or the sound system is stopped
        m_sound_requests_cv.wait(lock, [this] { return !m_sound_requests.empty() || !m_running; });

        if (!m_running) {
            break;
        }

        // Process the first sound request in the queue
        SoundRequest request = m_sound_requests.front();
        m_sound_requests.pop();
        lock.unlock();

        // Find the music or sound effect associated with the sound ID
        Mix_Chunk* chunk = nullptr;
        Mix_Music* music = nullptr;
        auto chunkIt = m_id_to_chunk_map.find(request.id);
        auto musicIt = m_id_to_music_map.find(request.id);

        if (chunkIt != m_id_to_chunk_map.end()) {
            chunk = chunkIt->second;
        }
        else if (musicIt != m_id_to_music_map.end()) {
            music = musicIt->second;
        }
        else {
            std::cerr << "Failed to find audio for sound ID: " << request.id << std::endl;
            continue;
        }

        if (chunk) {
            Mix_PlayChannel(-1, chunk, 0); // Play the chunk on any available channel
            Mix_VolumeChunk(chunk, static_cast<int>(request.volume));
        }
        else if (music) {
            Mix_PlayMusic(music, -1);
            Mix_VolumeMusic(static_cast<int>(request.volume));
        }
    }

    Mix_CloseAudio();
    Mix_Quit();
}

void sdl_sound_system::stop() {
    // Lock the mutex before modifying the running flag
    std::unique_lock<std::mutex> lock(m_sound_requests_mutex);
    m_running = false; // Set the running flag to false
    lock.unlock(); 
    m_sound_requests_cv.notify_all(); // Notify all waiting threads about the stop signal
    dae::EventQueue::RemoveListener(this);
}

void sdl_sound_system::register_sound_file(const std::string& file_path) {
    // Lock the mutex before modifying the file path to ID map
    std::lock_guard<std::mutex> lock(m_sound_requests_mutex);

    const sound_id new_id = next_sound_id++;

    m_file_path_to_id_map[file_path] = new_id;
}

sound_id sdl_sound_system::get_sound_id_for_file_path(const std::string& file_path) {
    // Lock the mutex before accessing the file path to ID map
    std::lock_guard<std::mutex> lock(m_sound_requests_mutex);

    // Check if the file path exists in the map
    if (const auto it = m_file_path_to_id_map.find(file_path); it != m_file_path_to_id_map.end()) {
        return it->second; // Return the corresponding ID
    }

    return 0;
}

void sdl_sound_system::load_sound(sound_id id, const std::string& file_path, bool isBackgroundMusic) {
    if (isBackgroundMusic) {
        // Background music, load it and store it in the background music map
        if (Mix_Music* music = Mix_LoadMUS(file_path.c_str())) {
            std::lock_guard<std::mutex> lock(m_sound_requests_mutex);
            m_id_to_music_map[id] = music;
        }
        else {
            std::cerr << "Failed to load background music for sound ID: " << id << ". SDL Mixer Error: " << Mix_GetError() << std::endl;
        }
    }
    else {
        // Normal sound effect, load it as usual
        if (Mix_Chunk* chunk = Mix_LoadWAV(file_path.c_str())) {
            std::lock_guard<std::mutex> lock(m_sound_requests_mutex);
            m_id_to_chunk_map[id] = chunk;
        }
        else {
            std::cerr << "Failed to load sound effect for sound ID: " << id << ". SDL Mixer Error: " << Mix_GetError() << std::endl;
        }
    }
}


void sdl_sound_system::unload_sound(sound_id id) {
    // Lock the mutex before modifying the ID to music map
    std::lock_guard<std::mutex> lock(m_sound_requests_mutex);
    auto it = m_id_to_music_map.find(id);
    if (it != m_id_to_music_map.end()) {
        Mix_FreeMusic(it->second); // Free the memory allocated for the music
        m_id_to_music_map.erase(it); // Remove the entry from the map
    }
}

void sdl_sound_system::mute() {
    // Lock the mutex before modifying the mute state and volume
    std::lock_guard<std::mutex> lock(m_sound_requests_mutex);
    if (m_muted) {
        // Unmute: Restore previous volume
        m_muted = false;
        m_volume = m_previousVolume;
        Mix_VolumeMusic(m_volume); // Set the volume back to the previous level
    }
    else {
        // Mute: Store current volume and set volume to 0
        m_muted = true;
        m_previousVolume = m_volume; // Store the current volume
        m_volume = 0;
        Mix_VolumeMusic(0); // Mute the music
    }
}


void sdl_sound_system::setVolume(float volume) {
    std::lock_guard<std::mutex> lock(m_sound_requests_mutex);
    m_volume = static_cast<int>(volume * MIX_MAX_VOLUME);
    if (!m_muted) {
        Mix_VolumeMusic(m_volume);
    }
    else {
        // If sound is muted, don't set volume directly, just store it for when it's unmuted
        m_previousVolume = m_volume; // Store the current volume
        std::cout << "Sound is muted. Volume change will take effect when sound is unmuted." << std::endl;
    }
}

float sdl_sound_system::getVolume() {
    std::lock_guard<std::mutex> lock(m_sound_requests_mutex);
    return static_cast<float>(m_volume) / MIX_MAX_VOLUME;
}


void logging_sound_system::onPlaySoundMessage(const dae::Message& message) {
    try {
        if (message.arguments.size() != 1) {
            std::cerr << "Invalid number of arguments for PlaySound message" << std::endl;
            return;
        }

        const sound_id soundID = std::any_cast<decltype(soundID)>(message.arguments[0]);

        const float currentVolume = _real_ss->getVolume();
        play(soundID);
        std::cout << "Playing sound ID " << soundID << " at volume " << currentVolume << std::endl;
    }
    catch (const std::bad_any_cast& e) {
        std::cerr << "Error extracting arguments from PlaySound message: " << e.what() << std::endl;
    }
}


