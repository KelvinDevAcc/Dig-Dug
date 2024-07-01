#pragma once
#include <map>
#include <string>
#include <vector>
#include <glm/vec2.hpp>
#include "Texture2D.h"

namespace dae
{

    struct SpriteAnimation final
    {
        SpriteAnimation(const std::vector<glm::ivec2>& cellFrames, int framesPerSecond);

        const glm::ivec2& GetCellFromNormalizedTime(float time) const;

        std::vector<glm::ivec2> cellFrames{};
        int frameCount{};
        int framesPerSecond{};
    };

    class Sprite final
    {
    public:
        Sprite(Texture2D* texturePtr, int rowCount, int colCount, const std::map<std::string, SpriteAnimation>& animations = {});
        ~Sprite();

        Texture2D& GetTexture() const;
        const SpriteAnimation* GetAnimation(const std::string& name) const;

        Texture2D* m_texture;
        glm::ivec2 cellSize;
        std::map<std::string, SpriteAnimation> animations;
    };
}
