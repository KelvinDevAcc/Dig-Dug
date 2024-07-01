#include "Sprite.h"
#include <algorithm>
#include <stdexcept>
#include "Texture2D.h"


dae::SpriteAnimation::SpriteAnimation(const std::vector<glm::ivec2>& cellFrames, int framesPerSecond) :
    cellFrames(cellFrames),
    frameCount(static_cast<int>(cellFrames.size())),
    framesPerSecond(framesPerSecond)
{}

const glm::ivec2& dae::SpriteAnimation::GetCellFromNormalizedTime(float time) const
{
    int frame = static_cast<int>(time * static_cast<float>(frameCount));
    frame = std::clamp(frame, 0, frameCount - 1);
    return cellFrames[frame];
}

const dae::SpriteAnimation* dae::Sprite::GetAnimation(const std::string& name) const
{
	const auto animationIt = animations.find(name);
    if (animationIt == animations.end()) {
        throw std::invalid_argument("Animation does not exist");
    }
    return &animationIt->second;
}

dae::Sprite::Sprite(dae::Texture2D* texturePtr, int rowCount, int colCount,
                    const std::map<std::string, SpriteAnimation>& animations) :

	m_texture(texturePtr),
	cellSize(texturePtr->GetSize().x / colCount, texturePtr->GetSize().y / rowCount),
    animations(animations)
{
}

dae::Sprite::~Sprite()
{
    if (m_texture)
    {
        m_texture = nullptr;
    }
}

dae::Texture2D& dae::Sprite::GetTexture() const
{
    return *m_texture;
}
