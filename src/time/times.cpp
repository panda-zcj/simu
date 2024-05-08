/*
 * File: time.cpp
 * Created Date: Sunday, December 18th 2022
 * Author: pikaqiu
 * -----
 * Last Modified: Mon Dec 19 2022
 * Modified By: pikaqiu
 * -----
 * Copyright (c) 2022 Dream
 *
 * All shall be well and all shall be well and all manner of things shall be well.
 * Nope...we're doomed!
 */

#include "times.h"
#include <string>
#include <iostream>
using namespace simu;

Time::Time(unsigned long curTimeStamp)
{
    this->timeStamp = curTimeStamp;
}

Time::~Time()
{
}

Time &Time::operator+(const double after)
{

    this->timeStamp += (unsigned long)(after * TIMESTAMP_PRECISION * MAX_USEC);
    return *this;
}

Time &Time::operator+(const unsigned long after)
{

    this->timeStamp += after * TIMESTAMP_PRECISION * MAX_USEC;
    return *this;
}

bool Time::operator<(const Time &cmp)
{
    return timeStamp < cmp.timeStamp;
}

bool Time::operator>(const Time &cmp)
{
    return timeStamp > cmp.timeStamp;
}

bool Time::operator==(const Time &cmp)
{
    return timeStamp == cmp.timeStamp;
}

bool Time::operator<=(const Time &cmp)
{
    return timeStamp <= cmp.timeStamp;
}

bool Time::operator>=(const Time &cmp)
{
    return timeStamp >= cmp.timeStamp;
}

void Time::printCurTime()
{
    std::string str;
    sprintf(str.data(), "[%dd-%2dh-%2dm-%2ds-%3dms-%3dus]:", getDay(),
            getHour(), getMin(), getSec(), getMsec(), getUsec());
    std::cout << str;
}
