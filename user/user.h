/**
 * @author: pikaqiu_zcj
 * @date: 2024-08-18
 * @par lastEditors: pikaqiu_zcj
 * @par lastEditTime: 2024-08-18
 * @file: \include\user.h
 * @message:
 * @copyright (c) 2024 by pikaqiu, All Rights Reserved.
 */
#ifndef USER_H
#define USER_H

#include "generatesimulationscene.h"
#include <random>

namespace simu
{
    /**
     * @brief generating simulation scene for the simulator running
     * @attention the current class is a derived class that implements the generation of a specific simulation scenario
     */
    class SimulatorScene : public GenerateSimulationScene
    {
    private:
        std::mt19937 gen;                              ///< the random number generator based Mersenne twister
        std::uniform_int_distribution<int> distribute; ///< define the random number distribution

    public:
        SimulatorScene(/* args */);
        ~SimulatorScene();

        /**
         * @brief The method for generating simulation scenarios
         * @param[in] simulator the state of simulator running
         * @return true return true if successful
         * @return false return false if failed
         */
        bool operator()(nlohmann::ordered_json &simulator);
    };

    /**
     * @brief visualization and analysis of simulation results
     */
    class SimulatorStatistic
    {
    private:
        /* data */
    public:
        SimulatorStatistic(/* args */);
        ~SimulatorStatistic();

        /**
         * @brief The method for visualization and analysis of simulation results
         * @param[in] simulator the state of simulator running
         * @return true return true if successful
         * @return false return false if failed
         */
        bool operator()(nlohmann::ordered_json &simulator);
    };

    /**
     * @brief process and dispatch simulation data
     */
    class SimulatorDataProcessing
    {
    private:
        /* data */
    public:
        SimulatorDataProcessing(/* args */);
        ~SimulatorDataProcessing();

        /**
         * @brief The method for processing and dispatching simulation data
         * @param[in] simulator the state of simulator running
         * @return true return true if successful
         * @return false return false if failed
         */
        bool operator()(nlohmann::ordered_json &simulator);
    };

    /**
     * @brief analyze the simulation results of the simulation node
     */
    class NodeStatistic
    {
    private:
        /* data */
    public:
        NodeStatistic(/* args */);
        ~NodeStatistic();

        /**
         * @brief The method for analyzing the simulation results of the simulation node
         * @param[in] node the running state of simulation node
         * @return true return true if successful
         * @return false return false if failed
         */
        bool operator()(nlohmann::ordered_json &node);
    };

    /**
     * @brief receiving events related to the current node
     */
    class NodeDataProcessing
    {
    private:
        /* data */
    public:
        NodeDataProcessing(/* args */);
        ~NodeDataProcessing();

        /**
         * @brief The method for receiving events related to the current node
         * @param[in] node the running state of simulation node
         * @param[in] simulatorJsonPacket packet broadcasted by the simulator
         * @return true return true if successful
         * @return false return false if failed
         */
        bool operator()(nlohmann::ordered_json &node, nlohmann::ordered_json &simulatorJsonPacket);
    };

    /**
     * @brief processing events received by the current node
     */
    class NodeProtocolStack
    {
    private:
        /* data */
    public:
        NodeProtocolStack(/* args */);
        ~NodeProtocolStack();

        /**
         * @brief The method for processing events received by the current node
         * @param[in] node the running state of simulation node
         * @return true return true if successful
         * @return false return false if failed
         */
        bool operator()(nlohmann::ordered_json &node);
    };

} // namespace simu

#endif // USER_H
