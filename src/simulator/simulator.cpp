/**
 * @author: pikaqiu_zcj
 * @date: 2024-07-28
 * @LastEditors: pikaqiu_zcj
 * @LastEditTime: 2024-07-28
 * @file: \src\simulator\simulator.cpp
 * @message:
 * @Copyright (c) 2024 by pikaqiu, All Rights Reserved.
 */

#include "simulator.h"
#include <fstream>
#include <thread>
using namespace simu;

const std::string simu::simulatorState[] = {
    "Simulator Init",
    "Simulator Run",
    "Simulator Stop",
    "Simulator Resume",
    "Simulator Finish",
};

nlohmann::ordered_json simulatorTemplete = {
    {"currentTime", 0},
    {"simulationDuration", 0},
    {"simulationInterval", 0}, // The simulation interval must be smaller than the minimum time for a simulation node's state to change
    {"simulatorState", ""},
    {"events", {{"active", nlohmann::ordered_json::array()}, {"detail", nlohmann::ordered_json::object()}}},
    {"nodes", {{"totalNodeNums", 0}, {"onlineNodeNums", 0}, {"active", nlohmann::ordered_json::array()}, {"detail", nlohmann::ordered_json::object()}}},
};

nlohmann::ordered_json simulatorMsgTemplete = {
    {"currentTime", 0},
    {"simulationDuration", 0},
    {"simulationInterval", 0}, // The simulation interval must be smaller than the minimum time for a simulation node's state to change
    {"simulatorState", ""},
    {"events", {{"active", nlohmann::ordered_json::array()}, {"detail", nlohmann::ordered_json::object()}}},
    {"nodes", {{"active", nlohmann::ordered_json::array()}, {"detail", nlohmann::ordered_json::object()}}},
};

Simulator::Simulator(Time &timeStamp, Time &g_simulationTime, double &g_simulationSlot, GenerateSimulationScene &generateSimulationScene, SimulatorCommunication &communication, SimulatorStatistic &statistic, SimulatorDataProcessing &dataProcessing) : g_timeStamp(timeStamp), g_simulationTime(g_simulationTime), g_simulationSlot(g_simulationSlot), generateSimulationScene(generateSimulationScene), communication(communication), statistic(statistic), dataProcessing(dataProcessing)
{
}

Simulator::~Simulator()
{
}

void Simulator::setSendPacket(nlohmann::ordered_json &packet, std::string nodeID)
{
    packet["currentTime"] = simulator["currentTime"];
    packet["simulationDuration"] = simulator["simulationDuration"];
    packet["simulationInterval"] = simulator["simulationInterval"];
    packet["simulatorState"] = simulator["simulatorState"];
    if (nodeID == SIMULATION_CONTROL_ID)
    {
        return;
    }
    else if (nodeID == SIMULATION_BROADCAST_ID)
    {
        packet["nodes"]["active"] = simulator["nodes"]["active"];
        packet["nodes"]["detail"] = simulator["nodes"]["detail"];
    }
    else if (simulator["nodes"]["detail"].contains(nodeID))
    {
        packet["nodes"]["active"] = {nodeID};
        packet["nodes"]["detail"][nodeID] = simulator["nodes"]["detail"][nodeID];
    }
    packet["events"]["active"] = simulator["events"]["active"];
    for (auto it = simulator["events"]["active"].begin(); it != simulator["events"]["active"].end(); it++)
    {
        packet["events"]["detail"][it->get<std::string>()] = simulator["events"]["detail"][it->get<std::string>()];
    }
    for (const auto &[key, value] : packet["userDefine"].items())
    {
        packet["userDefine"][key] = simulator["userDefine"][key];
    }
}

bool Simulator::preInitSimulatorState()
{
    // configure user define the information of simulator running
    std::ifstream ifs(JSON_FILENAME_SIMULATOR_STATE, std::ios_base::in);
    if (ifs.is_open())
    {
        try
        {
            nlohmann::ordered_json append;
            ifs >> append;
            simulatorTemplete["userDefine"] = append["simulatorTemplete"];
            simulatorMsgTemplete["userDefine"] = append["simulatorMsgTemplete"];
        }
        catch (nlohmann::ordered_json::parse_error &e)
        {
            LOG_CRIT << "Parse error in " << JSON_FILENAME_SIMULATOR_STATE << ": " << e.what();
            return false;
        }
    }
    ifs.close();

    LOG_DEBUG << "simulatorTemplete = " << simulatorTemplete.dump(4);
    LOG_DEBUG << "simulatorMsgTemplete = " << simulatorMsgTemplete.dump(4);

    return true;
}

bool Simulator::initSimulatorState()
{
    simulator = simulatorTemplete;
    simulator["currentTime"] = g_timeStamp.getTimeStamp();
    simulator["simulationDuration"] = g_simulationTime.getTimeStamp();
    simulator["simulationInterval"] = g_simulationSlot;
    simulator["simulatorState"] = simulatorState[static_cast<int>(SimulatorState::SimulatorInit)];

    return true;
}

bool Simulator::init()
{
    return communication.init() && communication.run() && preInitSimulatorState() && initSimulatorState();
}

bool Simulator::ConfigureSimulation()
{
    bool ret = true;
#if (SIMULATION_REMOTE_CONFIG_ENABLE == ENABLE)
    std::vector<int> ui = communication.getUIHandles();
    if (ui.size() == 0)
    {
        return false;
    }

    for (int i = 0; i < ui.size(); i++)
    {
        communication.bindNodeIDAndIPCHandle(SIMULATION_UI_ID_MASK(i), ui[i]);
    }

    bool stop = false;
    std::string data;
    while (!stop)
    {
        for (int i = 0; i < ui.size(); i++)
        {
            if (communication.getReceivePacket(SIMULATION_UI_ID_MASK(i), data))
            {
                // 远程UI界面传递的数据报文格式为simulatorTemplete
                simulator = nlohmann::ordered_json::parse(data);
                if (simulator.contains("simulatorState") && simulator["simulatorState"] == simulatorState[static_cast<int>(SimulatorState::SimulatorInit)])
                {
                    stop = true;
                    break;
                }
            }
            else
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(INTERVAL_POLL_COMMUNICATION_STATUS));
            }
        }
    }
#else
    generateSimulationScene(simulator);
#endif

    LOG_INFO << "simulator = " << simulator.dump(4);

    std::vector<int> handles = communication.getIPCHandles();
    int index = 0;
    for (auto it = simulator["nodes"]["active"].begin(); ret && it != simulator["nodes"]["active"].end(); it++)
    {
        int nodeID = SIMULATION_NODE_ID_MASK(std::stoi(it->get<std::string>(), nullptr, 16));
        ret &= communication.bindNodeIDAndIPCHandle(nodeID, handles[index]);

        nlohmann::ordered_json simulatorJsonPacket(simulatorMsgTemplete);
        setSendPacket(simulatorJsonPacket, it->get<std::string>());
        ret &= communication.pushSendPacket(nodeID, simulatorJsonPacket.dump());

        index = (index + 1) % handles.size();
    }

    return ret;
}

bool Simulator::run()
{
    bool ret = true;
    std::string data;
    std::vector<int> handles = communication.getIPCHandles();
    simulator["simulatorState"] = simulatorState[static_cast<int>(SimulatorState::SimulatorRun)];
    for (; ret && g_timeStamp <= g_simulationTime;)
    {
        nlohmann::ordered_json simulatorJsonPacket(simulatorMsgTemplete);
        setSendPacket(simulatorJsonPacket);

        for (int i = 0; ret && i < handles.size(); i++)
        {
            ret &= communication.pushSendPacketByHandle(handles[i], simulatorJsonPacket.dump());
        }

        std::vector<int> ui = communication.getUIHandles();
        for (int i = 0; i < ui.size(); i++)
        {
            communication.bindNodeIDAndIPCHandle(SIMULATION_UI_ID_MASK(i), ui[i]);
            communication.pushSendPacket(SIMULATION_UI_ID_MASK(i), simulatorJsonPacket.dump());
            if (communication.getReceivePacket(SIMULATION_UI_ID_MASK(i), data))
            {
                nlohmann::ordered_json jsonData = nlohmann::ordered_json::parse(data);
                if (jsonData.contains("simulatorState"))
                {
                    simulator["simulatorState"] = jsonData["simulatorState"];
                }
            }
        }

        g_timeStamp += g_simulationSlot;
        simulator["currentTime"] = g_timeStamp.getTimeStamp();

        for (int i = 0; ret && i < DEFAULT_SIMULATION_NODE_SIZE; i++)
        {
            while (!communication.getReceivePacket(SIMULATION_NODE_ID_MASK(i), data))
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(INTERVAL_POLL_COMMUNICATION_STATUS));
            }
            nlohmann::ordered_json nodeJsonPacket = nlohmann::ordered_json::parse(data);
            simulator["nodes"]["detail"][nodeJsonPacket["nodeID"]] = nodeJsonPacket;
        }

        dataProcessing(simulator);
    }

    simulator["simulatorState"] = simulatorState[static_cast<int>(SimulatorState::SimulatorFinish)];
    nlohmann::ordered_json simulatorJsonPacket(simulatorMsgTemplete);
    setSendPacket(simulatorJsonPacket, SIMULATION_CONTROL_ID);

    for (int i = 0; ret && i < handles.size(); i++)
    {
        ret &= communication.pushSendPacketByHandle(handles[i], simulatorJsonPacket.dump());
    }

    statistic(simulator);
    communication.exit();
    return ret;
}

void Simulator::exec()
{
    Time g_simulationTime = Time(DEFAULT_SIMULATION_TIME);
    double g_simulationSlot = DEFAULT_SIMULATION_SLOT;
    SimulatorScene generateSimulationScene;
    SimulatorCommunication communication;
    SimulatorDataProcessing dataProcessing;
    SimulatorStatistic statistic;
    Simulator __simulator__(_global_time_, g_simulationTime, g_simulationSlot, generateSimulationScene, communication, statistic, dataProcessing);
    bool ret = __simulator__.init() && __simulator__.ConfigureSimulation() && __simulator__.run();
    std::exit(!ret);
}