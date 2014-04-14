/*
 * Copyright © 2012 Intel Corporation
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 * Authors:
 *    Jackie Li <yaodong.li@intel.com>
 *
 */
#ifndef WSBM_H__
#define WSBM_H__

#include <common/WsbmWrapper.h>

/**
 * Class: WSBM
 * A wrapper class to use libwsbm functionalities
 */
class Wsbm
{
private:
    int mDrmFD;
public:
    Wsbm(int drmFD);
    ~Wsbm();
    bool initialize();
    void deinitialize();
    bool allocateTTMBuffer(uint32_t size, uint32_t align,void ** buf);
    bool allocateTTMBufferUB(uint32_t size, uint32_t align, void ** buf, void *user_pt);
    bool destroyTTMBuffer(void * buf);
    void * getCPUAddress(void * buf);
    uint32_t getGttOffset(void * buf);
    bool wrapTTMBuffer(uint32_t handle, void **buf);
    bool unreferenceTTMBuffer(void *buf);
    bool waitIdleTTMBuffer(void *buf);
    uint32_t getKBufHandle(void *buf);
private:
    bool mInitialized;
};

#endif /*__INTEL_WSBM_H__*/
