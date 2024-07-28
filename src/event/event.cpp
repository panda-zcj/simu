/**
 * @author: pikaqiu_zcj
 * @date: 2022-08-07
 * @LastEditors: pikaqiu_zcj
 * @LastEditTime: 2024-06-23
 * @file: \src\event\event.cpp
 * @message:
 * @Copyright (c) 2024 by pikaqiu, All Rights Reserved.
 */

#include "event.h"
#include "log.h"
#include "times.h"
#include <sstream>
#include <iomanip>
using namespace simu;

const std::string simu::eventState[] = {
    "Event Init",
    "Event Triggered",
    "Event Response In Progress",
    "Event Conflict",
    "Event No Response",
    "Event Response Completed",
};

nlohmann::ordered_json eventTemplete = {
    {"eventID", ""},
    {"startTime", 0},
    {"endTime", 0},
    {"expiredTime", 0},
    {"responsedTime", 0},
    {"srcNodeID", ""},
    {"srcNodeDescription", ""},
    {"destNodeID", ""},
    {"destNodeDescription", ""},
    {"msg", {{"length", 0}, {"acknowledgedSize", 0}, {"payload", ""}}},
    {"eventState", ""},
};

Event::Event(/* args */)
{
}

Event::~Event()
{
}

std::string Event::getHexStr(unsigned long long num)
{
    std::stringstream ss;
    ss << "0x" << std::setw(16) << std::setfill('0') << std::hex << num;
    return ss.str();
}

nlohmann::ordered_json Event::getEvent(long long duration,
                                       long long maxPropagateTime,
                                       std::string &content,
                                       std::string &srcId,
                                       std::string &dstId,
                                       std::string srcStr,
                                       std::string dstStr)
{
    nlohmann::ordered_json event(eventTemplete);
    unsigned long long src_id = std::stoi(srcId, nullptr, 16);
    unsigned long long dst_id = std::stoi(dstId, nullptr, 16);
    Time start = simu::_global_time_;
    event["startTime"] = start.getTimeStamp();
    start += duration;
    event["endTime"] = start.getTimeStamp();
    start += maxPropagateTime;
    event["expiredTime"] = start.getTimeStamp();
    event["eventID"] = getHexStr((((SIMULATION_NODE_ID_MASK(src_id) << 16) | SIMULATION_NODE_ID_MASK(SIMULATION_NODE_ID_MASK(dst_id))) << 32) | (__g_eventID__++));
    event["srcNodeID"] = srcId;
    event["srcNodeDescription"] = srcStr;
    event["destNodeID"] = dstId;
    event["destNodeDescription"] = dstStr;
    event["eventState"] = eventState[static_cast<int>(EventState::EventInit)];
    event["msg"]["length"] = content.size();
    event["msg"]["payload"] = content;

    LOG_DEBUG << event.dump(4);
    return event;
}

nlohmann::ordered_json Event::getEvent(int duration,
                                       int maxPropagateTime,
                                       std::string &content,
                                       std::string &srcId,
                                       std::string &dstId,
                                       std::string srcStr,
                                       std::string dstStr)
{
    nlohmann::ordered_json event(eventTemplete);
    unsigned long long src_id = std::stoi(srcId, nullptr, 16);
    unsigned long long dst_id = std::stoi(dstId, nullptr, 16);
    Time start = simu::_global_time_;
    event["startTime"] = start.getTimeStamp();
    start += duration;
    event["endTime"] = start.getTimeStamp();
    start += maxPropagateTime;
    event["expiredTime"] = start.getTimeStamp();
    event["eventID"] = getHexStr((((SIMULATION_NODE_ID_MASK(src_id) << 16) | SIMULATION_NODE_ID_MASK(dst_id)) << 32) | (__g_eventID__++));
    event["srcNodeID"] = srcId;
    event["srcNodeDescription"] = srcStr;
    event["destNodeID"] = dstId;
    event["destNodeDescription"] = dstStr;
    event["eventState"] = eventState[static_cast<int>(EventState::EventInit)];
    event["msg"]["length"] = content.size();
    event["msg"]["payload"] = content;

    LOG_DEBUG << event.dump(4);
    return event;
}

nlohmann::ordered_json Event::getEvent(double duration,
                                       double maxPropagateTime,
                                       std::string &content,
                                       std::string &srcId,
                                       std::string &dstId,
                                       std::string srcStr,
                                       std::string dstStr)
{
    nlohmann::ordered_json event(eventTemplete);
    unsigned long long src_id = std::stoi(srcId, nullptr, 16);
    unsigned long long dst_id = std::stoi(dstId, nullptr, 16);
    Time start = simu::_global_time_;
    event["startTime"] = start.getTimeStamp();
    start += duration;
    event["endTime"] = start.getTimeStamp();
    start += maxPropagateTime;
    event["expiredTime"] = start.getTimeStamp();
    event["eventID"] = getHexStr((((SIMULATION_NODE_ID_MASK(src_id) << 16) | SIMULATION_NODE_ID_MASK(dst_id)) << 32) | (__g_eventID__++));
    event["srcNodeID"] = srcId;
    event["srcNodeDescription"] = srcStr;
    event["destNodeID"] = dstId;
    event["destNodeDescription"] = dstStr;
    event["eventState"] = eventState[static_cast<int>(EventState::EventInit)];
    event["msg"]["length"] = content.size();
    event["msg"]["payload"] = content;

    LOG_DEBUG << event.dump(4);
    return event;
}

std::atomic<unsigned long long> Event::__g_eventID__{0};