#include "Timer.h"

AR2D_BEGIN_NAMESPACE

#include <stdexcept>

Timer::TimePoint Timer::Now() noexcept
{
	return Clock::now();
}

Timer::Timer()
{ 
	Start();
}

Timer::~Timer()
{ 
	Pause();
}

void Timer::Reset() noexcept
{ 
	tp1_ = tp0_ = Now();
	bStopped_ = true;
}

void Timer::Start() noexcept
{ 
	Reset();
	Resume();
	tpStart_ = Now();
}

void Timer::Pause() noexcept
{ 
	if (!IsPaused())
	{
		tp1_ = Now();
		bStopped_ = true;
	}
}

void Timer::Toggle() noexcept
{ 
	if (IsPaused())
	{
		Resume();
	}
	else
	{
		Pause();
	}
}

void Timer::Resume() noexcept
{ 
	bStopped_ = false;
}

bool Timer::IsPaused() const noexcept
{
	return bStopped_;
}

AR2D_END_NAMESPACE


