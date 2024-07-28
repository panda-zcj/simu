# 查找 GTest
find_package(GTest REQUIRED PATHS "${CMAKE_SOURCE_DIR}/lib/cmake")
# 输出信息以验证是否成功找到 GTest
message(STATUS "GTest found: ${GTest_FOUND}")

# 添加测试文件
file(GLOB_RECURSE SRC_TEST test*.cpp test*.cc test*.cxx)
add_executable(simu_test ${SRC_TEST})

# 链接 Google Test 主库
target_link_libraries(simu_test GTest::gtest GTest::gtest_main)
# 链接需要测试的代码库
target_link_libraries(simu_test simulator_static)
# 设置运行路径
set_target_properties(simu_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# 添加测试到 CTest
include(GoogleTest)
gtest_discover_tests(simu_test)