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
#include <poll.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/queue.h>
#include <linux/netlink.h>
#include <sys/types.h>
#include <unistd.h>

#include <Log.h>
#include <penwell/PnwHotplugControl.h>

namespace android {
namespace intel {

static Log& log = Log::getInstance();

PnwHotplugControl::PnwHotplugControl()
    : HotplugControl(),
      mUeventFd(-1)
{
    log.v("PnwHotplugControl");

    // init uevent socket
    struct sockaddr_nl addr;
    int sz = 64*1024;

    memset(&addr, 0, sizeof(addr));
    addr.nl_family = AF_NETLINK;
    addr.nl_pid =  pthread_self() | getpid();
    addr.nl_groups = 0xffffffff;

    mUeventFd = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_KOBJECT_UEVENT);
    if(mUeventFd < 0) {
        log.d("PnwHotplugControl: failed create uevent sockect");
        return;
    }

    setsockopt(mUeventFd, SOL_SOCKET, SO_RCVBUFFORCE, &sz, sizeof(sz));

    if(bind(mUeventFd, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        close(mUeventFd);
        mUeventFd = -1;
        return;
    }

    memset(mUeventMessage, 0, UEVENT_MSG_LEN);
}

PnwHotplugControl::~PnwHotplugControl()
{
    // close socket
    close(mUeventFd);
    mUeventFd = -1;
}

bool PnwHotplugControl::wait(int disp, int& event)
{
    struct pollfd fds;
    int nr;

    // event is useless
    event = 0;

    fds.fd = mUeventFd;
    fds.events = POLLIN;
    fds.revents = 0;
    nr = poll(&fds, 1, -1);

    if(nr > 0 && fds.revents == POLLIN) {
        int count = recv(mUeventFd, mUeventMessage, UEVENT_MSG_LEN - 2, 0);
        if (count > 0)
            return isHotplugEvent(mUeventMessage, UEVENT_MSG_LEN - 2);
    }

    return false;
}

bool PnwHotplugControl::isHotplugEvent(const char *msg, int msgLen)
{
    if (strcmp(msg, "change@/devices/pci0000:00/0000:00:02.0/drm/card0"))
        return false;

    msg += strlen(msg) + 1;

    do {
        if (!strncmp(msg, "HOTPLUG=1", strlen("HOTPLUG=1"))) {
            return true;
        }
        msg += strlen(msg) + 1;
    } while (*msg);

    return false;
}

} // namespace intel
} // namespace android
