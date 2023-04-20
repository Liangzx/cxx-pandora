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


#ifndef XEN_LVHD_H
#define XEN_LVHD_H

#include <xen/api/xen_common.h>
#include <xen/api/xen_host_decl.h>
#include <xen/api/xen_lvhd_decl.h>
#include <xen/api/xen_sr_decl.h>
#include <xen/api/xen_task_decl.h>


/*
 * The LVHD class.
 * 
 * LVHD SR specific operations.
 */


/**
 * Free the given xen_lvhd.  The given handle must have been allocated
 * by this library.
 */
extern void
xen_lvhd_free(xen_lvhd lvhd);


typedef struct xen_lvhd_set
{
    size_t size;
    xen_lvhd *contents[];
} xen_lvhd_set;

/**
 * Allocate a xen_lvhd_set of the given size.
 */
extern xen_lvhd_set *
xen_lvhd_set_alloc(size_t size);

/**
 * Free the given xen_lvhd_set.  The given set must have been allocated
 * by this library.
 */
extern void
xen_lvhd_set_free(xen_lvhd_set *set);


typedef struct xen_lvhd_record
{
    xen_lvhd handle;
    char *uuid;
} xen_lvhd_record;

/**
 * Allocate a xen_lvhd_record.
 */
extern xen_lvhd_record *
xen_lvhd_record_alloc(void);

/**
 * Free the given xen_lvhd_record, and all referenced values.  The
 * given record must have been allocated by this library.
 */
extern void
xen_lvhd_record_free(xen_lvhd_record *record);


typedef struct xen_lvhd_record_opt
{
    bool is_record;
    union
    {
        xen_lvhd handle;
        xen_lvhd_record *record;
    } u;
} xen_lvhd_record_opt;

/**
 * Allocate a xen_lvhd_record_opt.
 */
extern xen_lvhd_record_opt *
xen_lvhd_record_opt_alloc(void);

/**
 * Free the given xen_lvhd_record_opt, and all referenced values.  The
 * given record_opt must have been allocated by this library.
 */
extern void
xen_lvhd_record_opt_free(xen_lvhd_record_opt *record_opt);


typedef struct xen_lvhd_record_set
{
    size_t size;
    xen_lvhd_record *contents[];
} xen_lvhd_record_set;

/**
 * Allocate a xen_lvhd_record_set of the given size.
 */
extern xen_lvhd_record_set *
xen_lvhd_record_set_alloc(size_t size);

/**
 * Free the given xen_lvhd_record_set, and all referenced values.  The
 * given set must have been allocated by this library.
 */
extern void
xen_lvhd_record_set_free(xen_lvhd_record_set *set);



typedef struct xen_lvhd_record_opt_set
{
    size_t size;
    xen_lvhd_record_opt *contents[];
} xen_lvhd_record_opt_set;

/**
 * Allocate a xen_lvhd_record_opt_set of the given size.
 */
extern xen_lvhd_record_opt_set *
xen_lvhd_record_opt_set_alloc(size_t size);

/**
 * Free the given xen_lvhd_record_opt_set, and all referenced values. 
 * The given set must have been allocated by this library.
 */
extern void
xen_lvhd_record_opt_set_free(xen_lvhd_record_opt_set *set);


/**
 * Get a record containing the current state of the given LVHD.
 */
extern bool
xen_lvhd_get_record(xen_session *session, xen_lvhd_record **result, xen_lvhd lvhd);


/**
 * Get a reference to the LVHD instance with the specified UUID.
 */
extern bool
xen_lvhd_get_by_uuid(xen_session *session, xen_lvhd *result, char *uuid);


/**
 * Get the uuid field of the given LVHD.
 */
extern bool
xen_lvhd_get_uuid(xen_session *session, char **result, xen_lvhd lvhd);


/**
 * Upgrades an LVHD SR to enable thin-provisioning. Future VDIs created
 * in this SR will be thinly-provisioned, although existing VDIs will be left
 * alone. Note that the SR must be attached to the SRmaster for upgrade to
 * work.
 */
extern bool
xen_lvhd_enable_thin_provisioning(xen_session *session, char **result, xen_host host, xen_sr sr, int64_t initial_allocation, int64_t allocation_quantum);

/**
 * Upgrades an LVHD SR to enable thin-provisioning. Future VDIs created
 * in this SR will be thinly-provisioned, although existing VDIs will be left
 * alone. Note that the SR must be attached to the SRmaster for upgrade to
 * work.
 */
extern bool
xen_lvhd_enable_thin_provisioning_async(xen_session *session, xen_task *result, xen_host host, xen_sr sr, int64_t initial_allocation, int64_t allocation_quantum);


#endif
