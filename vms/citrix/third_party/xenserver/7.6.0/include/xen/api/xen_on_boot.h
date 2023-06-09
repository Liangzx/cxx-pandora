/*
 * Copyright (c) Citrix Systems, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   1) Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *
 *   2) Redistributions in binary form must reproduce the above
 *      copyright notice, this list of conditions and the following
 *      disclaimer in the documentation and/or other materials
 *      provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#ifndef XEN_ON_BOOT_H
#define XEN_ON_BOOT_H


#include <xen/api/xen_common.h>


enum xen_on_boot
{
    /**
     * When a VM containing this VDI is started, the contents of
     * the VDI are reset to the state they were in when this flag was last set.
     */
    XEN_ON_BOOT_RESET,

    /**
     * Standard behaviour.
     */
    XEN_ON_BOOT_PERSIST,

    /**
     * Unknown to this version of the bindings.
     */
    XEN_ON_BOOT_UNDEFINED
};


typedef struct xen_on_boot_set
{
    size_t size;
    enum xen_on_boot contents[];
} xen_on_boot_set;

/**
 * Allocate a xen_on_boot_set of the given size.
 */
extern xen_on_boot_set *
xen_on_boot_set_alloc(size_t size);

/**
 * Free the given xen_on_boot_set.  The given set must have been
 * allocated by this library.
 */
extern void
xen_on_boot_set_free(xen_on_boot_set *set);


/**
 * Return the name corresponding to the given code.  This string must
 * not be modified or freed.
 */
extern const char *
xen_on_boot_to_string(enum xen_on_boot val);


/**
 * Return the correct code for the given string, or set the session
 * object to failure and return an undefined value if the given string does
 * not match a known code.
 */
extern enum xen_on_boot
xen_on_boot_from_string(xen_session *session, const char *str);


#endif
