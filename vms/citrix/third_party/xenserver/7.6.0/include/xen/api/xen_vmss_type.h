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


#ifndef XEN_VMSS_TYPE_H
#define XEN_VMSS_TYPE_H


#include <xen/api/xen_common.h>


enum xen_vmss_type
{
    /**
     * The snapshot is a disk snapshot
     */
    XEN_VMSS_TYPE_SNAPSHOT,

    /**
     * The snapshot is a checkpoint
     */
    XEN_VMSS_TYPE_CHECKPOINT,

    /**
     * The snapshot is a VSS
     */
    XEN_VMSS_TYPE_SNAPSHOT_WITH_QUIESCE,

    /**
     * Unknown to this version of the bindings.
     */
    XEN_VMSS_TYPE_UNDEFINED
};


typedef struct xen_vmss_type_set
{
    size_t size;
    enum xen_vmss_type contents[];
} xen_vmss_type_set;

/**
 * Allocate a xen_vmss_type_set of the given size.
 */
extern xen_vmss_type_set *
xen_vmss_type_set_alloc(size_t size);

/**
 * Free the given xen_vmss_type_set.  The given set must have been
 * allocated by this library.
 */
extern void
xen_vmss_type_set_free(xen_vmss_type_set *set);


/**
 * Return the name corresponding to the given code.  This string must
 * not be modified or freed.
 */
extern const char *
xen_vmss_type_to_string(enum xen_vmss_type val);


/**
 * Return the correct code for the given string, or set the session
 * object to failure and return an undefined value if the given string does
 * not match a known code.
 */
extern enum xen_vmss_type
xen_vmss_type_from_string(xen_session *session, const char *str);


#endif
