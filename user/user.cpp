/**
 * @author: pikaqiu_zcj
 * @date: 2024-08-18
 * @par lastEditors: pikaqiu_zcj
 * @par lastEditTime: 2024-08-18
 * @file: \user\user.cpp
 * @message:
 * @copyright (c) 2024 by pikaqiu, All Rights Reserved.
 */

#include "user.h"
#include "log.h"
#include "node.h"
#include "event.h"
#include <cmath>

using namespace simu;

SimulatorScene::SimulatorScene(/* args */) : gen(std::mt19937((unsigned int)time(NULL))), distribute(std::uniform_int_distribution<int>(SIMULATION_UNIFORM_DISTRIBUTION_MIN, SIMULATION_UNIFORM_DISTRIBUTION_MAX))
{
}

SimulatorScene::~SimulatorScene()
{
}

bool SimulatorScene::operator()(nlohmann::ordered_json &simulator)
{
    if (!Node::preInitNodeState())
    {
        return false;
    }
    /*************************** configure node, type your code ***********************/

    nlohmann::ordered_json nodeJson = Node::initNodeState(SIMULATION_NODE_ID_MASK(0), "this is the master node", "master");
    nodeJson["nodeState"] = nodeState[static_cast<int>(NodeState::NodeStationary)];
    nodeJson["userDefine"]["eventPropagationSpeed"] = 300000000; ///< 300000000 m/s
    nodeJson["userDefine"]["bitRate"] = 1000000;                 ///< 1Mbps
    nodeJson["userDefine"]["speed"] = {0.0, 0.0};
    nodeJson["userDefine"]["location"] = {0.0, 0.0};
    nodeJson["userDefine"]["energy"] = 5000.0;                                            ///< 5000J
    nodeJson["userDefine"]["power"] = 1.0;                                                ///< 1W
    nodeJson["userDefine"]["snr"] = 10.0;                                                 ///< 10db
    nodeJson["userDefine"]["coverageArea"] = (double)SIMULATION_UNIFORM_DISTRIBUTION_MAX; ///< 300m
    simulator["nodes"]["detail"][nodeJson["nodeID"]] = nodeJson;
    for (int i = 1; i < DEFAULT_SIMULATION_NODE_SIZE; i++)
    {
        nodeJson = Node::initNodeState(SIMULATION_NODE_ID_MASK(i), "this is the slave node", "slave");
        nodeJson["nodeState"] = nodeState[static_cast<int>(NodeState::NodeMovement)];
        nodeJson["userDefine"]["eventPropagationSpeed"] = 300000000; ///< 300000000 m/s
        nodeJson["userDefine"]["bitRate"] = 1000000;                 ///< 1Mbps
        nodeJson["userDefine"]["speed"] = {20.0, 0.0};               ///< 20 m/s
        nodeJson["userDefine"]["location"] = {(double)(distribute(gen)), (double)(distribute(gen))};
        nodeJson["userDefine"]["energy"] = 500.0;                                             ///< 500J
        nodeJson["userDefine"]["power"] = 1.0;                                                ///< 1W
        nodeJson["userDefine"]["snr"] = 10.0;                                                 ///< 10db
        nodeJson["userDefine"]["coverageArea"] = (double)SIMULATION_UNIFORM_DISTRIBUTION_MAX; ///< 300m
        simulator["nodes"]["detail"][nodeJson["nodeID"]] = nodeJson;
    }

    /*************************** configure node, type your code ***********************/
    return GenerateSimulationScene::operator()(simulator);
}

SimulatorDataProcessing::SimulatorDataProcessing(/* args */)
{
}

SimulatorDataProcessing::~SimulatorDataProcessing()
{
}

bool SimulatorDataProcessing::operator()(nlohmann::ordered_json &simulator)
{
    nlohmann::ordered_json &nodes = simulator["nodes"]["detail"];
    nlohmann::ordered_json &events = simulator["events"]["detail"];
    /*************************** type your code ***********************/

    std::chrono::nanoseconds::rep totalThroughput = simulator["userDefine"]["statistic"]["totalThroughput"].get<long long>();
    std::chrono::nanoseconds::rep totalEventSuccessNums = simulator["userDefine"]["statistic"]["totalEventSuccessNums"].get<long long>();
    std::chrono::nanoseconds::rep totalAverageDelay = simulator["userDefine"]["statistic"]["totalAverageDelay"].get<long long>();
    std::chrono::nanoseconds::rep totalEventNums = simulator["userDefine"]["statistic"]["totalEventNums"].get<long long>();
    std::chrono::nanoseconds::rep currentTime = simulator["currentTime"].get<long long>();
    double simulationSlot = simulator["simulationInterval"].get<double>();
    std::vector<std::string> active;

    for (auto it = nodes.begin(); it != nodes.end(); it++)
    {
        nlohmann::ordered_json &node = nodes[it.key()];

        LOG_DEBUG << "node[" << node["nodeID"].dump() << "]["
                  << node["type"].dump() << "]["
                  << node["description"].dump() << "]["
                  << (int)(node["currentTime"].get<long long>() / MILLISECOND_TO_NANOSECOND / simulationSlot) << " slot]["
                  << node["nodeState"].dump() << "] : "
                  << node["userDefine"].dump(4);

        // 1. 管理节点传播的事件
        if (node.contains("events"))
        {
            if (node["events"].contains("receive"))
            {
                LOG_INFO << "node[" << node["nodeID"].dump() << "] has received event("
                         << node["events"]["receive"]["eventID"].dump() << ") from node["
                         << node["events"]["receive"]["srcNodeID"].dump() << "], the event state is "
                         << node["events"]["receive"]["eventState"].dump() << ", the start time is "
                         << Time(node["events"]["receive"]["startTime"].get<long long>() / MILLISECOND_TO_NANOSECOND).printCurTime() << "s, the completed time is "
                         << Time(node["events"]["receive"]["responsedTime"].get<long long>() / MILLISECOND_TO_NANOSECOND).printCurTime() << "s";

                if (node["events"]["receive"]["eventState"] == eventState[static_cast<int>(EventState::EventResponsed)])
                {
                    std::chrono::nanoseconds::rep startTime = node["events"]["receive"]["startTime"].get<long long>();
                    std::chrono::nanoseconds::rep responsedTime = node["events"]["receive"]["responsedTime"].get<long long>();
                    totalThroughput += node["events"]["receive"]["msg"]["acknowledgedSize"].get<int>();
                    totalEventSuccessNums++;
                    totalAverageDelay += responsedTime - startTime;
                }
                else if (node["events"]["receive"]["eventState"] == eventState[static_cast<int>(EventState::EventConflict)])
                {
                }
                else if (node["events"]["receive"]["eventState"] == eventState[static_cast<int>(EventState::EventResponseNo)])
                {
                }
                else if (node["events"]["receive"]["eventState"] == eventState[static_cast<int>(EventState::EventResponsing)])
                {
                }
                else if (node["events"]["receive"]["eventState"] == eventState[static_cast<int>(EventState::EventTriggered)])
                {
                }
                events[node["events"]["receive"]["eventID"]]["eventState"] = node["events"]["receive"]["eventState"];
            }
            if (node["events"].contains("send"))
            {
                LOG_INFO << "node[" << node["nodeID"].dump() << "] has sent event("
                         << node["events"]["send"]["eventID"].dump() << ") to node["
                         << node["events"]["send"]["destNodeID"].dump() << "] at time "
                         << Time(node["events"]["send"]["startTime"].get<long long>() / MILLISECOND_TO_NANOSECOND).printCurTime() << "s";

                if (node["events"]["send"]["eventState"] == eventState[static_cast<int>(EventState::EventInit)])
                {
                    events[node["events"]["send"]["eventID"]] = node["events"]["send"];
                    events[node["events"]["send"]["eventID"]]["eventState"] = eventState[static_cast<int>(EventState::EventTriggered)];
                    totalEventNums++;
                }
            }
            node.erase("events");
        }

        // 2. 节点运动状态管理
        // todo 节点移动边界处理
        if (node["userDefine"]["speed"][0].get<double>() != 0 || node["userDefine"]["speed"][1].get<double>() != 0)
        {
            double speed_x = node["userDefine"]["speed"][0].get<double>();
            double speed_y = node["userDefine"]["speed"][1].get<double>();
            double location_x = node["userDefine"]["location"][0].get<double>();
            double location_y = node["userDefine"]["location"][1].get<double>();
            location_x += simulationSlot / SECOND_TO_MILLISECOND * speed_x;
            location_y += simulationSlot / SECOND_TO_MILLISECOND * speed_y;
            node["userDefine"]["location"] = {location_x, location_y};
        }
    }

    // 3. 管理临界和超时事件
    for (auto it = events.begin(); it != events.end();)
    {
        std::string eventID = it.key();
        std::chrono::nanoseconds::rep startTime = events[eventID]["startTime"].get<long long>();
        std::chrono::nanoseconds::rep expiredTime = events[eventID]["expiredTime"].get<long long>();
        if (startTime <= currentTime && currentTime < expiredTime)
        {
            active.emplace_back(eventID);
        }
        else if (expiredTime <= currentTime)
        {
            LOG_INFO << "event[" << eventID << "] has been expired !!!";
            it = events.erase(it);
            continue;
        }
        else
        {
            /* code */
        }
        it++;
    }

    simulator["userDefine"]["statistic"]["totalThroughput"] = totalThroughput;
    simulator["userDefine"]["statistic"]["totalEventSuccessNums"] = totalEventSuccessNums;
    simulator["userDefine"]["statistic"]["totalAverageDelay"] = totalAverageDelay;
    simulator["userDefine"]["statistic"]["totalEventNums"] = totalEventNums;
    simulator["events"]["active"] = active;

    /*************************** type your code ***********************/

    return true;
}

SimulatorStatistic::SimulatorStatistic(/* args */)
{
}

SimulatorStatistic::~SimulatorStatistic()
{
}

bool SimulatorStatistic::operator()(nlohmann::ordered_json &simulator)
{
    /*************************** type your code ***********************/
    double simulationDuration = simulator["simulationDuration"].get<double>();
    double totalThroughput = simulator["userDefine"]["statistic"]["totalThroughput"].get<double>() * 8;
    double totalEventSuccessNums = simulator["userDefine"]["statistic"]["totalEventSuccessNums"].get<double>();
    double totalAverageDelay = simulator["userDefine"]["statistic"]["totalAverageDelay"].get<double>();
    simulator["userDefine"]["statistic"]["totalThroughput"] = totalThroughput / (simulationDuration / SECOND_TO_NANOSECOND);
    if (totalEventSuccessNums > 0)
    {
        simulator["userDefine"]["statistic"]["totalAverageDelay"] = totalAverageDelay / totalEventSuccessNums / SECOND_TO_NANOSECOND;
    }

    LOG_INFO << "totalThroughput = "
             << simulator["userDefine"]["statistic"]["totalThroughput"].dump() << "bps, totalAverageDelay = "
             << simulator["userDefine"]["statistic"]["totalAverageDelay"].dump() << "s, totalEventNums = "
             << simulator["userDefine"]["statistic"]["totalEventNums"].dump() << ", totalEventSuccessNums = "
             << simulator["userDefine"]["statistic"]["totalEventSuccessNums"].dump();
    /*************************** type your code ***********************/

    return true;
}

NodeDataProcessing::NodeDataProcessing(/* args */)
{
}

NodeDataProcessing::~NodeDataProcessing()
{
}

bool NodeDataProcessing::operator()(nlohmann::ordered_json &node, nlohmann::ordered_json &simulatorJsonPacket)
{
    double simulationSlot = simulatorJsonPacket["simulationInterval"].get<double>();
    node["currentTime"] = Time(simulationSlot + simulatorJsonPacket["currentTime"].get<long long>() / MILLISECOND_TO_NANOSECOND).getTimeStamp();
    nlohmann::ordered_json &nodes = simulatorJsonPacket["nodes"]["detail"];
    nlohmann::ordered_json &events = simulatorJsonPacket["events"]["detail"];
    nlohmann::ordered_json &curNodeRxEvents = node["events"]["receive"]["detail"];
    nlohmann::ordered_json &userDefine = node["userDefine"];
    for (const auto &[key, value] : nodes[node["nodeID"]]["userDefine"].items())
    {
        userDefine[key] = nodes[node["nodeID"]]["userDefine"][key];
    }
    LOG_DEBUG << "node[" << node["nodeID"].dump() << "]["
              << node["type"].dump() << "]["
              << node["description"].dump() << "]["
              << int(node["currentTime"].get<long long>() / MILLISECOND_TO_NANOSECOND / simulationSlot) - 1 << " slot]["
              << node["nodeState"].dump() << "] : "
              << node["userDefine"].dump(4);
    /*************************** type your code ***********************/

    std::chrono::nanoseconds::rep throughput = userDefine["statistic"]["throughput"].get<long long>();
    std::chrono::nanoseconds::rep reciveEventNums = userDefine["statistic"]["reciveEventNums"].get<long long>();
    std::chrono::nanoseconds::rep averageDelay = userDefine["statistic"]["averageDelay"].get<long long>();
    std::chrono::nanoseconds::rep currentTime = node["currentTime"].get<long long>();
    double snr = userDefine["snr"].get<double>();
    double coverageArea = userDefine["coverageArea"].get<double>();
    double location_x = userDefine["location"][0].get<double>();
    double location_y = userDefine["location"][1].get<double>();
    double totalPower = 0;
    double lambda = 1.0; ///< todo equivalent attenuation coefficient
    std::vector<std::string> selfReceiverEventID;
    std::vector<std::string> otherReceiverEventID;

    // 1. 计算信噪比
    for (auto it = events.begin(); it != events.end(); it++)
    {
        std::string eventID = it.key();
        std::string srcNodeID = events[eventID]["srcNodeID"];
        std::string destNodeID = events[eventID]["destNodeID"];
        std::chrono::nanoseconds::rep startTime = events[eventID]["startTime"];
        double location_x_0 = nodes[srcNodeID]["userDefine"]["location"][0];
        double location_y_0 = nodes[srcNodeID]["userDefine"]["location"][1];
        double distance_square = std::pow(location_x - location_x_0, 2) + std::pow(location_y - location_y_0, 2);
        double distance = std::sqrt(distance_square);
        double power_srcNode = nodes[srcNodeID]["userDefine"]["power"];
        double eventPropagationSpeed = nodes[destNodeID]["userDefine"]["eventPropagationSpeed"];
        double delay = 1000 * distance / eventPropagationSpeed;
        std::chrono::nanoseconds::rep arrivedTime = startTime + Time(delay).getTimeStamp();
        if (arrivedTime <= currentTime)
        {
            totalPower += distance_square > 0 ? power_srcNode : power_srcNode * lambda / distance_square;
        }
    }

    // 2. 接收数据处理
    for (auto it = events.begin(); it != events.end(); it++)
    {
        std::string eventID = it.key();
        if (curNodeRxEvents.contains(eventID) && ((curNodeRxEvents[eventID]["eventState"] == eventState[static_cast<int>(EventState::EventResponseNo)]) || (curNodeRxEvents[eventID]["eventState"] == eventState[static_cast<int>(EventState::EventConflict)])))
        {
            continue;
        }

        std::string destNodeID = events[eventID]["destNodeID"];
        std::string srcNodeID = events[eventID]["srcNodeID"];
        std::chrono::nanoseconds::rep startTime = events[eventID]["startTime"];
        std::chrono::nanoseconds::rep endTime = events[eventID]["endTime"];
        double location_x_0 = nodes[srcNodeID]["userDefine"]["location"][0];
        double location_y_0 = nodes[srcNodeID]["userDefine"]["location"][1];
        double distance_square = std::pow(location_x - location_x_0, 2) + std::pow(location_y - location_y_0, 2);
        double distance = std::sqrt(distance_square);
        double eventPropagationSpeed = nodes[destNodeID]["userDefine"]["eventPropagationSpeed"];
        double delay = 1000 * distance / eventPropagationSpeed;
        if (!curNodeRxEvents.contains(eventID))
        {
            std::chrono::nanoseconds::rep arrivedTime = startTime + Time(delay).getTimeStamp();
            if (arrivedTime <= currentTime)
            {
                curNodeRxEvents[eventID] = events[eventID];
                curNodeRxEvents[eventID]["eventState"] = eventState[static_cast<int>(EventState::EventResponsing)];
            }
            else
            {
                continue;
            }
        }

        if (std::fabs(distance - coverageArea) > EPSILON && distance > coverageArea)
        {
            if (node["nodeID"] == destNodeID)
            {
                curNodeRxEvents[eventID]["eventState"] = eventState[static_cast<int>(EventState::EventResponseNo)];
            }
            continue;
        }

        double power_srcNode = nodes[srcNodeID]["userDefine"]["power"];
        double thresholdPower = (distance_square > 0 ? power_srcNode : power_srcNode * lambda / distance_square) * (1 + 1 / std::pow(10, snr / 10));
        if (std::fabs(thresholdPower - totalPower) > EPSILON && thresholdPower < totalPower)
        {
            if (node["nodeID"] == destNodeID)
            {
                curNodeRxEvents[eventID]["eventState"] = eventState[static_cast<int>(EventState::EventConflict)];
            }
            continue;
        }

        std::chrono::nanoseconds::rep responsedTime = endTime + Time(delay).getTimeStamp();
        if (responsedTime <= currentTime)
        {
            if (node["nodeID"] == destNodeID)
            {
                selfReceiverEventID.emplace_back(eventID);
                curNodeRxEvents[eventID]["responsedTime"] = responsedTime;
                curNodeRxEvents[eventID]["msg"]["acknowledgedSize"] = curNodeRxEvents[eventID]["msg"]["length"];
                throughput += curNodeRxEvents[eventID]["msg"]["length"].get<int>();
                reciveEventNums++;
                averageDelay += responsedTime - startTime;
            }
            else
            {
                otherReceiverEventID.emplace_back(eventID);
            }
            curNodeRxEvents[eventID]["eventState"] = eventState[static_cast<int>(EventState::EventResponsed)];
        }
    }

    node["events"]["receive"]["selfReceiverEventID"] = selfReceiverEventID;
    node["events"]["receive"]["otherReceiverEventID"] = otherReceiverEventID;
    userDefine["statistic"]["throughput"] = throughput;
    userDefine["statistic"]["reciveEventNums"] = reciveEventNums;
    userDefine["statistic"]["averageDelay"] = averageDelay;

    /*************************** type your code ***********************/
    return true;
}

NodeStatistic::NodeStatistic(/* args */)
{
}

NodeStatistic::~NodeStatistic()
{
}

bool NodeStatistic::operator()(nlohmann::ordered_json &node)
{
    /*************************** type your code ***********************/
    double currentTime = node["currentTime"].get<double>();
    double throughput = node["userDefine"]["statistic"]["throughput"].get<double>() * 8;
    double reciveEventNums = node["userDefine"]["statistic"]["reciveEventNums"].get<double>();
    double averageDelay = node["userDefine"]["statistic"]["averageDelay"].get<double>();
    if (currentTime > 0)
    {
        node["userDefine"]["statistic"]["throughput"] = throughput / (currentTime / SECOND_TO_NANOSECOND);
    }
    if (reciveEventNums > 0)
    {
        node["userDefine"]["statistic"]["averageDelay"] = averageDelay / reciveEventNums / SECOND_TO_NANOSECOND;
    }

    LOG_INFO << "node[" << node["nodeID"].dump() << "] : " << "throughput = "
             << node["userDefine"]["statistic"]["throughput"].dump() << "bps, averageDelay = "
             << node["userDefine"]["statistic"]["averageDelay"].dump() << "s, sendEventNums = "
             << node["userDefine"]["statistic"]["sendEventNums"].dump() << ", reciveEventNums = "
             << node["userDefine"]["statistic"]["reciveEventNums"].dump();
    /*************************** type your code ***********************/

    return true;
}