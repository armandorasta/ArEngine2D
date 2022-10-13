#pragma once

#include "EngineCore.h"	

#include <chrono>
#include <vector>

AR2D_BEGIN_NAMESPACE

class Timer
{
public:
	using Float     = float;
	using Clock     = std::chrono::steady_clock;
	using TimePoint = Clock::time_point;

	template <class Unit = std::chrono::seconds>
	using Duration  = std::chrono::duration<Float, typename Unit::period>;

public:

	static TimePoint Now() noexcept;

public:

	Timer();
	constexpr Timer(Timer const&)	                = default;
	constexpr Timer(Timer&&) noexcept               = default;
	constexpr Timer& operator=(Timer const&)        = default;
	constexpr Timer& operator=(Timer&&) noexcept	= default;
	~Timer();

public:

	/**
	 * @brief Resets the timer, calling Peek right after this will basically return an instant.
	*/
	void Reset() noexcept;

	/**
	 * @brief Intended to be called once after construction to mark the start of the timer, 
	 *        but not needed because: timer already starts on construction.
	*/
	void Start() noexcept;

	/**
	 * @brief Pauses the timer if it's ticking, otherwise does nothing. Peek member function 
	 *        will return this current time point until the timer is resumed and paused again.
	*/
	void Pause() noexcept;

	/**
	 * @brief Starts the timer if it's paused, pauses it otherwise.
	*/
	void Toggle() noexcept;

	/**
	 * @return Resumes the time if it was paused, otherwise does nothing.
	*/
	void Resume() noexcept;

	/**
	 * @brief Acts the same as Peek if the timer is paused, otherwise marks the current point 
	 *        in time and returns the duration since the last time the timer was resumed\started.
	 * @tparam Unit => Time unit the duration will be in.
	 * @return returns the duration since the last time the timer was resumed\started.
	*/
	template <class Unit = std::chrono::seconds>
	Duration<Unit> Mark() noexcept
	{
		if (!IsPaused())
		{
			tp1_ = Now();
		}
		return Peek<Unit>();
	}

	/**
	 * @brief This does not mutate the state of the timer.
	 * @tparam Unit => Time unit the duration will be in.
	 * @return (Last pause) - (Last resume).
	*/
	template <class Unit = std::chrono::seconds>
	Duration<Unit> Peek() const noexcept
	{
		return Duration<Unit>{tp1_ - tp0_};
	}
	
	/**
	 * @tparam Unit => Time unit the duration will be in.
	 * @return Duration between now and last call to Start.
	*/
	template <class Unit = std::chrono::seconds>
	Duration<Unit> TimeSinceStart() const noexcept
	{
		return Duration<Unit>{Now() - tpStart_};
	}
	
	/**
	 * @return true if the timer is currently paused, otherwise returns false.
	*/
	bool IsPaused() const noexcept;

private:
	bool bStopped_{true};
	TimePoint tpStart_;
	TimePoint tp0_{Now()};
	TimePoint tp1_{Now()};
};

AR2D_END_NAMESPACE