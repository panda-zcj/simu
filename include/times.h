/** 
 * @author: pikaqiu_zcj
 * @date: 2022-12-18
 * @par lastEditors: pikaqiu_zcj
 * @par lastEditTime: 2024-08-01
 * @file: \include\times.h
 * @message: 
 * @copyright (c) 2024 by pikaqiu, All Rights Reserved. 
 */

#ifndef TIME_H
#define TIME_H

#include "global.h"
#include <string>
#include <chrono>

namespace simu
{
    class Time
    {
    private:
        /* data */
        std::chrono::high_resolution_clock::time_point timePoint;
        std::tm start_time;

    public:
        /**
         * @brief Construct a new Time object
         * @param[in] curTimeOffset the offset of The current time point, the precision is the Millisecond
         */
        Time(long long curTimeOffset = 0);
        /**
         * @brief Construct a new Time object
         * @param[in] curTimeOffset the offset of The current time point, the precision is the Millisecond
         */
        Time(int curTimeOffset = 0);
        /**
         * @brief Construct a new Time object
         * @param[in] curTimeOffset the offset of The current time point, the precision is the Millisecond
         */
        Time(double curTimeOffset = 0.0);
        ~Time();

    public:
        /**
         * @brief Get the Time Stamp, the precision is the nanoseconds
         * @return std::chrono::nanoseconds::rep
         */
        std::chrono::nanoseconds::rep getTimeStamp() const
        {
            return std::chrono::time_point_cast<std::chrono::nanoseconds>(timePoint).time_since_epoch().count();
        }
        /**
         * @brief Set the Time object
         * @param[in] curTimeOffset the offset of The current time point, the precision is the Millisecond
         */
        void setTime(double curTimeOffset = 0.0);
        /**
         * @brief format current time object
         * @return std::string  the string of format current time object
         */
        std::string printCurTime();

    public:
        /**
         * @brief The current time adds a delay
         * @param[in] after delay time, expressed in ms
         * @return Time&
         */
        Time &operator+(const long long after);
        /**
         * @brief The current time adds a delay
         * @param[in] after delay time, expressed in ms
         * @return Time&
         */
        Time &operator+(const int after);
        /**
         * @brief The current time adds a delay
         * @param[in] after delay time, expressed in ms
         * @return Time&
         */
        Time &operator+(const double after);
        /**
         * @brief The current time adds a delay
         * @param[in] after delay time, expressed in ms
         * @return Time&
         */
        Time &operator+=(const long long after);
        /**
         * @brief The current time adds a delay
         * @param[in] after delay time, expressed in ms
         * @return Time&
         */
        Time &operator+=(const int after);
        /**
         * @brief The current time adds a delay
         * @param[in] after delay time, expressed in ms
         * @return Time&
         */
        Time &operator+=(const double after);
        /**
         * @brief return True if less than, False if no
         * @param[in] cmp Time object to be compared
         * @return true
         * @return false
         */
        bool operator<(const Time &cmp);
        /**
         * @brief Return True if large than, False if no
         * @param[in] cmp Time object to be compared
         * @return true
         * @return false
         */
        bool operator>(const Time &cmp);
        /**
         * @brief Return True if equal, False if no
         * @param[in] cmp Time object to be compared
         * @return true
         * @return false
         */
        bool operator==(const Time &cmp);
        /**
         * @brief Return True if less than or equal, False if not
         * @param[in] cmp Time object to be compared
         * @return true
         * @return false
         */
        bool operator<=(const Time &cmp);
        /**
         * @brief Return True if greater than or equal to, False if not
         * @param[in] cmp  Time object to be compared
         * @return true
         * @return false
         */
        bool operator>=(const Time &cmp);
    };

    extern Time _global_time_;

} // namespace simu

#endif // TIME_H