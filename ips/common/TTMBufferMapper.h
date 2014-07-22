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
#ifndef TTMBUFFERMAPPER_H_
#define TTMBUFFERMAPPER_H_

#include <DataBuffer.h>
#include <BufferMapper.h>
#include <ips/common/Wsbm.h>

namespace android {
namespace intel {

class TTMBufferMapper : public BufferMapper {
public:
    TTMBufferMapper(Wsbm& wsbm, DataBuffer& buffer);
    virtual ~TTMBufferMapper();
public:
    bool map();
    bool unmap();

    uint32_t getGttOffsetInPage(int /* subIndex */) const {
        return mGttOffsetInPage;
    }
    void* getCpuAddress(int /* subIndex */) const {
        return mCpuAddress;
    }
    uint32_t getSize(int /* subIndex */) const {
        return mSize;
    }
    uint32_t getKHandle(int /* subIndex */) {
        return 0;
    }
    uint32_t getFbHandle(int /* subIndex */) {
        return 0;
    }
    void putFbHandle() {
        return;
    }

    // wait idle
    bool waitIdle();
private:
    int mRefCount;
    Wsbm& mWsbm;
    void* mBufferObject;

    // mapped info
    uint32_t mGttOffsetInPage;
    void* mCpuAddress;
    uint32_t mSize;
};

} //namespace intel
} //namespace android


#endif /* TTMBUFFERMAPPER_H_ */
