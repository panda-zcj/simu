/**
 * @author: pikaqiu_zcj
 * @date: 2024-07-13
 * @par LastEditors: pikaqiu_zcj
 * @par LastEditTime: 2024-08-01
 * @file: \include\communication.h
 * @message:
 * @copyright (c) 2024 by pikaqiu, All Rights Reserved.
 */

#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "global.h"
#include "log.h"

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>
#endif
#include <atomic>
#include <string>
#include <vector>
#include <queue>
#include <map>

#define SOCKET_SEND_DATA (0)
#define SOCKET_RECEIVE_DATA (1)
#define SOCKET_DATA_HEADER_WIDTH (8)
#ifdef _WIN32
#define SOCKET_FD SOCKET
#else
#define SOCKET_FD int
#endif
namespace simu
{
    /**
     * @brief The status of an IPC process
     */
    enum class IPCState
    {
        INIT,
        RUNNING,
        STOP
    };

    extern const std::string ipcState[];

    /**
     * @brief actions of IPC processes
     */
    enum class IPCProcess
    {
        SEND,    ///< send packets from the send or receive queue
        RECEIVE, ///< receive packets from the send or receive queue
        ACCESS,  ///< get the front packet from the send or receive queue
        POP,     ///< pop the front packet from the send or receive queue
        PUSH,    ///< push the packet into the send or receive queue
        EXIT
    };

    class TCPSocket
    {
    public:
        TCPSocket();
        ~TCPSocket();

        /**
         * @brief init IPC handle (socket file descriptor) of server
         * @param[in] port TCP port
         * @param[in] serverIP local TCP IP
         * @param[in] backlog max numbers of IPC handle (socket file descriptor)
         * @return true return true if successful
         * @return false return false if failed
         */
        bool initServer(int port = IPC_SOCKET_LISTEN_PORT, const std::string serverIP = IPC_SOCKET_LISTEN_IP, int backlog = IPC_SOCKET_LISTEN_MAX_CLIENTS);
        /**
         * @brief wait to client's connection
         * @param[in] timeoutMS timeout duration in milliseconds
         * @return true return true if successful
         * @return false return false if failed
         */
        bool waitForClient(int timeoutMS = IPC_TIMEOUT_SIMULATOR_WAIT_REGISTER);
        /**
         * @brief  init IPC handle (socket file descriptor) of client
         * @return true return true if successful
         * @return false return false if failed
         */
        bool initClient();
        /**
         * @brief connect to server
         * @param[in] remoteIP remote TCP IP
         * @param[in] port TCP port
         * @return true return true if successful
         * @return false return false if failed
         */
        bool connectToServer(const std::string remoteIP = IPC_SOCKET_SERVER_IP, int port = IPC_SOCKET_LISTEN_PORT);
        /**
         * @brief the polling for transferring packets from the send and receive queue
         * @param[in] timeoutMS timeout duration in milliseconds
         * @return true return true if successful
         * @return false return false if failed
         */
        bool pollTransferData(int timeoutMS = IPC_TIMEOUT_RECEIVE_DATA);
        /**
         * @brief manage the polling for transferring packets from the send and receive queues
         * @param[in] data transferring packets
         * @param[in] sockfd IPC handle (socket file descriptor)
         * @param[in] state \ref IPCProcess
         * @return true return true if successful
         * @return false return false if failed
         */
        bool manageIPCProcess(std::string &data, int sockfd, IPCProcess state = IPCProcess::RECEIVE);
        /**
         * @brief get all IPC handles (socket file descriptor) for communication with simulation nodes and simulator
         * @return std::vector<int>
         */
        std::vector<int> getSockfdsActive() const;
        /**
         * @brief get all IPC handles (socket file descriptor) for communication with control and display interfaces
         * @return std::vector<int>
         */
        std::vector<int> getSockfdsUIActive() const;
        /**
         * @brief get the IPC handle (socket file descriptor) for the simulation node's communication with simulator
         * @return int IPC handle (socket file descriptor)
         */
        int getSockfdClient() const;
        /**
         * @brief get the IPC handle (socket file descriptor) for the simulator's communication with all simulation nodes
         * @return int IPC handle (socket file descriptor)
         */
        int getSockfdServer() const;
        /**
         * @brief get the IPC handle (socket file descriptor) for the simulator's communication with all remote control and display interfaces
         * @return int IPC handle (socket file descriptor)
         */
        int getSockfdUIServer() const;
        /**
         * @brief determines whether the current IPC handle (socket file descriptor) is active
         * @param[in] sockfd the socket file descriptor
         * @return true return true if active
         * @return false return false if not active
         */
        bool isActive(int sockfd);
        /**
         * @brief stop polling transfer packets from the send and receive queue
         * @return true return true if successful
         * @return false return false if failed
         * @attention this function will kill the poll immediately
         */
        bool terminate();

    private:
        std::atomic<bool> stopPoll; ///< stop polling transfer packets from the send and receive queue if true
        SOCKET_FD sockfdServer;     ///< The IPC handle (socket file descriptor) for the simulator's communication with all simulation nodes
        SOCKET_FD sockfdClient;     ///< The IPC handle (socket file descriptor) for the simulation node's communication with simulator
        SOCKET_FD sockfdUIServer;   ///< The IPC handle (socket file descriptor) for the simulator's communication with all remote control and display interfaces
        /**
         * @brief the structure used to configure and manage the monitoring of socket file descriptors
         */
        struct pollfd fds[IPC_SOCKET_LISTEN_MAX_CLIENTS + 1];
        /**
         * @brief In the I/O multiplexing mode, the number of monitored IPC handles (socket file descriptor)
         */
        std::atomic<int> fdsNumsActive;
        /**
         * @brief The send and receive queues queues for active IPC handles (socket file descriptor),  storing communication data with all simulation nodes
         */
        std::map<SOCKET_FD, std::vector<std::queue<std::string>>> sockfdsActive;
        /**
         * @brief The send and receive queues associated with active IPC handles (socket file descriptor), storing communication data with all remote control and display interfaces
         */
        std::map<SOCKET_FD, std::vector<std::queue<std::string>>> sockfdsUIActive;
        /**
         * @brief cache the data received by active IPC handles (socket file descriptor)
         */
        std::map<SOCKET_FD, std::string> sockfdsReceiveData;
        /**
         * @brief push the packet into the send queue
         * @param[in] data packet about to be sent
         * @param[in] sockfd the socket file descriptor
         * @return true return true if successful
         * @return false return false if failed
         */
        bool pushSendPacket(std::string &data, int sockfd);
        /**
         * @brief get the front packet from the send queue
         * @param[in] data the front packet from the send queue
         * @param[in] sockfd the socket file descriptor
         * @return true return true if successful
         * @return false return false if failed
         */
        bool getSendPacket(std::string &data, int sockfd);
        /**
         * @brief add header for packet about to be sent
         * @param[in] data packet about to be sent
         * @param[in] padChar the char of the padding
         */
        void addHeader(std::string &data, char padChar = '0');
        /**
         * @brief send data to the corresponding handle (socket file descriptor)
         * @param[in] data packet about to be sent
         * @param[in] sockfd the socket file descriptor
         * @return true return true if successful
         * @return false return false if failed
         */
        bool sendData(std::string &data, int sockfd);
        /**
         * @brief push the packet into the receive queue
         * @param[in] data  packet has been received
         * @param[in] sockfd the socket file descriptor
         * @return true return true if successful
         * @return false return false if failed
         */
        bool pushReceivePacket(std::string &data, int sockfd);
        /**
         * @brief get the front packet from the receive queue
         * @param[in] data the front packet from the receive queue
         * @param[in] sockfd the socket file descriptor
         * @return true return true if successful
         * @return false return false if failed
         */
        bool getReceivePacket(std::string &data, int sockfd);
        /**
         * @brief pop the front packet from the send or receive queue
         * @param[in] sockfd the socket file descriptor
         * @param[in] type \ref SOCKET_SEND_DATA the send queue \ref SOCKET_RECEIVE_DATA the receive queue
         * @return true return true if successful
         * @return false return false if failed
         */
        bool popPacket(int sockfd, int type = SOCKET_SEND_DATA);
        /**
         * @brief close one IPC handle (socket file descriptor)
         * @param[in] sockfd the socket file descriptor
         */
        void closeSocket(SOCKET_FD &sockfd);
    };

    /**
     * @brief responsible for data exchange with the simulation node, as well as the remote control and display interface, to complete the entire simulation
     */
    class SimulatorCommunication
    {
    private:
        TCPSocket simulatorIPC;    ///< one instance of an IPC process
        std::map<int, size_t> ipc; ///< one map of nodeIDs to handles
        IPCState pollState;        ///< the status of the IPC process
        bool stopPollThread;       ///< stop polling transfer packets from the send and receive queue if true

    public:
        SimulatorCommunication(/* args */);
        ~SimulatorCommunication();

    private:
        /**
         * @brief poll transfer packets from the send and receive queue
         * @param[in] timeoutMS timeout duration in milliseconds
         * @return true return true if successful
         * @return false return false if failed
         */
        bool pollTransferData(int timeoutMS = IPC_TIMEOUT_RECEIVE_DATA);

    public:
        /**
         * @brief init the IPC process
         * @return true return true if successful
         * @return false return false if failed
         */
        bool init();
        /**
         * @brief execute the IPC process
         * @param[in] timeoutMS timeout duration in milliseconds
         * @return true return true if successful
         * @return false return false if failed
         */
        bool run(int timeoutMS = IPC_TIMEOUT_RECEIVE_DATA);
        /**
         * @brief exit the IPC process
         * @return true return true if successful
         * @return false return false if failed
         * @attention this function will kill the IPC process gracefully
         */
        bool exit();
        /**
         * @brief terminate the IPC process
         * @return true return true if successful
         * @return false return false if failed
         * @attention this function will kill the IPC process immediately
         */
        bool terminate();
        /**
         * @brief push the packet into the send queue
         * @param[in] nodeID the ID of simulation node
         * @param[in] data packet about to be sent
         * @return true return true if successful
         * @return false return false if failed
         */
        bool pushSendPacket(int nodeID, std::string data);
        /**
         * @brief push the packet into the send queue
         * @param[in] handle the IPC handle (socket file descriptor)
         * @param[in] data packet about to be sent
         * @return true return true if successful
         * @return false return false if failed
         */
        bool pushSendPacketByHandle(int handle, std::string data);
        /**
         * @brief get the front packet from the receive queue
         * @param[in] nodeID the ID of simulation node
         * @param[in] data the front packet from the receive queue
         * @return true return true if successful
         * @return false return false if failed
         * @attention considering that multiple nodeIDs may map to a single IPC handle (socket file descriptor), where one socket corresponds to a single send and receive queue, the packet retrieved from the same socket may not align with the current nodeID. The specific node can only be identified by analyzing the packet itself
         */
        bool getReceivePacket(int nodeID, std::string &data);
        /**
         * @brief pop the front packet from the receive queue
         * @param[in] nodeID the ID of simulation node
         * @return true return true if successful
         * @return false return false if failed
         * @attention considering that multiple nodeIDs may map to a single IPC handle (socket file descriptor), where one socket corresponds to a single send and receive queue, the discarded packet may not correspond to the current nodeID. The specific node can only be identified by analyzing the packet itself
         */
        bool popReceivePacket(int nodeID);
        /**
         * @brief get all IPC handles (socket file descriptor) for communication with simulation nodes
         * @return std::vector<int>
         */
        std::vector<int> getIPCHandles();
        /**
         * @brief get all IPC handles (socket file descriptor) for communication with control and display interfaces
         * @return std::vector<int>
         */
        std::vector<int> getUIHandles();
        /**
         * @brief map the node to the communication handle
         * @param[in] nodeID the ID of simulation node
         * @param[in] handle the IPC handle (socket file descriptor)
         * @return true return true if successful
         * @return false return false if failed
         */
        bool bindNodeIDAndIPCHandle(int nodeID, size_t handle);
        /**
         * @brief get the status of the IPC process
         * @return \ref IPCState
         */
        IPCState getPollState() const;
    };

    /**
     * @brief responsible for exchanging information of other nodes with the simulator so that the entire simulation runs smoothly
     */
    class NodeCommunication
    {
    private:
        TCPSocket nodeIPC;   ///< one instance of an IPC process
        int sockfd;          ///< one IPC handle (socket file descriptor)
        IPCState pollState;  ///< the status of the IPC process
        bool stopPollThread; ///< stop polling transfer packets from the send and receive queue if true

    public:
        NodeCommunication(/* args */);
        ~NodeCommunication();

    private:
        /**
         * @brief poll transfer packets from the send and receive queue
         * @param[in] timeoutMS timeout duration in milliseconds
         * @return true return true if successful
         * @return false return false if failed
         */
        bool pollTransferData(int timeoutMS = IPC_TIMEOUT_RECEIVE_DATA);

    public:
        /**
         * @brief init the IPC process
         * @return true return true if successful
         * @return false return false if failed
         */
        bool init();
        /**
         * @brief execute the IPC process
         * @param[in] timeoutMS timeout duration in milliseconds
         * @return true return true if successful
         * @return false return false if failed
         */
        bool run(int timeoutMS = IPC_TIMEOUT_RECEIVE_DATA);
        /**
         * @brief exit the IPC process
         * @return true return true if successful
         * @return false return false if failed
         * @attention this function will kill the IPC process gracefully
         */
        bool exit();
        /**
         * @brief terminate the IPC process
         * @return true return true if successful
         * @return false return false if failed
         * @attention this function will kill the IPC process immediately
         */
        bool terminate();
        /**
         * @brief push the packet into the send queue
         * @param[in] data packet about to be sent
         * @return true return true if successful
         * @return false return false if failed
         */
        bool pushSendPacket(std::string data);
        /**
         * @brief get the front packet from the receive queue
         * @param[in] data the front packet from the receive queue
         * @return true return true if successful
         * @return false return false if failed
         * @attention considering that multiple simulation nodes may share a single IPC process instance, it is not possible to determine which node a packet in the send and receive queue belongs to. The specific node can only be identified by analyzing the packet itself
         */
        bool getReceivePacket(std::string &data);
        /**
         * @brief pop the front packet from the receive queue
         * @return true return true if successful
         * @return false return false if failed
         * @attention considering that multiple simulation nodes may share a single IPC process instance, it is not possible to determine which node a packet in the send and receive queue belongs to. The specific node can only be identified by analyzing the packet itself
         */
        bool popReceivePacket();
        /**
         * @brief get the IPC handles (socket file descriptor) for communication with simulator
         * @return std::vector<int>
         * @attention at most one IPC handle
         */
        std::vector<int> getIPCHandles();
        /**
         * @brief get the status of the IPC process
         * @return \ref IPCState
         */
        IPCState getPollState() const;
    };

} // namespace simu

#endif // COMMUNICATION_H