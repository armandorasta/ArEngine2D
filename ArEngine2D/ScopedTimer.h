#pragma once

#include "Timer.h"
#include <functional>

AR2D_BEGIN_NAMESPACE

template <class TimeUnit = std::chrono::microseconds>
class ScopedTimer
{
public:
	
	using CallBack = std::function<void(Timer::Duration<TimeUnit> const&)>;
	using Duration = Timer::Duration<TimeUnit>;

public:
	
	ScopedTimer() = default;
	ScopedTimer(Duration* target)
		: target_{target}
	{ 
		AR2D_ASSERT(target, "Invalid use of constructor, use the default constructor instead");
	}
	ScopedTimer(CallBack deathCallback)
		: callbackFunc_{std::move(deathCallback)}
	{ 
		AR2D_ASSERT(callbackFunc_, "Invalid use of constructor, use the default constructor instead");
	}
	~ScopedTimer()
	{
		auto const time{timer_.Mark()};
		if (target_)
		{
			*target_ = time;
		}
		if (callbackFunc_)
		{
			callbackFunc_(time);
		}
	}

public:

	void SetTarget(Duration* target)
	{
		AR2D_ASSERT(target, "Tried to set ScopedTimer target to nullptr");
		target_ = target;
	}

	void SetDeathCallBack(CallBack callback)
	{
		AR2D_ASSERT(callback, "Tried to set ScopedTimer death call back to empty function");
		callbackFunc_ = std::move(callbackFunc_);
	}

private:
	Timer timer_;
	Timer::Duration<TimeUnit>* target_{};
	CallBack callbackFunc_{};
};

AR2D_END_NAMESPACE