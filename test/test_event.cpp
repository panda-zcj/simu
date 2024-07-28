/**
 * @author: pikaqiu_zcj
 * @date: 2024-08-04
 * @par lastEditors: pikaqiu_zcj
 * @par lastEditTime: 2024-08-04
 * @file: \test\test_json.cpp
 * @message:
 * @copyright (c) 2024 by pikaqiu, All Rights Reserved.
 */

#include "event.h"
#include "times.h"
#include "log.h"
#include "gtest/gtest.h"

using namespace simu;

nlohmann::ordered_json nodeTemplete = {
    {"nodeID", ""},
    {"type", ""},
    {"description", ""},
    {"currentTime", 0},
    {"nodeState", ""},
    {"speed", {1, 1}},
    {"location", {0, 0}},
    {"energy", 0},
    {"power", 0},
    {"coverageArea", 1500},
    {"statistic", {{"throughput", 0}, {"averageDelay", 0}, {"sendEventNums", 0}, {"reciveEventNums", 0}}},
    {"events", {{"send", nlohmann::ordered_json::object()}, {"receive", {{"selfReceiverEventID", nlohmann::ordered_json::array()}, {"otherReceiverEventID", nlohmann::ordered_json::array()}, {"detail", nlohmann::ordered_json::object()}}}}}};

nlohmann::ordered_json nodeMsgTemplete = {
    {"nodeID", ""},
    {"type", ""},
    {"description", ""},
    {"currentTime", 0},
    {"nodeState", ""},
    {"speed", {1, 1}},
    {"location", {0, 0}},
    {"energy", 0},
    {"power", 0},
    {"coverageArea", 1500},
    {"event", nlohmann::ordered_json::object()}};

class JsonSuite : public ::testing::Test
{
protected:
    static void SetUpTestSuite()
    {
        nanoLogInit("test", ".\\", nanolog::LogLevel::CRIT);
        std::cout << std::endl;
    }

    static void TearDownTestSuite()
    {
    }

    void SetUp() override
    {
        _global_time_.setTime(0);
    }

    void TearDown() override
    {
    }
};

TEST_F(JsonSuite, event_override_double)
{
    double duration = 10;
    double maxPropagateTime = 0;
    std::string content = "this a double test";
    std::string srcId = "0x1";
    std::string dstId = "0x2";
    nlohmann::ordered_json event = Event::getEvent(duration, maxPropagateTime, content, srcId, dstId);

    std::cout << event.dump(4) << std::endl;

    EXPECT_EQ(dstId, event["destNodeID"]);
    EXPECT_EQ(srcId, event["srcNodeID"]);
    EXPECT_EQ(_global_time_.getTimeStamp(), event["startTime"]);
    EXPECT_EQ(_global_time_.getTimeStamp() + (long long)(duration) * 1000000, event["endTime"]);
    EXPECT_EQ(_global_time_.getTimeStamp() + (long long)(duration) * 1000000, event["expiredTime"]);
    EXPECT_EQ(0, event["responsedTime"]);
    EXPECT_EQ("0x0001000200000000", event["eventID"]);
    EXPECT_EQ("", event["srcNodeDescription"]);
    EXPECT_EQ("", event["destNodeDescription"]);
    EXPECT_EQ(eventState[static_cast<int>(EventState::EventInit)], event["eventState"]);
    EXPECT_EQ(content.size(), event["msg"]["length"]);
    EXPECT_EQ(content, event["msg"]["payload"]);
}

TEST_F(JsonSuite, event_override_long)
{
    long long duration = 20;
    long long maxPropagateTime = 0;
    std::string content = "this a long long test";
    std::string srcId = "0x3";
    std::string dstId = "0x4";
    nlohmann::ordered_json event = Event::getEvent(duration, maxPropagateTime, content, srcId, dstId);

    std::cout << event.dump(4) << std::endl;

    EXPECT_EQ(dstId, event["destNodeID"]);
    EXPECT_EQ(srcId, event["srcNodeID"]);
    EXPECT_EQ(_global_time_.getTimeStamp(), event["startTime"]);
    EXPECT_EQ(_global_time_.getTimeStamp() + (long long)(duration) * 1000000, event["endTime"]);
    EXPECT_EQ(_global_time_.getTimeStamp() + (long long)(duration) * 1000000, event["expiredTime"]);
    EXPECT_EQ(0, event["responsedTime"]);
    EXPECT_EQ("0x0003000400000001", event["eventID"]);
    EXPECT_EQ("", event["srcNodeDescription"]);
    EXPECT_EQ("", event["destNodeDescription"]);
    EXPECT_EQ(eventState[static_cast<int>(EventState::EventInit)], event["eventState"]);
    EXPECT_EQ(content.size(), event["msg"]["length"]);
    EXPECT_EQ(content, event["msg"]["payload"]);
}

TEST_F(JsonSuite, event_override_int)
{
    int duration = 30;
    int maxPropagateTime = 0;
    std::string content = "this a int test";
    std::string srcId = "0x5";
    std::string dstId = "0x6";
    nlohmann::ordered_json event = Event::getEvent(duration, maxPropagateTime, content, srcId, dstId);

    std::cout << event.dump(4) << std::endl;

    EXPECT_EQ(dstId, event["destNodeID"]);
    EXPECT_EQ(srcId, event["srcNodeID"]);
    EXPECT_EQ(_global_time_.getTimeStamp(), event["startTime"]);
    EXPECT_EQ(_global_time_.getTimeStamp() + (long long)(duration) * 1000000, event["endTime"]);
    EXPECT_EQ(_global_time_.getTimeStamp() + (long long)(duration) * 1000000, event["expiredTime"]);
    EXPECT_EQ(0, event["responsedTime"]);
    EXPECT_EQ("0x0005000600000002", event["eventID"]);
    EXPECT_EQ("", event["srcNodeDescription"]);
    EXPECT_EQ("", event["destNodeDescription"]);
    EXPECT_EQ(eventState[static_cast<int>(EventState::EventInit)], event["eventState"]);
    EXPECT_EQ(content.size(), event["msg"]["length"]);
    EXPECT_EQ(content, event["msg"]["payload"]);
}

TEST_F(JsonSuite, node)
{
    long long duration = 40;
    long long maxPropagateTime = 0;
    std::string content = "this a node event";
    std::string srcId = "0x7";
    std::string dstId = "0x8";
    nlohmann::ordered_json event = Event::getEvent(duration, maxPropagateTime, content, srcId, dstId);

    nlohmann::ordered_json node(nodeTemplete);
    node["events"]["send"] = event;

    std::cout << node.dump(4) << std::endl;

    EXPECT_EQ(dstId, node["events"]["send"]["destNodeID"]);
    EXPECT_EQ(srcId, node["events"]["send"]["srcNodeID"]);
    EXPECT_EQ(_global_time_.getTimeStamp(), node["events"]["send"]["startTime"]);
    EXPECT_EQ(_global_time_.getTimeStamp() + (long long)(duration) * 1000000, node["events"]["send"]["endTime"]);
    EXPECT_EQ(_global_time_.getTimeStamp() + (long long)(duration) * 1000000, node["events"]["send"]["expiredTime"]);
    EXPECT_EQ(0, node["events"]["send"]["responsedTime"]);
    EXPECT_EQ("0x0007000800000003", node["events"]["send"]["eventID"]);
    EXPECT_EQ("", node["events"]["send"]["srcNodeDescription"]);
    EXPECT_EQ("", node["events"]["send"]["destNodeDescription"]);
    EXPECT_EQ(eventState[static_cast<int>(EventState::EventInit)], node["events"]["send"]["eventState"]);
    EXPECT_EQ(content.size(), node["events"]["send"]["msg"]["length"]);
    EXPECT_EQ(content, node["events"]["send"]["msg"]["payload"]);
}

TEST_F(JsonSuite, nodeMsg)
{
    long long duration = 50;
    long long maxPropagateTime = 0;
    std::string content = "this a node msg";
    std::string srcId = "0x9";
    std::string dstId = "0xa";
    nlohmann::ordered_json event = Event::getEvent(duration, maxPropagateTime, content, srcId, dstId);

    nlohmann::ordered_json node(nodeMsgTemplete);
    node["events"]["send"] = event;

    std::cout << node.dump(4) << std::endl;

    EXPECT_EQ(dstId, node["events"]["send"]["destNodeID"]);
    EXPECT_EQ(srcId, node["events"]["send"]["srcNodeID"]);
    EXPECT_EQ(_global_time_.getTimeStamp(), node["events"]["send"]["startTime"]);
    EXPECT_EQ(_global_time_.getTimeStamp() + (long long)(duration) * 1000000, node["events"]["send"]["endTime"]);
    EXPECT_EQ(_global_time_.getTimeStamp() + (long long)(duration) * 1000000, node["events"]["send"]["expiredTime"]);
    EXPECT_EQ(0, node["events"]["send"]["responsedTime"]);
    EXPECT_EQ("0x0009000a00000004", node["events"]["send"]["eventID"]);
    EXPECT_EQ("", node["events"]["send"]["srcNodeDescription"]);
    EXPECT_EQ("", node["events"]["send"]["destNodeDescription"]);
    EXPECT_EQ(eventState[static_cast<int>(EventState::EventInit)], node["events"]["send"]["eventState"]);
    EXPECT_EQ(content.size(), node["events"]["send"]["msg"]["length"]);
    EXPECT_EQ(content, node["events"]["send"]["msg"]["payload"]);
}

TEST_F(JsonSuite, nodeReceive)
{
    long long duration = 60;
    long long maxPropagateTime = 0;
    std::string content = "this a node msg";
    std::string srcId = "0xb";
    std::string dstId = "0xc";
    nlohmann::ordered_json event = Event::getEvent(duration, maxPropagateTime, content, srcId, dstId);

    nlohmann::ordered_json node(nodeTemplete);
    node["events"]["receive"]["selfReceiverEventID"].push_back(event["eventID"]);
    node["events"]["receive"]["detail"][event["eventID"]] = event;

    std::cout << node.dump(4) << std::endl;

    EXPECT_EQ(dstId, node["events"]["receive"]["detail"][event["eventID"]]["destNodeID"]);
    EXPECT_EQ(srcId, node["events"]["receive"]["detail"][event["eventID"]]["srcNodeID"]);
    EXPECT_EQ(_global_time_.getTimeStamp(), node["events"]["receive"]["detail"][event["eventID"]]["startTime"]);
    EXPECT_EQ(_global_time_.getTimeStamp() + (long long)(duration) * 1000000, node["events"]["receive"]["detail"][event["eventID"]]["endTime"]);
    EXPECT_EQ(_global_time_.getTimeStamp() + (long long)(duration) * 1000000, node["events"]["receive"]["detail"][event["eventID"]]["expiredTime"]);
    EXPECT_EQ(0, node["events"]["receive"]["detail"][event["eventID"]]["responsedTime"]);
    EXPECT_EQ("0x000b000c00000005", node["events"]["receive"]["detail"][event["eventID"]]["eventID"]);
    EXPECT_EQ("", node["events"]["receive"]["detail"][event["eventID"]]["srcNodeDescription"]);
    EXPECT_EQ("", node["events"]["receive"]["detail"][event["eventID"]]["destNodeDescription"]);
    EXPECT_EQ(eventState[static_cast<int>(EventState::EventInit)], node["events"]["receive"]["detail"][event["eventID"]]["eventState"]);
    EXPECT_EQ(content.size(), node["events"]["receive"]["detail"][event["eventID"]]["msg"]["length"]);
    EXPECT_EQ(content, node["events"]["receive"]["detail"][event["eventID"]]["msg"]["payload"]);
}
