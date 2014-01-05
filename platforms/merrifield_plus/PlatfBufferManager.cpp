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
#include <HwcTrace.h>
#include <PlatfBufferManager.h>
#include <anniedale/AnnGrallocBuffer.h>
#include <anniedale/AnnGrallocBufferMapper.h>

namespace android {
namespace intel {

PlatfBufferManager::PlatfBufferManager()
    : BufferManager()
{

}

PlatfBufferManager::~PlatfBufferManager()
{

}

bool PlatfBufferManager::initialize()
{
    return BufferManager::initialize();
}

void PlatfBufferManager::deinitialize()
{
    BufferManager::deinitialize();
}

DataBuffer* PlatfBufferManager::createDataBuffer(gralloc_module_t *module,
                                                 uint32_t handle)
{
    return new AnnGrallocBuffer(handle);
}

BufferMapper* PlatfBufferManager::createBufferMapper(gralloc_module_t *module,
                                                        DataBuffer& buffer)
{
    if (!module)
        return 0;

    return new AnnGrallocBufferMapper(*(IMG_gralloc_module_public_t*)module,
                                        buffer);
}

bool PlatfBufferManager::convertRGBToNV12(uint32_t rgbHandle, uint32_t yuvHandle,
                                  crop_t& srcCrop, uint32_t async)

{
    IMG_gralloc_module_public_t *imgGrallocModule = (IMG_gralloc_module_public_t *) mGrallocModule;
    if (imgGrallocModule->Blit(imgGrallocModule, (buffer_handle_t)rgbHandle,
                                (buffer_handle_t)yuvHandle,
                                srcCrop.w, srcCrop.h, srcCrop.x,
                                srcCrop.y, async)) {
        ETRACE("Blit failed");
        return false;
    }
    return true;
}

} // namespace intel
} // namespace android