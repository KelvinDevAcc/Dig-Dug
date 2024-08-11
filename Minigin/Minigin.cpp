#include <stdexcept>
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Minigin.h"

#include <thread>

#include "GameData.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "GameTime.h"
#include "SceneData.h"
#include "TweenManager.h"

SDL_Window* g_window{};

void PrintSDLVersion()
{
    SDL_version version{};
    SDL_VERSION(&version)
    printf("We compiled against SDL version %u.%u.%u ...\n",
        version.major, version.minor, version.patch);

    SDL_GetVersion(&version);
    printf("We are linking against SDL version %u.%u.%u.\n",
        version.major, version.minor, version.patch);

    SDL_IMAGE_VERSION(&version)
    printf("We compiled against SDL_image version %u.%u.%u ...\n",
        version.major, version.minor, version.patch);

    version = *IMG_Linked_Version();
    printf("We are linking against SDL_image version %u.%u.%u.\n",
        version.major, version.minor, version.patch);

    SDL_TTF_VERSION(&version)
    printf("We compiled against SDL_ttf version %u.%u.%u ...\n",
        version.major, version.minor, version.patch);

    version = *TTF_Linked_Version();
    printf("We are linking against SDL_ttf version %u.%u.%u.\n",
        version.major, version.minor, version.patch);
}

dae::minigin::minigin(const std::string& dataPath)
{
    PrintSDLVersion();

    // Set SDL hint for VSync
    SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");

    // Initialize SDL video subsystem
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
    }

    // Create SDL window
    g_window = SDL_CreateWindow(
        "Programming 4 assignment",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        1270,
        720,
        SDL_WINDOW_OPENGL
    );
    if (g_window == nullptr)
    {
        throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
    }

    // Initialize renderer instance
    Renderer::GetInstance().Init(g_window);

    // Initialize ResourceManager
    ResourceManager::GetInstance().Init(dataPath);
}

dae::minigin::~minigin()
{
    Renderer::GetInstance().Destroy();
    SDL_DestroyWindow(g_window);
    g_window = nullptr;
    SDL_Quit();
}

void dae::minigin::Run(const std::function<void()>& load)
{
    load();

    const auto& renderer = Renderer::GetInstance();
    const auto& sceneManager = SceneManager::GetInstance();
    const auto& input = InputManager::GetInstance();

    GameTime::Start();

    while (true)
    {
        const bool doContinue = input.ProcessInput();
        if (!doContinue)
            break;

        GameTime::Update();

        TweenManager::GetInstance().Update();

        sceneManager.Update();

        SceneData::GetInstance().Update();

        renderer.Render();

        EventQueue::Process();

        // Sleep to maintain frame rate
        const auto sleepTime = GameTime::m_LastTime + std::chrono::milliseconds(1000 / m_FrameRate) - std::chrono::high_resolution_clock::now();
        if (sleepTime > std::chrono::milliseconds(0))
        {
            std::this_thread::sleep_for(sleepTime);
        }
    }
}
