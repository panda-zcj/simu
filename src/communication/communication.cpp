/**
 * @author: pikaqiu_zcj
 * @date: 2024-07-22
 * @par lastEditors: pikaqiu_zcj
 * @par lastEditTime: 2024-08-13
 * @file: \src\communication\communication.cpp
 * @message:
 * @copyright (c) 2024 by pikaqiu, All Rights Reserved.
 */

#include "communication.h"
#include <iostream>
#include <thread>
#include <mutex>
#ifdef __linux__
#include <cstring>
#endif
using namespace simu;

std::mutex mtx; // 定义一个互斥锁
const std::string simu::ipcState[] = {"INIT", "RUNNING", "STOP"};

TCPSocket::TCPSocket() : sockfdClient(-1), sockfdServer(-1), sockfdUIServer(-1)
{
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        LOG_CRIT << "WSAStartup failed";
    }
#endif
    fdsNumsActive.store(0);
    stopPoll.store(false);
}

TCPSocket::~TCPSocket()
{
    closeSocket(sockfdServer);
    closeSocket(sockfdClient);
    closeSocket(sockfdUIServer);
    for (int i = 0; i < fdsNumsActive.load(); ++i)
    {
        closeSocket(fds[i].fd);
    }
#ifdef _WIN32
    WSACleanup();
#endif
}

bool TCPSocket::initServer(int port, std::string serverIP, int backlog)
{
    sockfdServer = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfdServer == -1)
    {
        LOG_CRIT << "Server Socket creation failed";
        return false;
    }

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(serverIP.c_str());
    address.sin_port = htons(port);

    if (bind(sockfdServer, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        LOG_CRIT << "Server Bind failed";
        closeSocket(sockfdServer);
        return false;
    }

    if (listen(sockfdServer, backlog) < 0)
    {
        LOG_CRIT << "Server Listen failed";
        closeSocket(sockfdServer);
        return false;
    }

    LOG_DEBUG << "init server success, sockfdServer = " << sockfdServer;

    LOG_DEBUG << "init ui server ......";

    sockfdUIServer = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfdUIServer == -1)
    {
        LOG_CRIT << "UIServer Socket creation failed";
        return false;
    }

    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(serverIP.c_str());
    address.sin_port = htons(UI_SOCKET_LISTEN_PORT);

    if (bind(sockfdUIServer, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        LOG_CRIT << "UIServer Bind failed";
        closeSocket(sockfdUIServer);
        return false;
    }

    if (listen(sockfdUIServer, backlog) < 0)
    {
        LOG_CRIT << "UIServer Listen failed";
        closeSocket(sockfdUIServer);
        return false;
    }

    fds[fdsNumsActive.load()].fd = sockfdUIServer;
    fds[fdsNumsActive.load()].events = POLLIN;
    fdsNumsActive++;

    LOG_DEBUG << "init ui server success, sockfdUIServer = " << sockfdUIServer;

    return true;
}

bool TCPSocket::waitForClient(int timeoutMS)
{
    struct pollfd fdsServer[2];
    fdsServer[0].fd = sockfdServer;
    fdsServer[0].events = POLLIN;
    fdsServer[1].fd = sockfdUIServer;
    fdsServer[1].events = POLLIN;
    while (true)
    {
#ifdef _WIN32
        int poll_count = WSAPoll(fdsServer, 2, timeoutMS);
#else
        int poll_count = poll(fdsServer, 2, timeoutMS);
#endif
        if (poll_count < 0)
        {
            LOG_CRIT << "Poll failed";
            return false;
        }
        else if (poll_count == 0)
        {
            LOG_WARN << "Poll timeout occurred, " << "the numbers of actived sockets (nodes) is " << sockfdsActive.size() << ", the numbers of actived sockets (UI) is " << sockfdsUIActive.size();
#if (SIMULATION_REMOTE_CONFIG_ENABLE == DISABLE)
            if (sockfdsActive.size() == 0)
            {
                return false;
            }
#else
            if (sockfdsActive.size() == 0 || sockfdsUIActive.size() == 0)
            {
                return false;
            }
#endif
            break;
        }

        for (int i = 0; i < 2; ++i)
        {
            if (fdsServer[i].revents == 0)
            {
                continue;
            }
            else if (fdsServer[i].revents & POLLIN)
            {
                int client_fd = -1;
                struct sockaddr_in client_addr;
                socklen_t client_len = sizeof(client_addr);
                if ((client_fd = accept(sockfdServer, (struct sockaddr *)&client_addr, (socklen_t *)&client_len)) < 0)
                {
                    LOG_CRIT << "Accept failed";
                    return false;
                }
                if (fdsServer[i].fd == sockfdUIServer)
                {
                    // Add the new client socket to the fds array
                    LOG_INFO << "New ui client accepted, sockfd(" << client_fd << ")";
                    sockfdsUIActive[client_fd] = std::vector<std::queue<std::string>>(2);
                    sockfdsReceiveData[client_fd] = "";
                    fds[fdsNumsActive.load()].fd = client_fd;
                    fds[fdsNumsActive.load()].events = POLLIN | POLLOUT;
                    fdsNumsActive++;
                }
                else
                {
                    // Add the new client socket to the fds array
                    LOG_INFO << "New connection accepted, sockfdClients[" << sockfdsActive.size() << "] = " << client_fd;
                    sockfdsActive[client_fd] = std::vector<std::queue<std::string>>(2);
                    sockfdsReceiveData[client_fd] = "";
                    fds[fdsNumsActive.load()].fd = client_fd;
                    fds[fdsNumsActive.load()].events = POLLIN | POLLOUT;
                    fdsNumsActive++;
                }
            }
            else
            {
                LOG_CRIT << "Unexpected poll event, revents = " << fdsServer[i].revents << ", fd = " << fdsServer[i].fd;
                return false;
            }
        }
    }
    return true;
}

bool TCPSocket::initClient()
{
    sockfdClient = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfdClient == -1)
    {
        LOG_CRIT << "Failed to create socket";
        return false;
    }

    LOG_DEBUG << "init client success, sockfdClient(" << sockfdClient << ")";

    return true;
}

bool TCPSocket::connectToServer(const std::string remoteIP, int port)
{
    // 设置服务器地址信息
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(remoteIP.c_str());

    // 连接服务器
    if (connect(sockfdClient, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        LOG_CRIT << "Connection failed";
        closeSocket(sockfdClient);
        return false;
    }

    LOG_INFO << "Connection server success, sockfdClient(" << sockfdClient << ")";
    sockfdsActive[sockfdClient] = std::vector<std::queue<std::string>>(2);
    sockfdsReceiveData[sockfdClient] = "";
    fds[fdsNumsActive.load()].fd = sockfdClient;
    fds[fdsNumsActive.load()].events = POLLIN | POLLOUT;
    fdsNumsActive++;

    return true;
}

void TCPSocket::addHeader(std::string &data, char padChar)
{
    std::string input = std::to_string(data.length() + SOCKET_DATA_HEADER_WIDTH);
    if (input.length() >= SOCKET_DATA_HEADER_WIDTH)
    {
        input = input.substr(0, SOCKET_DATA_HEADER_WIDTH); // 截断超出部分
    }
    else
    {
        input.insert(0, std::string(SOCKET_DATA_HEADER_WIDTH - input.length(), padChar)); // 在前面填充字符
    }

    data.insert(0, input);
}

bool TCPSocket::popPacket(int sockfd, int type)
{
    if (sockfdsActive.find(sockfd) == sockfdsActive.end() && sockfdsUIActive.find(sockfd) == sockfdsUIActive.end())
    {
        LOG_CRIT << "sockfd(" << sockfd << ")" << " is not exist";
        return false;
    }
    if (sockfdsActive.find(sockfd) != sockfdsActive.end() && !sockfdsActive[sockfd][type].empty())
    {
        LOG_DEBUG << "pop " << (type == SOCKET_RECEIVE_DATA ? "receive" : "send") << " packet is \"" << sockfdsActive[sockfd][type].front() << "\"";
        sockfdsActive[sockfd][type].pop();
    }
    else if (sockfdsUIActive.find(sockfd) != sockfdsUIActive.end() && !sockfdsUIActive[sockfd][type].empty())
    {
        LOG_DEBUG << "pop " << (type == SOCKET_RECEIVE_DATA ? "receive" : "send") << " packet is \"" << sockfdsUIActive[sockfd][type].front() << "\"";
        sockfdsUIActive[sockfd][type].pop();
    }
    else
    {
        LOG_CRIT << "sockfd(" << sockfd << ")" << " is not " << (type == SOCKET_RECEIVE_DATA ? "receive" : "send") << " packet";
        return false;
    }

    return true;
}

bool TCPSocket::pushSendPacket(std::string &data, int sockfd)
{
    bool ret = true;
    if (sockfdsActive.find(sockfd) != sockfdsActive.end())
    {
        sockfdsActive[sockfd][SOCKET_SEND_DATA].push(data);
    }
    else if (sockfdsUIActive.find(sockfd) != sockfdsUIActive.end())
    {
        sockfdsUIActive[sockfd][SOCKET_SEND_DATA].push(data);
    }
    else
    {
        ret = false;
        LOG_CRIT << "sockfd(" << sockfd << ")" << " is not exist";
    }

    return ret;
}

bool TCPSocket::getSendPacket(std::string &data, int sockfd)
{
    if (sockfdsActive.find(sockfd) == sockfdsActive.end() && sockfdsUIActive.find(sockfd) == sockfdsUIActive.end())
    {
        LOG_CRIT << "sockfd(" << sockfd << ")" << " is not exist";
        return false;
    }
    bool ret = true;
    if (sockfdsActive.find(sockfd) != sockfdsActive.end() && !sockfdsActive[sockfd][SOCKET_SEND_DATA].empty())
    {
        data = sockfdsActive[sockfd][SOCKET_SEND_DATA].front();
    }
    else if (sockfdsUIActive.find(sockfd) != sockfdsUIActive.end() && !sockfdsUIActive[sockfd][SOCKET_SEND_DATA].empty())
    {
        data = sockfdsUIActive[sockfd][SOCKET_SEND_DATA].front();
    }
    else
    {
        ret = false;
    }

    return ret;
}

bool TCPSocket::sendData(std::string &data, int sockfd)
{
    addHeader(data);
    ssize_t total_sent = 0;
    ssize_t len = data.length();

    LOG_DEBUG << "socket fd(" << sockfd << ") send \"" << data << "\"";

    while (total_sent < len)
    {
        ssize_t sent = send(sockfd, data.c_str() + total_sent, len - total_sent, 0);
        if (sent == -1)
        {
            LOG_CRIT << "sockfd(" << sockfd << ") send failed";
            return false;
        }
        total_sent += sent;
    }

    popPacket(sockfd);
    return true;
}

bool TCPSocket::pushReceivePacket(std::string &data, int sockfd)
{
    LOG_DEBUG << "socket fd(" << sockfd << ") receive \"" << data << "\"";
    int receive_bytes = 0;
    while (data.size() > SOCKET_DATA_HEADER_WIDTH)
    {
        receive_bytes = std::stoi(data.substr(0, SOCKET_DATA_HEADER_WIDTH));
        if (receive_bytes > data.length())
        {
            return false;
        }
        if (sockfdsUIActive.find(sockfd) != sockfdsUIActive.end())
        {
            sockfdsUIActive[sockfd][SOCKET_RECEIVE_DATA].push(data.substr(SOCKET_DATA_HEADER_WIDTH, receive_bytes - SOCKET_DATA_HEADER_WIDTH));
        }
        else if (sockfdsActive.find(sockfd) != sockfdsActive.end())
        {
            sockfdsActive[sockfd][SOCKET_RECEIVE_DATA].push(data.substr(SOCKET_DATA_HEADER_WIDTH, receive_bytes - SOCKET_DATA_HEADER_WIDTH));
        }
        data.erase(0, receive_bytes);
    }
    return data.empty();
}

bool TCPSocket::getReceivePacket(std::string &data, int sockfd)
{
    if (sockfdsActive.find(sockfd) == sockfdsActive.end() && sockfdsUIActive.find(sockfd) == sockfdsUIActive.end())
    {
        LOG_CRIT << "sockfd(" << sockfd << ")" << " is not exist";
        return false;
    }
    bool ret = true;
    if (sockfdsActive.find(sockfd) != sockfdsActive.end() && !sockfdsActive[sockfd][SOCKET_RECEIVE_DATA].empty())
    {
        data = sockfdsActive[sockfd][SOCKET_RECEIVE_DATA].front();
        popPacket(sockfd, SOCKET_RECEIVE_DATA);
    }
    else if (sockfdsUIActive.find(sockfd) != sockfdsUIActive.end() && !sockfdsUIActive[sockfd][SOCKET_RECEIVE_DATA].empty())
    {
        data = sockfdsUIActive[sockfd][SOCKET_RECEIVE_DATA].front();
        popPacket(sockfd, SOCKET_RECEIVE_DATA);
    }
    else
    {
        ret = false;
    }

    return ret;
}

bool TCPSocket::manageIPCProcess(std::string &data, int sockfd, IPCProcess state)
{
    std::lock_guard<std::mutex> lock(mtx);
    bool ret = true;
    if (state == IPCProcess::RECEIVE)
    {
        ret = pushReceivePacket(data, sockfd);
    }
    else if (state == IPCProcess::SEND)
    {
        if (stopPoll.load() == true)
        {
            LOG_DEBUG << "poll thread socket is not exist";
            return false;
        }
        ret = getSendPacket(data, sockfd) && sendData(data, sockfd);
    }
    else if (state == IPCProcess::ACCESS)
    {
        ret = getReceivePacket(data, sockfd);
    }
    else if (state == IPCProcess::POP)
    {
        ret = popPacket(sockfd, SOCKET_RECEIVE_DATA);
    }
    else if (state == IPCProcess::PUSH)
    {
        if (stopPoll.load() == true)
        {
            LOG_DEBUG << "poll thread socket is not exist";
            return false;
        }
        ret = pushSendPacket(data, sockfd);
    }
    else if (state == IPCProcess::EXIT)
    {
        stopPoll.store(true);
    }

    return ret;
}

bool TCPSocket::pollTransferData(int timeoutMS)
{
    if (fdsNumsActive.load() == 0)
    {
        LOG_WARN << "no actived socket";
        return false;
    }
    while (true)
    {
#ifdef _WIN32
        int poll_count = WSAPoll(fds, fdsNumsActive.load(), timeoutMS);
#else
        int poll_count = poll(fds, fdsNumsActive.load(), timeoutMS);
#endif
        if (poll_count < 0)
        {
            LOG_CRIT << "Poll failed";
            return false;
        }
        else if (poll_count == 0)
        {
            LOG_WARN << "Poll timeout occurred";
            return false;
        }

        for (int i = 0; i < fdsNumsActive.load(); ++i)
        {
            if (fds[i].revents & POLLIN)
            {
                if (fds[i].fd == sockfdUIServer && sockfdUIServer != -1)
                {
                    int client_fd = -1;
                    struct sockaddr_in client_addr;
                    socklen_t client_len = sizeof(client_addr);
                    if ((client_fd = accept(sockfdUIServer, (struct sockaddr *)&client_addr, (socklen_t *)&client_len)) < 0)
                    {
                        LOG_CRIT << "Accept failed";
                        return false;
                    }

                    // Add the new client socket to the fds array
                    LOG_INFO << "New ui client accepted, sockfd(" << client_fd << ")";
                    sockfdsUIActive[client_fd] = std::vector<std::queue<std::string>>(2);
                    sockfdsReceiveData[client_fd] = "";
                    fds[fdsNumsActive.load()].fd = client_fd;
                    fds[fdsNumsActive.load()].events = POLLIN | POLLOUT;
                    fdsNumsActive++;
                    continue;
                }

                char buffer[1024] = {0};
                int valread = recv(fds[i].fd, buffer, sizeof(buffer), 0);
                if (valread == 0)
                {
                    LOG_CRIT << "Peer disconnected, sockfd(" << fds[i].fd << ")";
                    if (sockfdsActive.find(fds[i].fd) != sockfdsActive.end())
                    {
                        stopPoll.store(true);
                    }
                    sockfdsReceiveData.erase(fds[i].fd);
                    closeSocket(fds[i].fd);
                }
                else if (valread < 0)
                {
                    LOG_CRIT << "receive error, sockfd(" << fds[i].fd << ")";
                }
                else
                {
                    sockfdsReceiveData[fds[i].fd].append(buffer, valread);
                    manageIPCProcess(sockfdsReceiveData[fds[i].fd], fds[i].fd);
                }
            }
            if (fds[i].revents & POLLOUT)
            {
                std::string data;
                manageIPCProcess(data, fds[i].fd, IPCProcess::SEND);
            }
            if (fds[i].revents & (POLLERR | POLLNVAL | POLLHUP))
            {
                LOG_CRIT << "Peer disconnected error, sockfd(" << fds[i].fd << ")";
                if ((fds[i].fd == sockfdUIServer && sockfdUIServer != -1) || (fds[i].fd == sockfdClient && sockfdClient != -1))
                {
                    LOG_CRIT << "terminate poll thread, stopPoll(" << (stopPoll.load() ? "true" : "false") << ")" << ", the numbers of listen sockets is " << fdsNumsActive.load();
                    return false;
                }
                if (sockfdsActive.find(fds[i].fd) != sockfdsActive.end())
                {
                    stopPoll.store(true);
                }
                sockfdsReceiveData.erase(fds[i].fd);
                closeSocket(fds[i].fd);
            }
        }

        // Remove closed connections from the fds array
        for (int i = 0; i < fdsNumsActive.load(); ++i)
        {
            if (fds[i].fd == -1)
            {
                fds[i].fd = fds[fdsNumsActive.load() - 1].fd;
                fds[i].events = fds[fdsNumsActive.load() - 1].events;
                fdsNumsActive--;
                i--;
            }
        }

        if (stopPoll.load())
        {
            LOG_INFO << "exit poll thread, stopPoll(" << (stopPoll.load() ? "true" : "false") << ")" << ", the numbers of listen sockets is " << fdsNumsActive.load();
            return false;
        }
    }
    return true;
}

std::vector<int> TCPSocket::getSockfdsActive() const
{
    std::vector<int> fds;
    for (auto it = sockfdsActive.begin(); it != sockfdsActive.end(); ++it)
    {
        fds.push_back(it->first);
    }
    return fds;
}

std::vector<int> TCPSocket::getSockfdsUIActive() const
{
    std::vector<int> fds;
    for (auto it = sockfdsUIActive.begin(); it != sockfdsUIActive.end(); ++it)
    {
        fds.push_back(it->first);
    }
    return fds;
}

int TCPSocket::getSockfdClient() const
{
    return sockfdClient;
}

int TCPSocket::getSockfdServer() const
{
    return sockfdServer;
}

int TCPSocket::getSockfdUIServer() const
{
    return sockfdUIServer;
}

bool TCPSocket::isActive(int sockfd)
{
    for (int i = 0; i < fdsNumsActive.load(); ++i)
    {
        if (fds[i].fd == sockfd)
        {
            return true;
        }
    }
    LOG_DEBUG << "sockfd(" << sockfd << ")" << " is not exist or closed";
    return false;
}

void TCPSocket::closeSocket(SOCKET_FD &sockfd)
{
    std::lock_guard<std::mutex> lock(mtx);
    if (sockfd != -1)
    {
        LOG_DEBUG << "close sockfd(" << sockfd << ")";
#ifdef _WIN32
        closesocket(sockfd);
#else
        close(sockfd);
#endif
        sockfd = -1;
    }
}

bool TCPSocket::terminate()
{
    closeSocket(sockfdUIServer);
    closeSocket(sockfdClient);
    return true;
}

SimulatorCommunication::SimulatorCommunication(/* args */) : stopPollThread(false), pollState(IPCState::INIT)
{
}

SimulatorCommunication::~SimulatorCommunication()
{
}

bool SimulatorCommunication::init()
{
    return simulatorIPC.initServer() && simulatorIPC.waitForClient();
}

bool SimulatorCommunication::run(int timeoutMS)
{
    if (pollState == IPCState::RUNNING)
    {
        LOG_DEBUG << "current IPC state is (pollState = " << ipcState[static_cast<int>(pollState)] << ")";
        return false;
    }
    stopPollThread = false;
    pollState = IPCState::INIT;
    if (pollTransferData(timeoutMS))
    {
        while (true)
        {
            if (pollState == IPCState::RUNNING)
            {
                return true;
            }
            else if (pollState == IPCState::STOP)
            {
                return false;
            }
        }
    }
    return false;
}

bool SimulatorCommunication::terminate()
{
    stopPollThread = true;
    simulatorIPC.terminate();
    while (pollState == IPCState::RUNNING)
    {
        /* code */
    }
    return true;
}

bool SimulatorCommunication::exit()
{
    stopPollThread = true;
    std::string data;
    simulatorIPC.manageIPCProcess(data, -1, IPCProcess::EXIT);
    while (pollState == IPCState::RUNNING)
    {
        /* code */
    }
    return true;
}

bool SimulatorCommunication::pollTransferData(int timeoutMS)
{
    auto task = [&, timeoutMS]()
    {
        pollState = IPCState::RUNNING;
        LOG_DEBUG << "start poll exchange data (pollState = " << ipcState[static_cast<int>(pollState)] << ", stopPollThread = " << (stopPollThread ? "true" : "false") << ")";
        while (!stopPollThread)
        {
            if (!simulatorIPC.pollTransferData(timeoutMS))
            {
                break;
            }
        };
        pollState = IPCState::STOP;
        stopPollThread = true;
        LOG_DEBUG << "stop poll exchange data (pollState = " << ipcState[static_cast<int>(pollState)] << ", stopPollThread  = " << (stopPollThread ? "true" : "false") << ")";
    };
    std::thread t(task);
    t.detach();
    return true;
}

bool SimulatorCommunication::pushSendPacket(int nodeID, std::string data)
{
    return (ipc.find(nodeID) != ipc.end()) && simulatorIPC.manageIPCProcess(data, ipc[nodeID], IPCProcess::PUSH);
}

bool SimulatorCommunication::pushSendPacketByHandle(int handle, std::string data)
{
    return simulatorIPC.isActive(handle) && simulatorIPC.manageIPCProcess(data, handle, IPCProcess::PUSH);
}

bool SimulatorCommunication::getReceivePacket(int nodeID, std::string &data)
{
    return (ipc.find(nodeID) != ipc.end()) && simulatorIPC.manageIPCProcess(data, ipc[nodeID], IPCProcess::ACCESS);
}

bool SimulatorCommunication::popReceivePacket(int nodeID)
{
    std::string data;
    return (ipc.find(nodeID) != ipc.end()) && simulatorIPC.manageIPCProcess(data, ipc[nodeID], IPCProcess::POP);
}

std::vector<int> SimulatorCommunication::getIPCHandles()
{
    return simulatorIPC.getSockfdsActive();
}

std::vector<int> SimulatorCommunication::getUIHandles()
{
    return simulatorIPC.getSockfdsUIActive();
}

bool SimulatorCommunication::bindNodeIDAndIPCHandle(int nodeID, size_t handle)
{
    if (simulatorIPC.isActive(handle))
    {
        if (ipc.find(nodeID) == ipc.end())
        {
            ipc[nodeID] = handle;
        }
        LOG_DEBUG << "NodeID(" << nodeID << ") <=> handle(" << ipc[nodeID] << ")";
        return true;
    }
    return false;
}

IPCState SimulatorCommunication::getPollState() const
{
    return pollState;
}

NodeCommunication::NodeCommunication(/* args */) : sockfd(-1), stopPollThread(false), pollState(IPCState::INIT)
{
}

NodeCommunication::~NodeCommunication()
{
}

bool NodeCommunication::init()
{
    return nodeIPC.initClient() && nodeIPC.connectToServer() && (sockfd = nodeIPC.getSockfdClient());
}

bool NodeCommunication::run(int timeoutMS)
{
    if (pollState == IPCState::RUNNING)
    {
        LOG_DEBUG << "current IPC state is (pollState = " << ipcState[static_cast<int>(pollState)] << ")";
        return false;
    }
    stopPollThread = false;
    pollState = IPCState::INIT;
    if (pollTransferData(timeoutMS))
    {
        while (true)
        {
            if (pollState == IPCState::RUNNING)
            {
                return true;
            }
            else if (pollState == IPCState::STOP)
            {
                return false;
            }
        }
    }
    return false;
}

bool NodeCommunication::terminate()
{
    stopPollThread = true;
    nodeIPC.terminate();
    while (pollState == IPCState::RUNNING)
    {
        /* code */
    }
    return true;
}

bool NodeCommunication::exit()
{
    stopPollThread = true;
    std::string data;
    nodeIPC.manageIPCProcess(data, -1, IPCProcess::EXIT);
    while (pollState == IPCState::RUNNING)
    {
        /* code */
    }
    return true;
}

bool NodeCommunication::pollTransferData(int timeoutMS)
{
    auto task = [&, timeoutMS]()
    {
        pollState = IPCState::RUNNING;
        LOG_DEBUG << "start poll exchange data (pollState = " << ipcState[static_cast<int>(pollState)] << ", stopPollThread = " << (stopPollThread ? "true" : "false") << ")";
        while (!stopPollThread)
        {
            if (!nodeIPC.pollTransferData(timeoutMS))
            {
                break;
            }
        };
        pollState = IPCState::STOP;
        stopPollThread = true;
        LOG_DEBUG << "stop poll exchange data (pollState = " << ipcState[static_cast<int>(pollState)] << ", stopPollThread = " << (stopPollThread ? "true" : "false") << ")";
    };
    std::thread t(task);
    t.detach();
    return true;
}

bool NodeCommunication::pushSendPacket(std::string data)
{
    return nodeIPC.manageIPCProcess(data, sockfd, IPCProcess::PUSH);
}

bool NodeCommunication::getReceivePacket(std::string &data)
{
    return nodeIPC.manageIPCProcess(data, sockfd, IPCProcess::ACCESS);
}

bool NodeCommunication::popReceivePacket()
{
    std::string data;
    return nodeIPC.manageIPCProcess(data, sockfd, IPCProcess::POP);
}

std::vector<int> NodeCommunication::getIPCHandles()
{
    return nodeIPC.getSockfdsActive();
}

IPCState NodeCommunication::getPollState() const
{
    return pollState;
}