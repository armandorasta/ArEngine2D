#pragma once

#include "EngineCore.h"

#include <random>

AR2D_BEGIN_NAMESPACE

class Vec2;
class ColorF;

class Random
{
private:
	inline static std::mt19937 s_Engine{std::random_device{}()};

public:

	Random() = delete;

public:

	/**
	 * @return a reference to the internal eng used.
	*/
	static std::mt19937& GetEngine() noexcept;

	/**
	 * @return a random floating point number in [0, 1].
	*/
	template <std::floating_point FloatType = float>
	static FloatType RandomFloat() noexcept
	{
		return RandomFloat(static_cast<FloatType>(0), static_cast<FloatType>(1));
	}

	/**
	 * @return a random floating point number in [min, max].
	*/
	template <std::floating_point FloatType>
	static FloatType RandomFloat(FloatType min, FloatType max) noexcept
	{
		AR2D_ASSERT(min < max, "Invalid range passed to Random::RandomFloat");
		std::uniform_real_distribution<FloatType> uniDest{min, max};
		return uniDest(s_Engine);
	}

	/**
	 * @return a random integer in [min, max].
	*/
	template <std::integral IntType = std::int32_t>
	static IntType RandomInt(
		IntType min = std::numeric_limits<IntType>::min(),
		IntType max = std::numeric_limits<IntType>::max()) noexcept
	{
		AR2D_ASSERT(min < max, "Invalid range passed to Random::RandomInt");
		std::uniform_int_distribution<IntType> uniDest{min, max};
		return uniDest(s_Engine);
	}

	/**
	 * @return a vector with components with random values in [0, 1].
	*/
	static Vec2 RandomVec2() noexcept;

	/**
	 * @return a vector in the rectangle bounded by 'min' and 'max' inclusively.
	*/
	static Vec2 RandomVec2(Vec2 min, Vec2 max) noexcept;

	/**
	 * @return a completely random color with a alpha value of 1.
	*/
	static ColorF RandomColorF() noexcept;

	/**
	 * @return a completely random color with a random alpha value as well.
	*/
	static ColorF RandomColorFWithAlpha() noexcept;
};

AR2D_END_NAMESPACE