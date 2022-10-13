#include "Random.h"

#include "Vec2.h"
#include "ColorF.h"


AR2D_BEGIN_NAMESPACE

std::mt19937& Random::GetEngine() noexcept
{
    return s_Engine;
}

Vec2 Random::RandomVec2() noexcept
{
    return {RandomFloat(), RandomFloat()};
}

Vec2 Random::RandomVec2(Vec2 min, Vec2 max) noexcept
{
    AR2D_ASSERT(min < max, "Invalid Random Passed to Random::RandomVec2");
    return {RandomFloat(min.x, max.x), RandomFloat(min.y, max.y)};
}

ColorF Random::RandomColorF() noexcept
{
    return {RandomFloat(), RandomFloat(), RandomFloat(), 1.f};
}

ColorF Random::RandomColorFWithAlpha() noexcept
{
    return {RandomFloat(), RandomFloat(), RandomFloat(), RandomFloat()};
}

AR2D_END_NAMESPACE
