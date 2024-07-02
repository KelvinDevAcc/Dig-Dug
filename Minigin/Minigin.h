#pragma once
#include <string>
#include <functional>

namespace dae
{
    class minigin
    {
    public:
        explicit minigin(const std::string& dataPath);
        ~minigin();
        void Run(const std::function<void()>& load);

        minigin(const minigin& other) = delete;
        minigin(minigin&& other) = delete;
        minigin& operator=(const minigin& other) = delete;
        minigin& operator=(minigin&& other) = delete;

    private:
        int m_FrameRate = 60;
    };
}
