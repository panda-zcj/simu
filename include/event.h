/**
 * @author: pikaqiu_zcj
 * @date: 2022-08-07
 * @par lastEditors: pikaqiu_zcj
 * @par lastEditTime: 2024-08-10
 * @file: \include\event.h
 * @message:
 * @copyright (c) 2024 by pikaqiu, All Rights Reserved.
 */

#ifndef EVENT_H
#define EVENT_H

#include "global.h"
#include "nlohmann/json.hpp"
#include <string>
#include <atomic>

namespace simu
{
    /**
     * @brief define the format of the data packets exchanged between the simulator and the simulation nodes
     */
    class Event
    {
    private:
        /* data */
        static std::atomic<unsigned long long> __g_eventID__; ///< eventID <=> srcNodeID(eventID[63:48]) + dstNodeID(eventID[47:32]) + eventGlobalIndex(eventID[31:0])
    public:
        Event(/* args */);
        ~Event();

    private:
        /**
         * @brief convert decimal to hexadecimal
         * @param[in] num the decimal number to be converted
         * @return std::string
         */
        static std::string getHexStr(unsigned long long num);

    public:
        /**
         * @brief creat one event instance
         * @param[in] duration The duration of the event, the precision is the milliseconds
         * @param[in] maxPropagateTime the propagate delay of the event, the precision is the milliseconds
         * @param[in] content Event Content
         * @param[in] srcId ID of the Event Publisher
         * @param[in] dstId ID of the Event Receiver
         * @param[in] srcStr Detailed Description of the Event Publisher
         * @param[in] dstStr Detailed Description of the Event Receiver
         * @return nlohmann::ordered_json
         */
        static nlohmann::ordered_json getEvent(double duration,
                                               double maxPropagateTime,
                                               std::string &content,
                                               std::string &srcId,
                                               std::string &dstId,
                                               std::string srcStr = "",
                                               std::string dstStr = "");
        /**
         * @brief creat one event instance
         * @param[in] duration The duration of the event, the precision is the milliseconds
         * @param[in] maxPropagateTime the propagate delay of the event, the precision is the milliseconds
         * @param[in] content Event Content
         * @param[in] srcId ID of the Event Publisher
         * @param[in] dstId ID of the Event Receiver
         * @param[in] srcStr Detailed Description of the Event Publisher
         * @param[in] dstStr Detailed Description of the Event Receiver
         * @return nlohmann::ordered_json
         */
        static nlohmann::ordered_json getEvent(long long duration,
                                               long long maxPropagateTime,
                                               std::string &content,
                                               std::string &srcId,
                                               std::string &dstId,
                                               std::string srcStr = "",
                                               std::string dstStr = "");
        /**
         * @brief creat one event instance
         * @param[in] duration The duration of the event, the precision is the milliseconds
         * @param[in] maxPropagateTime the propagate delay of the event, the precision is the milliseconds
         * @param[in] content Event Content
         * @param[in] srcId ID of the Event Publisher
         * @param[in] dstId ID of the Event Receiver
         * @param[in] srcStr Detailed Description of the Event Publisher
         * @param[in] dstStr Detailed Description of the Event Receiver
         * @return nlohmann::ordered_json
         */
        static nlohmann::ordered_json getEvent(int duration,
                                               int maxPropagateTime,
                                               std::string &content,
                                               std::string &srcId,
                                               std::string &dstId,
                                               std::string srcStr = "",
                                               std::string dstStr = "");
    };

} // namespace simu

#endif // EVENT_H