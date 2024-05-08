/*
 * File: Event.h
 * Created Date: Sunday, August 7th 2022
 * Author: pikaqiu
 * -----
 * Last Modified: Wed Dec 21 2022
 * Modified By: pikaqiu
 * -----
 * Copyright (c) 2022 Dream
 *
 * All shall be well and all shall be well and all manner of things shall be well.
 * Nope...we're doomed!
 */

#ifndef EVENT_H
#define EVENT_H

#include "content.h"
#include "times.h"
#include <vector>
#include <string>
#include <unordered_map>

namespace simu
{
    class Event
    {
    private:
        /* data */
        Time time;
        Content *content;
        Object *srcObject;
        Object *dstObject;
        unsigned int srcId;
        unsigned int dstId;
        std::string srcStr;
        std::string dstStr;

    public:
        Event();
        ~Event();

        void creatEvent(Time timeStamp,
                        Content *content = nullptr,
                        Object *srcObject = nullptr,
                        Object *dstObject = nullptr,
                        unsigned int srcId = 0,
                        unsigned int dstId = 0,
                        std::string srcStr = "",
                        std::string dstStr = "");
        Time getTimeStamp() const
        {
            return this->time;
        }
        Content *getEventContent() const
        {
            return this->content;
        }
        Object *getSrcObject() const
        {
            return this->srcObject;
        }
        Object *getDstObject() const
        {
            return this->dstObject;
        }
        unsigned int getSrcId() const
        {
            return this->srcId;
        }
        unsigned int getDstId() const
        {
            return this->dstId;
        }
        std::string getSrcStr() const
        {
            return this->srcStr;
        }
        std::string getDstStr() const
        {
            return this->dstStr;
        }
    };

    class EventLoop
    {
    private:
        /* data */
        std::vector<Event> _eventLoop;
        std::unordered_map<unsigned int, Object *> moc_object;
        unsigned int front;
        unsigned int rear;
        unsigned int size;
        unsigned int capacity;
        Time g_timeStamp;

        bool releaseEvent();

    public:
        EventLoop(unsigned int capcity = 500);
        ~EventLoop();

        bool registerObject(unsigned int key, Object *object);
        bool pushEvent(Event event);
        void exec(Time simuTime);
    };
    extern EventLoop eventLoop;

} // namespace simu

#endif // EVENT_H