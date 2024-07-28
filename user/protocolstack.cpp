/**
 * @author: pikaqiu_zcj
 * @date: 2024-10-07
 * @par lastEditors: pikaqiu_zcj
 * @par lastEditTime: 2024-10-07
 * @file: \user\protocolstack.cpp
 * @message:
 * @copyright (c) 2024 by pikaqiu, All Rights Reserved.
 */

#include "user.h"
#include "log.h"
#include "node.h"
#include "event.h"

using namespace simu;

NodeProtocolStack::NodeProtocolStack(/* args */)
{
}

NodeProtocolStack::~NodeProtocolStack()
{
}

bool NodeProtocolStack::operator()(nlohmann::ordered_json &node)
{
    if (!node["events"]["receive"]["selfReceiverEventID"].empty())
    {
        std::string eventID = node["events"]["receive"]["selfReceiverEventID"][0];
        nlohmann::ordered_json &event = node["events"]["receive"]["detail"][eventID];
        LOG_INFO << "node[" << node["nodeID"].dump() << "] from node["
                 << event["srcNodeID"].dump() << "] at time "
                 << Time(event["responsedTime"].get<long long>() / MILLISECOND_TO_NANOSECOND).printCurTime() << "s, receives event and starts time is "
                 << Time(event["startTime"].get<long long>() / MILLISECOND_TO_NANOSECOND).printCurTime() << "s";
        LOG_DEBUG << "receive data : " << event["msg"]["payload"].get<std::string>();

        /*** type your code ****/

        /*** type your code ****/
        event["msg"]["payload"] = "";
    }

    /*** type your code ****/

    if (node["nodeID"] == "0x0002" && node["currentTime"] == DEFAULT_SIMULATION_SLOT * (1000000))
    {
        std::string content = "this is node(0x0002)'s request, send data to node(0x0001) !!!";
        std::string srcId = Node::getHexStr(2);
        std::string dstId = Node::getHexStr(1);
        double duration = 1000 * content.size() * 8 / node["userDefine"]["bitRate"].get<double>();
        double maxPropagateTime = 1000 * node["userDefine"]["coverageArea"].get<double>() / node["userDefine"]["eventPropagationSpeed"].get<double>();
        node["events"]["send"] = Event::getEvent(duration, maxPropagateTime, content, srcId, dstId);
    }
    else if (node["nodeID"] == "0x0001" && node["currentTime"] == DEFAULT_SIMULATION_SLOT * (2000000))
    {
        std::string content = "this is node(0x0001)'s request, send data to node(0x0000) !!! data is ";
        for (int i = 0; i < 50; i++)
        {
            content += "this is test. ";
        }
        std::string srcId = Node::getHexStr(1);
        std::string dstId = Node::getHexStr(0);
        double duration = 1000 * content.size() * 8 / node["userDefine"]["bitRate"].get<double>();
        double maxPropagateTime = 1000 * node["userDefine"]["coverageArea"].get<double>() / node["userDefine"]["eventPropagationSpeed"].get<double>();
        node["events"]["send"] = Event::getEvent(duration, maxPropagateTime, content, srcId, dstId);
    }

    /*** type your code ****/
    if (node["events"]["send"].contains("eventID"))
    {
        LOG_INFO << "node[" << node["nodeID"].dump() << "] has sent event("
                 << node["events"]["send"]["eventID"].dump() << ") to node["
                 << node["events"]["send"]["destNodeID"].dump() << "] at time "
                 << Time(node["events"]["send"]["startTime"].get<long long>() / MILLISECOND_TO_NANOSECOND).printCurTime() << "s";
    }
    return true;
}