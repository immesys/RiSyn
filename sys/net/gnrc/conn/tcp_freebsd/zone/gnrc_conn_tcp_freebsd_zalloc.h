/*
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#ifndef GNRC_CONN_TCP_FREEBSD_ZALLOC_H_
#define GNRC_CONN_TCP_FREEBSD_ZALLOC_H_

#include "memmgr.h"

static bool initialized = false;

void conn_tcp_freebsd_zone_init(void)
{
    if (!initialized) {
        initialized = true;
        memmgr_init();
    }
}

void* conn_tcp_freebsd_zalloc(unsigned long numbytes) {
    conn_tcp_freebsd_zone_init();
    return memmgr_alloc(numbytes);
}

void conn_tcp_freebsd_zfree(void* ptr) {
    assert(initialized);
    memmgr_free(ptr);
}

#endif
