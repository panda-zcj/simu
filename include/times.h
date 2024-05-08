/*** 
 * @Author: pikaqiu_zcj
 * @Date: 2022-12-18
 * @LastEditors: pikaqiu_zcj
 * @LastEditTime: 2024-04-14
 * @FilePath: \include\times.h
 * @Description: 
 * @Copyright (c) 2024 by pikaqiu, All Rights Reserved. 
 */

#ifndef TIME_H
#define TIME_H

namespace simu
{
// 时间戳精度为1ns
#define TIMESTAMP_PRECISION (1000)
// 最大时间范围
#define MAX_USEC (1000)
#define MAX_MSEC (1000)
#define MAX_SEC (60)
#define MAX_MIN (60)
#define MAX_HOUR (24)

    class Time
    {
    private:
        /* data */
        unsigned long timeStamp;

    public:
        Time(unsigned long curTimeStamp = 0);
        ~Time();

    public:
        int getDay() const
        {
            return timeStamp / TIMESTAMP_PRECISION / MAX_USEC / MAX_MSEC / MAX_SEC / MAX_MIN / MAX_HOUR;
        }
        int getHour() const
        {
            return timeStamp / TIMESTAMP_PRECISION / MAX_USEC / MAX_MSEC / MAX_SEC / MAX_MIN % MAX_HOUR;
        }
        int getMin() const
        {
            return timeStamp / TIMESTAMP_PRECISION / MAX_USEC / MAX_MSEC / MAX_SEC % MAX_MIN;
        }
        int getSec() const
        {
            return timeStamp / TIMESTAMP_PRECISION / MAX_USEC / MAX_MSEC % MAX_SEC;
        }
        int getMsec() const
        {
            return timeStamp / TIMESTAMP_PRECISION / MAX_USEC % MAX_MSEC;
        }
        int getUsec() const
        {
            return timeStamp / TIMESTAMP_PRECISION % MAX_USEC;
        }
        unsigned long getTime() const
        {
            return timeStamp;
        }
        void setTime(unsigned long curTimeStamp = 0)
        {
            timeStamp = curTimeStamp;
        }
        void printCurTime();

    public:
        /**
         * @brief 返回延后的时间戳
         *
         * @param after 延后时间，单位为ms
         * @return Time&
         */       
        Time &operator+(const double after);
        /**
         * @brief 返回延后的时间戳
         *
         * @param after 延后时间，单位为ms
         * @return Time&
         */
        Time &operator+(const unsigned long after);
        /**
         * @brief 
         * 
         * @param cmp 
         * @return true 
         * @return false 
         */
        bool operator<(const Time &cmp);
        bool operator>(const Time &cmp);
        bool operator==(const Time &cmp);
        bool operator<=(const Time &cmp);
        bool operator>=(const Time &cmp);
    };

} // namespace simu

#endif // TIME_H