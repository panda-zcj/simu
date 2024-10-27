/**
 * @author: pikaqiu_zcj
 * @date: 2024-06-15
 * @par lastEditors: pikaqiu_zcj
 * @par lastEditTime: 2024-08-01
 * @file: \include\global.h
 * @message:
 * @copyright (c) 2024 by pikaqiu, All Rights Reserved.
 */

#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#define EPSILON 1e-5
#define UNUSED(x) (void)(x)
#define ENABLE (1)
#define DISABLE (0)
#ifdef _WIN32
#define LOG_DIRECTORY ".\\"
#else
#define LOG_DIRECTORY "./"
#endif // _WIN32
#define LOG_FILENAME "simulation"
#define LOG_FILE_ROLL_SIZE_MB (1)                                ///< the max size (MB) of per log file
#define LOG_LEVEL nanolog::LogLevel::DEBUG                       ///< select one from DEBUG、INFO、WARN、CRIT
#define LOG_OUTPUT_CONSOLE (0)                                   ///< 1 indicates logs are output to the console, otherwise which are output to the file
#define IPC_TIMEOUT_SIMULATOR_WAIT_REGISTER (10000)              ///< the timeout duration for IPC communication, during which the simulator waits for node registration, is specified in milliseconds
#define IPC_TIMEOUT_RECEIVE_DATA (-1)                            ///< the timeout duration for IPC communication, during which the simulator or node waits to receive data, is specified in milliseconds. A value of -1 indicates that there is no timeout
#define IPC_SOCKET_LISTEN_PORT (8888)                            ///< the port number that the simulator listens on for socket communication
#define IPC_SOCKET_LISTEN_IP "0.0.0.0"                           ///< the ip address that the simulator listens on for socket communication
#define IPC_SOCKET_SERVER_IP "127.0.0.1"                         ///< the ip address of the simulator for socket communication
#define IPC_SOCKET_LISTEN_MAX_CLIENTS (5)                        ///< the number of clients that the simulator can accept for socket communication
#define UI_SOCKET_LISTEN_PORT (9999)                             ///< the port number that the simulator listens on for the remote interface display
#define DEFAULT_SIMULATION_TIME (10)                             ///< the default simulation time, the precision is the milliseconds
#define DEFAULT_SIMULATION_SLOT (1)                              ///< the default simulation slot, the precision is the milliseconds
#define DEFAULT_SIMULATION_NODE_SIZE (3)                         ///< the default number of simulation node
#define SIMULATION_NODE_ID_MASK(x) (x & 0xffff)                  ///< the ID mask of simulation node
#define SIMULATION_UI_ID_MASK(x) ((x & 0xffff) << 16)            ///< the ID mask of remote ui interface
#define SIMULATION_REMOTE_CONFIG_ENABLE (0)                      ///< whether to enable remote interface to start simulation, 1 indicates enable
#define SIMULATION_UNIFORM_DISTRIBUTION_MAX (300)                ///< during the simulation, the maximum value of the uniform distribution
#define SIMULATION_UNIFORM_DISTRIBUTION_MIN (0)                  ///< during the simulation, the minmum value of the uniform distribution
#define INTERVAL_POLL_COMMUNICATION_STATUS (1000)                ///< interval for polling communication state, is specified in milliseconds
#define JSON_FILENAME_SIMULATOR_STATE "simulator_run_state.json" ///< the simulator's extra running state
#define JSON_FILENAME_NODE_STATE "node_run_state.json"           ///< the node's extra running state
#define SECOND_TO_NANOSECOND (1000000000.0)                      ///< seconds to nanoseconds
#define SECOND_TO_MILLISECOND (1000.0)                           ///< seconds to milliseconds
#define MILLISECOND_TO_NANOSECOND (1000000.0)                    ///< milliseconds to nanoseconds

#include <string>

namespace simu
{
    enum class SimulatorState
    {
        SimulatorInit,   ///< init one simulator
        SimulatorRun,    ///< the simulator is in execution state
        SimulatorStop,   ///< the simulator is in a stopped state
        SimulatorResume, ///< the simulator resumes execution
        SimulatorFinish  ///< the simulation is completed
    };
    extern const std::string simulatorState[];

    enum class NodeState
    {
        NodeInit,       ///< init one node
        NodeMovement,   ///< the node is in a moving state
        NodeStationary, ///< the node is in a stationary state
        NodeOffline,    ///< the node is in a offline state
        NodeBUTT
    };
    extern const std::string nodeState[];

    enum class EventState
    {
        EventInit,       ///< init one event
        EventTriggered,  ///< simulation node triggers an event
        EventResponsing, ///< the target node is responding to the event
        EventConflict,   ///< event conflict
        EventResponseNo, ///< event timeout without response
        EventResponsed   ///< event processing completed
    };
    extern const std::string eventState[];
} // namespace simu

#endif // __GLOBAL_H__