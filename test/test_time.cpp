/** 
 * @author: pikaqiu_zcj
 * @date: 2024-07-31
 * @par lastEditors: pikaqiu_zcj
 * @par lastEditTime: 2024-08-04
 * @file: \test\test_time.cpp
 * @message: 
 * @copyright (c) 2024 by pikaqiu, All Rights Reserved. 
 */

#include "times.h"
#include "gtest/gtest.h"
using namespace simu;

TEST(Time, operate)
{
    EXPECT_EQ("[00:00:00.000000000]", _global_time_.printCurTime());
    _global_time_ = _global_time_ + 800.0;
    EXPECT_EQ("[00:00:00.800000000]", _global_time_.printCurTime());
    _global_time_ = _global_time_ + 700LL;
    EXPECT_EQ("[00:00:01.500000000]", _global_time_.printCurTime());
    _global_time_ = _global_time_ + 600;
    EXPECT_EQ("[00:00:02.100000000]", _global_time_.printCurTime());
    _global_time_ += 800.0;
    EXPECT_EQ("[00:00:02.900000000]", _global_time_.printCurTime());
    _global_time_ += 700LL;
    EXPECT_EQ("[00:00:03.600000000]", _global_time_.printCurTime());
    _global_time_ += 600;
    EXPECT_EQ("[00:00:04.200000000]", _global_time_.printCurTime());
    _global_time_.setTime(900.0);
    EXPECT_EQ("[00:00:00.900000000]", _global_time_.printCurTime());
}

TEST(Time, compare)
{
    _global_time_.setTime(900.0);
    EXPECT_EQ("[00:00:00.900000000]", _global_time_.printCurTime());
    Time cmp(100);
    EXPECT_TRUE(_global_time_ > cmp);
    EXPECT_TRUE(_global_time_ >= cmp);
    EXPECT_FALSE(_global_time_ < cmp);
    EXPECT_FALSE(_global_time_ <= cmp);
    EXPECT_FALSE(_global_time_ == cmp);
}