/** 
 * @author: pikaqiu_zcj
 * @date: 2022-12-18
 * @par lastEditors: pikaqiu_zcj
 * @par lastEditTime: 2024-08-01
 * @file: \src\time\times.cpp
 * @message: 
 * @copyright (c) 2024 by pikaqiu, All Rights Reserved. 
 */

#include "times.h"
#include <iostream>
#include <sstream>
#include <iomanip>
using namespace simu;

Time::Time(long long curTimeOffset)
{
    std::chrono::milliseconds offset(curTimeOffset);

    timePoint = std::chrono::high_resolution_clock::time_point(std::chrono::high_resolution_clock::duration(0));

    // 添加偏移量
    timePoint += offset;
}

Time::Time(int curTimeOffset)
{
    std::chrono::milliseconds offset(curTimeOffset);

    timePoint = std::chrono::high_resolution_clock::time_point(std::chrono::high_resolution_clock::duration(0));

    // 添加偏移量
    timePoint += offset;
}

Time::Time(double curTimeOffset)
{
    std::chrono::duration<double, std::ratio<1, 1000>> offset(curTimeOffset);

    timePoint = std::chrono::high_resolution_clock::time_point(std::chrono::high_resolution_clock::duration(0));

    // 添加偏移量
    timePoint += std::chrono::duration_cast<std::chrono::high_resolution_clock::duration>(offset);
}

Time::~Time()
{
}

Time &Time::operator+(const double after)
{
    std::chrono::duration<double, std::ratio<1, 1000>> offset(after);
    timePoint += std::chrono::duration_cast<std::chrono::high_resolution_clock::duration>(offset);
    return *this;
}

Time &Time::operator+(const int after)
{
    std::chrono::milliseconds offset(after);
    timePoint += offset;
    return *this;
}

Time &Time::operator+(const long long after)
{
    std::chrono::milliseconds offset(after);
    timePoint += offset;
    return *this;
}

Time &Time::operator+=(const double after)
{
    std::chrono::duration<double, std::ratio<1, 1000>> offset(after);
    timePoint += std::chrono::duration_cast<std::chrono::high_resolution_clock::duration>(offset);
    return *this;
}

Time &Time::operator+=(const int after)
{
    std::chrono::milliseconds offset(after);
    timePoint += offset;
    return *this;
}

Time &Time::operator+=(const long long after)
{
    std::chrono::milliseconds offset(after);
    timePoint += offset;
    return *this;
}

bool Time::operator<(const Time &cmp)
{
    return timePoint < cmp.timePoint;
}

bool Time::operator>(const Time &cmp)
{
    return timePoint > cmp.timePoint;
}

bool Time::operator==(const Time &cmp)
{
    return timePoint == cmp.timePoint;
}

bool Time::operator<=(const Time &cmp)
{
    return timePoint <= cmp.timePoint;
}

bool Time::operator>=(const Time &cmp)
{
    return timePoint >= cmp.timePoint;
}

void Time::setTime(double curTimeOffset)
{
    std::chrono::duration<double, std::ratio<1, 1000>> offset(curTimeOffset);
    timePoint = std::chrono::high_resolution_clock::time_point(std::chrono::duration_cast<std::chrono::high_resolution_clock::duration>(offset));
}

std::string Time::printCurTime()
{
    // 将时间点转换为时间戳（纳秒精度）
    auto nano_seconds = std::chrono::time_point_cast<std::chrono::nanoseconds>(timePoint);
    auto timestamp = nano_seconds.time_since_epoch().count();

    std::time_t time_t = timestamp / 1000000000;
    auto gmtime = std::gmtime(&time_t);
    char buffer[32] = {0};
    strftime(buffer, 32, "%H:%M:%S.", gmtime);
    char nanoseconds[12] = {0};
#ifdef _WIN32
    sprintf(nanoseconds, "%09llu", timestamp % 1000000000);
#else
    sprintf(nanoseconds, "%09lu", timestamp % 1000000000);
#endif

    // 创建一个 stringstream 对象用于存储输出
    std::ostringstream oss;

    // 输出时间部分（小时、分钟、秒）
    oss << '[' << buffer << nanoseconds << ']';

    return oss.str();
}

Time simu::_global_time_(0LL);