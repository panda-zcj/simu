/*
 * File: content.cpp
 * Created Date: Sunday, December 18th 2022
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

#include "content.h"
using namespace simu;

Content::Content()
{
}

Content::~Content()
{
}

Content *Content::createContent(/*arg*/)
{
    Content *content = new Content();
    // wait to init /*arg*/
    return content;
}