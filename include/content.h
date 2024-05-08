/*
 * File: content.h
 * Created Date: Sunday, December 18th 2022
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

#ifndef CONTENT_H
#define CONTENT_H

#include "object.h"

namespace simu
{
    class Content : public Object
    {
    private:
        /* data */

    public:
        /**
         * @attention derived classes and father classes Construct function must private
         */
        Content();
        ~Content();
        /**
         * @brief create transparentData
         * @attention the function of derived classes must override if add transparentDatas
         * @return Object*
         */
        static Content *createContent(Object);
    };

} // namespace simu

#endif // CONTENT_H