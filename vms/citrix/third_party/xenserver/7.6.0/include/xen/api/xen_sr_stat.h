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


#ifndef XEN_SR_STAT_H
#define XEN_SR_STAT_H

#include "xen_sr_health_internal.h"
#include <xen/api/xen_common.h>
#include <xen/api/xen_sr_health.h>
#include <xen/api/xen_sr_stat_decl.h>


/*
 * The sr_stat class.
 * 
 * A set of high-level properties associated with an SR.
 */


/**
 * Free the given xen_sr_stat.  The given handle must have been
 * allocated by this library.
 */
extern void
xen_sr_stat_free(xen_sr_stat sr_stat);


typedef struct xen_sr_stat_set
{
    size_t size;
    xen_sr_stat *contents[];
} xen_sr_stat_set;

/**
 * Allocate a xen_sr_stat_set of the given size.
 */
extern xen_sr_stat_set *
xen_sr_stat_set_alloc(size_t size);

/**
 * Free the given xen_sr_stat_set.  The given set must have been
 * allocated by this library.
 */
extern void
xen_sr_stat_set_free(xen_sr_stat_set *set);


typedef struct xen_sr_stat_record
{
    xen_sr_stat handle;
    char *uuid;
    char *name_label;
    char *name_description;
    int64_t free_space;
    int64_t total_space;
    bool clustered;
    enum xen_sr_health health;
} xen_sr_stat_record;

/**
 * Allocate a xen_sr_stat_record.
 */
extern xen_sr_stat_record *
xen_sr_stat_record_alloc(void);

/**
 * Free the given xen_sr_stat_record, and all referenced values.  The
 * given record must have been allocated by this library.
 */
extern void
xen_sr_stat_record_free(xen_sr_stat_record *record);


typedef struct xen_sr_stat_record_opt
{
    bool is_record;
    union
    {
        xen_sr_stat handle;
        xen_sr_stat_record *record;
    } u;
} xen_sr_stat_record_opt;

/**
 * Allocate a xen_sr_stat_record_opt.
 */
extern xen_sr_stat_record_opt *
xen_sr_stat_record_opt_alloc(void);

/**
 * Free the given xen_sr_stat_record_opt, and all referenced values. 
 * The given record_opt must have been allocated by this library.
 */
extern void
xen_sr_stat_record_opt_free(xen_sr_stat_record_opt *record_opt);


typedef struct xen_sr_stat_record_set
{
    size_t size;
    xen_sr_stat_record *contents[];
} xen_sr_stat_record_set;

/**
 * Allocate a xen_sr_stat_record_set of the given size.
 */
extern xen_sr_stat_record_set *
xen_sr_stat_record_set_alloc(size_t size);

/**
 * Free the given xen_sr_stat_record_set, and all referenced values. 
 * The given set must have been allocated by this library.
 */
extern void
xen_sr_stat_record_set_free(xen_sr_stat_record_set *set);



typedef struct xen_sr_stat_record_opt_set
{
    size_t size;
    xen_sr_stat_record_opt *contents[];
} xen_sr_stat_record_opt_set;

/**
 * Allocate a xen_sr_stat_record_opt_set of the given size.
 */
extern xen_sr_stat_record_opt_set *
xen_sr_stat_record_opt_set_alloc(size_t size);

/**
 * Free the given xen_sr_stat_record_opt_set, and all referenced
 * values.  The given set must have been allocated by this library.
 */
extern void
xen_sr_stat_record_opt_set_free(xen_sr_stat_record_opt_set *set);




#endif
