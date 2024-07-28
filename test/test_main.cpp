/** 
 * @author: pikaqiu_zcj
 * @date: 2024-06-23
 * @par lastEditors: pikaqiu_zcj
 * @par lastEditTime: 2024-08-04
 * @file: \test\test_main.cpp
 * @message: 
 * @copyright (c) 2024 by pikaqiu, All Rights Reserved. 
 */

#include "gtest/gtest.h"

int main(int argc, char **argv)
{
    // 初始化 Google Test 框架
    ::testing::InitGoogleTest(&argc, argv);
    // 运行所有测试
    return RUN_ALL_TESTS();
}
