#pragma once

#include <functional>

namespace Easing
{
    using EasingFunction = std::function<float(float)>;

    inline float Linear(float t) { return t; };
    inline float EaseInQuad(float t) { return t * t; };
    inline float EaseOutQuad(float t) { return t * (2 - t); };
    inline float EaseInOutQuad(float t) { return t < 0.5 ? 2 * t * t : -1 + (4 - 2 * t) * t;};
    inline float Bounce(float t)
	{
    	if (t < 0.5f) return (1 - Bounce(1 - 2 * t)) / 2;
        if (t < 0.75f) return (3 - 2 * (1 - 4 * t)) / 2;
        return 1 - Bounce(1 - 2 * (t - 0.75f));
    };

    inline float EaseOutBounce(float t) {
        if (t < (1 / 2.75f)) {
            return 7.5625f * t * t;
        }
        else if (t < (2 / 2.75f)) {
            t -= (1.5f / 2.75f);
            return 7.5625f * t * t + 0.75f;
        }
        else if (t < (2.5 / 2.75)) {
            t -= (2.25f / 2.75f);
            return 7.5625f * t * t + 0.9375f;
        }
        else {
            t -= (2.625f / 2.75f);
            return 7.5625f * t * t + 0.984375f;
        }
    }

    // Add more easing functions as needed
}
