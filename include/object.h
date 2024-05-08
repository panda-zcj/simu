/*
 * File: object.h
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

#ifndef OBJECT_H
#define OBJECT_H

namespace simu
{
    class Object
    {
    private:
        /* data */
    public:
        Object(/* args */);
        virtual ~Object();
        virtual bool processEvent(Object *);
    };

    Object::Object(/* args */)
    {
    }

    Object::~Object()
    {
    }

    bool Object::processEvent(Object *)
    {
        return true;
    }

} // namespace simu

#endif // OBJECT_H