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


#ifndef XEN_PROBE_RESULT_H
#define XEN_PROBE_RESULT_H

#include <xen/api/xen_common.h>
#include <xen/api/xen_probe_result_decl.h>
#include <xen/api/xen_sr_stat.h>
#include <xen/api/xen_string_string_map.h>


/*
 * The probe_result class.
 * 
 * A set of properties that describe one result element of SR.probe. Result
 * elements and properties can change dynamically based on changes to the the
 * SR.probe input-parameters or the target.
 */


/**
 * Free the given xen_probe_result.  The given handle must have been
 * allocated by this library.
 */
extern void
xen_probe_result_free(xen_probe_result probe_result);


typedef struct xen_probe_result_set
{
    size_t size;
    xen_probe_result *contents[];
} xen_probe_result_set;

/**
 * Allocate a xen_probe_result_set of the given size.
 */
extern xen_probe_result_set *
xen_probe_result_set_alloc(size_t size);

/**
 * Free the given xen_probe_result_set.  The given set must have been
 * allocated by this library.
 */
extern void
xen_probe_result_set_free(xen_probe_result_set *set);


typedef struct xen_probe_result_record
{
    xen_probe_result handle;
    xen_string_string_map *configuration;
    bool complete;
    struct xen_sr_stat_record *sr;
    xen_string_string_map *extra_info;
} xen_probe_result_record;

/**
 * Allocate a xen_probe_result_record.
 */
extern xen_probe_result_record *
xen_probe_result_record_alloc(void);

/**
 * Free the given xen_probe_result_record, and all referenced values. 
 * The given record must have been allocated by this library.
 */
extern void
xen_probe_result_record_free(xen_probe_result_record *record);


typedef struct xen_probe_result_record_opt
{
    bool is_record;
    union
    {
        xen_probe_result handle;
        xen_probe_result_record *record;
    } u;
} xen_probe_result_record_opt;

/**
 * Allocate a xen_probe_result_record_opt.
 */
extern xen_probe_result_record_opt *
xen_probe_result_record_opt_alloc(void);

/**
 * Free the given xen_probe_result_record_opt, and all referenced
 * values.  The given record_opt must have been allocated by this library.
 */
extern void
xen_probe_result_record_opt_free(xen_probe_result_record_opt *record_opt);


typedef struct xen_probe_result_record_set
{
    size_t size;
    xen_probe_result_record *contents[];
} xen_probe_result_record_set;

/**
 * Allocate a xen_probe_result_record_set of the given size.
 */
extern xen_probe_result_record_set *
xen_probe_result_record_set_alloc(size_t size);

/**
 * Free the given xen_probe_result_record_set, and all referenced
 * values.  The given set must have been allocated by this library.
 */
extern void
xen_probe_result_record_set_free(xen_probe_result_record_set *set);



typedef struct xen_probe_result_record_opt_set
{
    size_t size;
    xen_probe_result_record_opt *contents[];
} xen_probe_result_record_opt_set;

/**
 * Allocate a xen_probe_result_record_opt_set of the given size.
 */
extern xen_probe_result_record_opt_set *
xen_probe_result_record_opt_set_alloc(size_t size);

/**
 * Free the given xen_probe_result_record_opt_set, and all referenced
 * values.  The given set must have been allocated by this library.
 */
extern void
xen_probe_result_record_opt_set_free(xen_probe_result_record_opt_set *set);




#endif
