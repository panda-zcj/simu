/**
 * @author: pikaqiu_zcj
 * @date: 2024-06-23
 * @par lastEditors: pikaqiu_zcj
 * @par lastEditTime: 2024-08-10
 * @file: \src\node\node.cpp
 * @message:
 * @copyright (c) 2024 by pikaqiu, All Rights Reserved.
 */

#include "node.h"
#include <sstream>
#include <fstream>
#include <iomanip>
#include <thread>

using namespace simu;

const std::string simu::nodeState[] = {
    "Node Init",
    "Node Movement",
    "Node Stationary",
    "Node Offline",
};

nlohmann::ordered_json nodeTemplete = {
    {"nodeID", ""},
    {"type", ""},
    {"description", ""},
    {"currentTime", 0},
    {"nodeState", ""},
    {"events", {{"send", nlohmann::ordered_json::object()}, {"receive", {{"selfReceiverEventID", nlohmann::ordered_json::array()}, {"otherReceiverEventID", nlohmann::ordered_json::array()}, {"detail", nlohmann::ordered_json::object()}}}}},
};

nlohmann::ordered_json nodeMsgTemplete = {
    {"nodeID", ""},
    {"type", ""},
    {"description", ""},
    {"currentTime", 0},
    {"nodeState", ""},
};

Node::Node(NodeCommunication &communication, NodeStatistic &statistic, NodeDataProcessing &dataProcessing, NodeProtocolStack &protocolStack) : communication(communication), statistic(statistic), dataProcessing(dataProcessing), protocolStack(protocolStack)
{
    node = nodeTemplete;
}

Node::~Node()
{
}

bool Node::setNodeState(unsigned short nodeID, NodeState state, std::string description, std::string type)
{
    node["nodeID"] = getHexStr(nodeID); ///< nodeID <=> [15:0], short integer variable
    node["type"] = type;
    node["description"] = description;
    node["nodeState"] = nodeState[static_cast<int>(state)];

    return true;
}

bool Node::run(nlohmann::ordered_json &simulatorJsonPacket)
{
    bool ret = dataProcessing(node, simulatorJsonPacket) && protocolStack(node);
    nlohmann::ordered_json nodeJsonPacket(nodeMsgTemplete);

    nodeJsonPacket["nodeID"] = node["nodeID"];
    nodeJsonPacket["type"] = node["type"];
    nodeJsonPacket["description"] = node["description"];
    nodeJsonPacket["currentTime"] = node["currentTime"];
    nodeJsonPacket["nodeState"] = node["nodeState"];
    if (node["events"]["send"].contains("eventID"))
    {
        nodeJsonPacket["events"] = nlohmann::ordered_json::object();
        nodeJsonPacket["events"]["send"] = node["events"]["send"];
        node["events"]["send"] = nlohmann::ordered_json::object();
        node["userDefine"]["statistic"]["sendEventNums"] = node["userDefine"]["statistic"]["sendEventNums"].get<int>() + 1;
    }
    if (!node["events"]["receive"]["selfReceiverEventID"].empty())
    {
        if (!nodeJsonPacket.contains("events"))
        {
            nodeJsonPacket["events"] = nlohmann::ordered_json::object();
        }
        std::string eventID = node["events"]["receive"]["selfReceiverEventID"][0];
        nodeJsonPacket["events"]["receive"] = node["events"]["receive"]["detail"][eventID];
    }
    for (const auto &[key, value] : nodeJsonPacket["userDefine"].items())
    {
        nodeJsonPacket["userDefine"][key] = node["userDefine"][key];
    }
    ret &= communication.pushSendPacket(nodeJsonPacket.dump());
    return ret;
}

bool Node::exit()
{
    statistic(node);
    return true;
}

std::string Node::getHexStr(unsigned short num)
{
    std::stringstream ss;
    ss << "0x" << std::setw(4) << std::setfill('0') << std::hex << num;
    return ss.str();
}

bool Node::preInitNodeState()
{
    // configure user define the state that node running
    nlohmann::ordered_json append;
    std::ifstream ifs(JSON_FILENAME_NODE_STATE, std::ios_base::in);
    if (ifs)
    {
        try
        {
            ifs >> append;
            nodeTemplete["userDefine"] = append["nodeTemplete"];
            nodeMsgTemplete["userDefine"] = append["nodeMsgTemplete"];
        }
        catch (nlohmann::ordered_json::parse_error &e)
        {
            LOG_CRIT << "Parse error in " << JSON_FILENAME_NODE_STATE << ": " << e.what();
            return false;
        }
    }
    ifs.close();

    LOG_DEBUG << "nodeTemplete = " << nodeTemplete.dump(4);
    LOG_DEBUG << "nodeMsgTemplete = " << nodeMsgTemplete.dump(4);

    return true;
}

nlohmann::ordered_json Node::initNodeState(unsigned short nodeID, std::string description, std::string type)
{
    nlohmann::ordered_json nodeJson = nodeMsgTemplete;
    nodeJson["nodeID"] = getHexStr(nodeID);
    nodeJson["type"] = type;
    nodeJson["description"] = description;
    nodeJson["nodeState"] = nodeState[static_cast<int>(NodeState::NodeInit)];

    return nodeJson;
}

void Node::exec()
{
    // 1. 建立连接
    NodeCommunication communication;
    if (!(communication.init() && communication.run()))
    {
        std::exit(-1);
    }
    // 2. 相关全局变量初始化
    std::vector<Node *> nodes;
    std::string data;
    bool ret = preInitNodeState();
    // 3. 仿真循环
    while (ret)
    {
        if (communication.getReceivePacket(data))
        {
            nlohmann::ordered_json simulatorJsonPacket = nlohmann::ordered_json::parse(data);
            double simulationSlot = simulatorJsonPacket["simulationInterval"].get<double>();
            simu::_global_time_.setTime(simulationSlot + simulatorJsonPacket["currentTime"].get<long long>() / MILLISECOND_TO_NANOSECOND);
            if (simulatorJsonPacket["simulatorState"] == simulatorState[static_cast<int>(SimulatorState::SimulatorInit)])
            {
                for (auto it = simulatorJsonPacket["nodes"]["active"].begin(); it != simulatorJsonPacket["nodes"]["active"].end(); it++)
                {
                    NodeStatistic statistic;
                    NodeDataProcessing dataProcessing;
                    NodeProtocolStack protocolStack;
                    Node *simulationNode = new Node(communication, statistic, dataProcessing, protocolStack);

                    std::string nodeIDStr = it->get<std::string>();
                    int nodeID = SIMULATION_NODE_ID_MASK(std::stoi(nodeIDStr, nullptr, 16));
                    auto getNodeState = [&simulatorJsonPacket, nodeIDStr]() -> simu::NodeState
                    {
                        for (size_t i = 0; i < static_cast<size_t>(NodeState::NodeBUTT); ++i)
                        {
                            if (nodeState[i] == simulatorJsonPacket["nodes"]["detail"][nodeIDStr]["nodeState"])
                            {
                                return static_cast<NodeState>(i);
                            }
                        }
                        return NodeState::NodeInit;
                    };
                    simulationNode->setNodeState(nodeID, getNodeState(), simulatorJsonPacket["nodes"]["detail"][nodeIDStr]["description"], simulatorJsonPacket["nodes"]["detail"][nodeIDStr]["type"]);

                    nodes.push_back(simulationNode);
                }
            }
            else if (simulatorJsonPacket["simulatorState"] == simulatorState[static_cast<int>(SimulatorState::SimulatorRun)])
            {
                for (auto it = nodes.begin(); it != nodes.end(); it++)
                {
                    ret &= (*it)->run(simulatorJsonPacket);
                }
            }
            else if (simulatorJsonPacket["simulatorState"] == simulatorState[static_cast<int>(SimulatorState::SimulatorFinish)])
            {
                break;
            }
            else if (simulatorJsonPacket["simulatorState"] == simulatorState[static_cast<int>(SimulatorState::SimulatorStop)])
            {
                /* code */
            }
            else if (simulatorJsonPacket["simulatorState"] == simulatorState[static_cast<int>(SimulatorState::SimulatorResume)])
            {
                /* code */
            }
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(INTERVAL_POLL_COMMUNICATION_STATUS));
        }
    }
    // 4. 仿真结束，释放相关资源
    for (auto it = nodes.begin(); it != nodes.end(); it++)
    {
        (*it)->exit();
        delete (*it);
    }
    // 5. 断开连接
    communication.exit();
    std::exit(!ret);
}