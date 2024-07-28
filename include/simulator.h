/**
 * @author: pikaqiu_zcj
 * @date: 2024-06-24
 * @LastEditors: pikaqiu_zcj
 * @LastEditTime: 2024-06-25
 * @file: \include\simulator.h
 * @message:
 * @Copyright (c) 2024 by pikaqiu, All Rights Reserved.
 */
#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "global.h"
#include "communication.h"
#include "times.h"
#include "event.h"
#include "user.h"

#define SIMULATION_BROADCAST_ID "BROADCAST" ///< broadcast the data of all simulation nodes
#define SIMULATION_CONTROL_ID "CONTROL"     ///< the packet of controlling simulation state

namespace simu
{
    /**
     * @brief The simulator is responsible for managing the states of all simulation nodes and distributing events triggered by simulation nodes, but it does not handle event processing or state transitions of simulation nodes. simulation nodes poll the simulator to report their own states and request the states of all simulation nodes maintained by the simulator. Additionally, to simplify implementation, the simulator will uniformly handle the motion state of the nodes.
     */
    class Simulator
    {
    private:
        /**
         * @brief the state of simulator running
         */
        nlohmann::ordered_json simulator;
        /**
         * @brief global timestamp
         */
        Time &g_timeStamp;
        /**
         * @brief global simulation time
         */
        Time &g_simulationTime;
        /**
         * @brief global simulation slot
         */
        double &g_simulationSlot;
        /**
         * @brief generating simulation scene for the simulator running
         */
        GenerateSimulationScene &generateSimulationScene;
        /**
         * @brief IPC handle for simulation data exchange
         */
        SimulatorCommunication &communication;
        /**
         * @brief visualization and analysis of simulation results
         */
        SimulatorStatistic &statistic;
        /**
         * @brief process and dispatch simulation data
         */
        SimulatorDataProcessing &dataProcessing;

    public:
        /**
         * @brief construct one Simulator
         * @attention these variables, used as input parameters, have a global lifecycle
         * @param[in] timeStamp global time stamp
         * @param[in] g_simulationTime global simulation time
         * @param[in] g_simulationSlot global simulation slot
         * @param[in] generateSimulationScene generating simulation scene for the simulator running
         * @param[in] communication IPC handle for simulation data exchange
         * @param[in] statistic visualization and analysis of simulation results
         * @param[in] dataProcessing  process and dispatch simulation data
         */
        Simulator(Time &timeStamp, Time &g_simulationTime, double &g_simulationSlot, GenerateSimulationScene &generateSimulationScene, SimulatorCommunication &communication, SimulatorStatistic &statistic, SimulatorDataProcessing &dataProcessing);
        /**
         * @brief destroy the simulator
         */
        ~Simulator();

    private:
        /**
         * @brief set the send packet
         * @param[in] packet the packet to be sent
         * @param[in] nodeID the ID of simulation node
         */
        void setSendPacket(nlohmann::ordered_json &packet, std::string nodeID = SIMULATION_BROADCAST_ID);
        /**
         * @brief append the simulator's extra running state
         * @return true return true if successful
         * @return false return false if failed
         */
        bool preInitSimulatorState();
        /**
         * @brief init the state of simulator running
         * @return true return true if successful
         * @return false return false if failed
         */
        bool initSimulatorState();

    public:
        /**
         * @brief init simulator
         * @return true return true if successful
         * @return false return false if failed
         */
        bool init();
        /**
         * @brief configure simulation scenario
         * @return true return true if successful
         * @return false return false if failed
         */
        bool ConfigureSimulation();
        /**
         * @brief run simulator
         * @return true return true if successful
         * @return false return false if failed
         */
        bool run();

    public:
        /**
         * @brief execute the entire simulation
         */
        static void exec();
    };

} // namespace simu

#endif // SIMULATOR_H