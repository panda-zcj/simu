/*
 * File: Event.cpp
 * Created Date: Sunday, August 7th 2022
 * Author: pikaqiu
 * -----
 * Last Modified: Mon Dec 19 2022
 * Modified By: pikaqiu
 * -----
 * Copyright (c) 2022 Dream
 *
 * All shall be well and all shall be well and all manner of things shall be well.
 * Nope...we're doomed!
 */

#include "event.h"
#include <iostream>
using namespace simu;

Event::Event()
{
}

Event::~Event()
{
    if (this->content != nullptr)
    {
        delete this->content;
        this->content = nullptr;
    }
}

void Event::creatEvent(Time timeStamp,
                       Content *content,
                       Object *srcObject,
                       Object *dstObject,
                       unsigned int srcId,
                       unsigned int dstId,
                       std::string srcStr,
                       std::string dstStr)
{
    this->time = timeStamp;
    this->srcObject = srcObject;
    this->dstObject = dstObject;
    this->srcId = srcId;
    this->dstId = dstId;
    this->srcStr = srcStr;
    this->dstStr = dstStr;
    this->content = content;
}

EventLoop::EventLoop(unsigned int capacity)
{
    this->_eventLoop = std::vector<Event>(capacity);
    this->front = 0;
    this->rear = 0;
    this->size = 0;
    this->capacity = capacity;
}

EventLoop::~EventLoop()
{
}

bool EventLoop::registerObject(unsigned int key, Object *object)
{
    bool res = false;
    if (moc_object.find(key) == moc_object.end())
    {
        moc_object.insert(std::make_pair(key, object));
        res = true;
    }
    return res;
}

bool EventLoop::pushEvent(Event event)
{
    if (this->size >= this->capacity)
    {
        std::cout << "event over capacity" << std::endl;
        return false;
    }
    _eventLoop[this->rear] = event;
    this->rear = (this->rear + this->capacity + 1) % this->capacity;
    this->size++;
    unsigned int end = (this->front + this->capacity - 1) % this->capacity;
    unsigned int cur = (this->rear + this->capacity - 1) % this->capacity;
    unsigned int pre = (cur + this->capacity - 1) % this->capacity;
    while (pre != end)
    {
        if (_eventLoop[cur].getTimeStamp() < _eventLoop[pre].getTimeStamp())
        {
            // todo std::move
            Event temp = _eventLoop[cur];
            _eventLoop[cur] = _eventLoop[pre];
            _eventLoop[cur] = temp;
            cur = (cur + this->capacity - 1) % this->capacity;
            pre = (pre + this->capacity - 1) % this->capacity;
            continue;
        }
        break;
    }
    return true;
}

bool EventLoop::releaseEvent()
{
    this->size--;
    if (this->size < 0)
    {
        this->size = 0;
        return false;
    }
    this->front = (this->front + this->capacity + 1) % this->capacity;
    return true;
}

void EventLoop::exec(Time simuTime)
{
    // 执行事件函数
    for (; g_timeStamp < simuTime; g_timeStamp = g_timeStamp + (unsigned long)1)
    {
        /* code */
        for (int i = this->front; i < this->rear; i = (i + this->capacity + 1) % this->capacity)
        {
            if (this->_eventLoop[i].getTimeStamp() <= this->g_timeStamp)
            {
                bool res = this->_eventLoop[i].getDstObject()->processEvent(this->_eventLoop[i].getEventContent());
                // todo res 处理
                releaseEvent();
            }
        }
    }
}

EventLoop eventLoop = EventLoop();