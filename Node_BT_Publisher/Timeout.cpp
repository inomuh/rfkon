#include "Timeout.h"

std::chrono::steady_clock::time_point Timeout::startTime() const
{
    return mStartTime;
}

std::chrono::steady_clock::duration Timeout::getMaxDuration() const
{
    return mMaxDuration;
}

void Timeout::setMaxDuration(std::chrono::steady_clock::duration d)
{
    mMaxDuration = d;
}

void Timeout::setTimerToZero()
{
    Timeout::mStartTime = clock_type::now();
}

bool Timeout::isExpired() const
{
    const auto endTime = clock_type::now();

    return (endTime - startTime()) > getMaxDuration();
}
