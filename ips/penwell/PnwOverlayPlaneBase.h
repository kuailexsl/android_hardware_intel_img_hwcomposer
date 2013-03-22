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
#ifndef PNW_OVERLAY_PLANE_BASE_H
#define PNW_OVERLAY_PLANE_BASE_H

#include <utils/KeyedVector.h>
#include <hal_public.h>
#include <DisplayPlane.h>
#include <DisplayDevice.h>
#include <BufferMapper.h>
#include <penwell/Wsbm.h>
#include <penwell/PnwOverlayHardware.h>
#include <penwell/PnwVideoPayloadBuffer.h>

namespace android {
namespace intel {

typedef struct {
    OverlayBackBufferBlk *buf;
    uint32_t gttOffsetInPage;
    uint32_t bufObject;
} OverlayBackBuffer;

class PnwOverlayPlaneBase : public DisplayPlane {
    // flush flags
    enum {
        FLUSH_NEEDED     = 0x00000001UL,
        WAIT_VBLANK      = 0x00000002UL,
        UPDATE_COEF      = 0x00000004UL,
        BOB_DEINTERLACE  = 0x00000008UL,
        DELAY_DISABLE    = 0x00000010UL,
        WMS_NEEDED       = 0x00000020UL,
    };
public:
    PnwOverlayPlaneBase(int index, int disp);
    ~PnwOverlayPlaneBase();

    virtual bool isValidBuffer(uint32_t handle);
    virtual bool isValidTransform(uint32_t trans);
    virtual bool isValidBlending(uint32_t blending);
    virtual bool isValidScaling(hwc_rect_t& src, hwc_rect_t& dest);
    virtual void invalidateBufferCache();

    virtual bool assignToDevice(int disp);

    virtual void setZOrderConfig(ZOrderConfig& config);

    // plane operations
    virtual bool flip() = 0;
    virtual bool reset();
    virtual bool enable();
    virtual bool disable();

    virtual void* getContext() const = 0;
    virtual bool initialize();
protected:
    // generic overlay register flush
    virtual bool flush(uint32_t flags);
    virtual bool setDataBuffer(BufferMapper& mapper);
    virtual bool bufferOffsetSetup(BufferMapper& mapper);
    virtual uint32_t calculateSWidthSW(uint32_t offset, uint32_t width);
    virtual bool coordinateSetup(BufferMapper& mapper);
    virtual bool setCoeffRegs(double *coeff, int mantSize,
                                 coeffPtr pCoeff, int pos);
    virtual void updateCoeff(int taps, double fCutoff,
                                bool isHoriz, bool isY,
                                coeffPtr pCoeff);
    virtual bool scalingSetup(BufferMapper& mapper);
    virtual void checkPosition(int& x, int& y, int& w, int& h);
protected:
    // back buffer operations
    virtual OverlayBackBuffer* createBackBuffer();
    virtual void deleteBackBuffer();
    virtual void resetBackBuffer();

    virtual BufferMapper* getTTMMapper(BufferMapper& grallocMapper);
    virtual bool rotatedBufferReady(BufferMapper& mapper);
    
protected:
    // TTM data buffer cache
    BufferCache *mTTMBufferCache;
    // overlay back buffer
    OverlayBackBuffer *mBackBuffer;
    // wsbm
    Wsbm *mWsbm;
    // pipe
    uint32_t mPipe;
};

} // namespace intel
} // namespace android

#endif /* PNW_OVERLAY_PLANE_BASE_H */

