/**
 * @author: pikaqiu_zcj
 * @date: 2024-07-13
 * @LastEditors: pikaqiu_zcj
 * @LastEditTime: 2024-07-21
 * @file: \include\node.h
 * @message:
 * @Copyright (c) 2024 by pikaqiu, All Rights Reserved.
 */
#ifndef NODE_H
#define NODE_H

#include "global.h"
#include "times.h"
#include "log.h"
#include "event.h"
#include "communication.h"
#include "user.h"

namespace simu
{
    /**
     * @brief The simulation node needs to retrieve information relevant to itself from the messages broadcasted by the simulator, calculate the running state that should be in based on this information, and finally report its running state to the simulator.
     */
    class Node
    {
    private:
        /**
         * @brief the state of simulation node running
         */
        nlohmann::ordered_json node;
        /**
         * @brief IPC handle for simulation data exchange
         */
        NodeCommunication &communication;
        /**
         * @brief analyze the simulation results of the current node
         */
        NodeStatistic &statistic;
        /**
         * @brief receiving events related to the current node
         */
        NodeDataProcessing &dataProcessing;
        /**
         * @brief processing events received by the current node
         */
        NodeProtocolStack &protocolStack;

    public:
        /**
         * @brief construct one simulation node
         * @attention these variables, used as input parameters, have a global lifecycle
         * @param[in] communication IPC handle for simulation data exchange
         * @param[in] statistic analyze the simulation results of the current node
         * @param[in] dataProcessing receiving events related to the current node
         * @param[in] protocolStack processing events received by the current node
         */
        Node(NodeCommunication &communication, NodeStatistic &statistic, NodeDataProcessing &dataProcessing, NodeProtocolStack &protocolStack);
        /**
         * @brief destroy the simulation node
         */
        ~Node();

    public:
        /**
         * @brief set the running state of simulation node
         * @param[in] nodeID the ID of simulation node
         * @param[in] state the running state of simulation node
         * @param[in] description the description of simulation node
         * @param[in] type the type of simulation node
         * @return true return true if successful
         * @return false return false if failed
         */
        bool setNodeState(unsigned short nodeID, NodeState state = NodeState::NodeInit, std::string description = "", std::string type = "");
        /**
         * @brief run simulation node
         * @param[in] simulatorJsonPacket packet broadcasted by the simulator
         * @return true return true if successful
         * @return false return false if failed
         */
        bool run(nlohmann::ordered_json &simulatorJsonPacket);
        /**
         * @brief  exit the simulation
         * @return true return true if successful
         * @return false return false if failed
         */
        bool exit();

    public:
        /**
         * @brief convert decimal to hexadecimal
         * @param[in] num the decimal number to be converted
         * @return std::string
         */
        static std::string getHexStr(unsigned short num);
        /**
         * @brief append the simulation node's extra running state
         * @return true return true if successful
         * @return false return false if failed
         */
        static bool preInitNodeState();
        /**
         * @brief init the state of simulation node running
         * @param[in] nodeID the ID of simulation node
         * @param[in] description the description of simulation node
         * @param[in] type the type of simulation node
         * @return nlohmann::ordered_json the state of simulation node running
         */
        static nlohmann::ordered_json initNodeState(unsigned short nodeID, std::string description = "", std::string type = "");
        /**
         * @brief execute the entire simulation
         */
        static void exec();
    };

} // namespace simu

#endif // NODE_H
