#ifndef TIMEOUT_H
#define TIMEOUT_H

#include <iostream>
#include <future>
#include <chrono>

class Timeout
{
public:
    typedef std::chrono::steady_clock clock_type;
    typedef clock_type::time_point time_point;
    typedef clock_type::duration duration;

    explicit Timeout(duration maxDuration) :
    mStartTime(clock_type::now()),
    mMaxDuration(maxDuration)
    {}

    time_point startTime() const;

    duration getMaxDuration() const;

    void setMaxDuration(duration);

    void setTimerToZero();

    bool isExpired() const;


private:
    time_point mStartTime;
    duration mMaxDuration;
};

#endif // TIMEOUT_H

