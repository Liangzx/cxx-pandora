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


#ifndef XEN_VMSS_H
#define XEN_VMSS_H

#include "xen_vmss_frequency_internal.h"
#include "xen_vmss_type_internal.h"
#include <xen/api/xen_common.h>
#include <xen/api/xen_string_string_map.h>
#include <xen/api/xen_task_decl.h>
#include <xen/api/xen_vm.h>
#include <xen/api/xen_vm_decl.h>
#include <xen/api/xen_vmss_decl.h>
#include <xen/api/xen_vmss_frequency.h>
#include <xen/api/xen_vmss_type.h>
#include <xen/api/xen_vmss_xen_vmss_record_map.h>


/*
 * The VMSS class.
 * 
 * VM Snapshot Schedule.
 */


/**
 * Free the given xen_vmss.  The given handle must have been allocated
 * by this library.
 */
extern void
xen_vmss_free(xen_vmss vmss);


typedef struct xen_vmss_set
{
    size_t size;
    xen_vmss *contents[];
} xen_vmss_set;

/**
 * Allocate a xen_vmss_set of the given size.
 */
extern xen_vmss_set *
xen_vmss_set_alloc(size_t size);

/**
 * Free the given xen_vmss_set.  The given set must have been allocated
 * by this library.
 */
extern void
xen_vmss_set_free(xen_vmss_set *set);


typedef struct xen_vmss_record
{
    xen_vmss handle;
    char *uuid;
    char *name_label;
    char *name_description;
    bool enabled;
    enum xen_vmss_type type;
    int64_t retained_snapshots;
    enum xen_vmss_frequency frequency;
    xen_string_string_map *schedule;
    time_t last_run_time;
    struct xen_vm_record_opt_set *vms;
} xen_vmss_record;

/**
 * Allocate a xen_vmss_record.
 */
extern xen_vmss_record *
xen_vmss_record_alloc(void);

/**
 * Free the given xen_vmss_record, and all referenced values.  The
 * given record must have been allocated by this library.
 */
extern void
xen_vmss_record_free(xen_vmss_record *record);


typedef struct xen_vmss_record_opt
{
    bool is_record;
    union
    {
        xen_vmss handle;
        xen_vmss_record *record;
    } u;
} xen_vmss_record_opt;

/**
 * Allocate a xen_vmss_record_opt.
 */
extern xen_vmss_record_opt *
xen_vmss_record_opt_alloc(void);

/**
 * Free the given xen_vmss_record_opt, and all referenced values.  The
 * given record_opt must have been allocated by this library.
 */
extern void
xen_vmss_record_opt_free(xen_vmss_record_opt *record_opt);


typedef struct xen_vmss_record_set
{
    size_t size;
    xen_vmss_record *contents[];
} xen_vmss_record_set;

/**
 * Allocate a xen_vmss_record_set of the given size.
 */
extern xen_vmss_record_set *
xen_vmss_record_set_alloc(size_t size);

/**
 * Free the given xen_vmss_record_set, and all referenced values.  The
 * given set must have been allocated by this library.
 */
extern void
xen_vmss_record_set_free(xen_vmss_record_set *set);



typedef struct xen_vmss_record_opt_set
{
    size_t size;
    xen_vmss_record_opt *contents[];
} xen_vmss_record_opt_set;

/**
 * Allocate a xen_vmss_record_opt_set of the given size.
 */
extern xen_vmss_record_opt_set *
xen_vmss_record_opt_set_alloc(size_t size);

/**
 * Free the given xen_vmss_record_opt_set, and all referenced values. 
 * The given set must have been allocated by this library.
 */
extern void
xen_vmss_record_opt_set_free(xen_vmss_record_opt_set *set);


/**
 * Get a record containing the current state of the given VMSS.
 */
extern bool
xen_vmss_get_record(xen_session *session, xen_vmss_record **result, xen_vmss vmss);


/**
 * Get a reference to the VMSS instance with the specified UUID.
 */
extern bool
xen_vmss_get_by_uuid(xen_session *session, xen_vmss *result, char *uuid);


/**
 * Create a new VMSS instance, and return its handle.
 */
extern bool
xen_vmss_create(xen_session *session, xen_vmss *result, xen_vmss_record *record);

/**
 * Create a new VMSS instance, and return its handle.
 */
extern bool
xen_vmss_create_async(xen_session *session, xen_task *result, xen_vmss_record *record);


/**
 * Destroy the specified VMSS instance.
 */
extern bool
xen_vmss_destroy(xen_session *session, xen_vmss vmss);

/**
 * Destroy the specified VMSS instance.
 */
extern bool
xen_vmss_destroy_async(xen_session *session, xen_task *result, xen_vmss vmss);


/**
 * Get all the VMSS instances with the given label.
 */
extern bool
xen_vmss_get_by_name_label(xen_session *session, struct xen_vmss_set **result, char *label);


/**
 * Get the uuid field of the given VMSS.
 */
extern bool
xen_vmss_get_uuid(xen_session *session, char **result, xen_vmss vmss);


/**
 * Get the name/label field of the given VMSS.
 */
extern bool
xen_vmss_get_name_label(xen_session *session, char **result, xen_vmss vmss);


/**
 * Get the name/description field of the given VMSS.
 */
extern bool
xen_vmss_get_name_description(xen_session *session, char **result, xen_vmss vmss);


/**
 * Get the enabled field of the given VMSS.
 */
extern bool
xen_vmss_get_enabled(xen_session *session, bool *result, xen_vmss vmss);


/**
 * Get the type field of the given VMSS.
 */
extern bool
xen_vmss_get_type(xen_session *session, enum xen_vmss_type *result, xen_vmss vmss);


/**
 * Get the retained_snapshots field of the given VMSS.
 */
extern bool
xen_vmss_get_retained_snapshots(xen_session *session, int64_t *result, xen_vmss vmss);


/**
 * Get the frequency field of the given VMSS.
 */
extern bool
xen_vmss_get_frequency(xen_session *session, enum xen_vmss_frequency *result, xen_vmss vmss);


/**
 * Get the schedule field of the given VMSS.
 */
extern bool
xen_vmss_get_schedule(xen_session *session, xen_string_string_map **result, xen_vmss vmss);


/**
 * Get the last_run_time field of the given VMSS.
 */
extern bool
xen_vmss_get_last_run_time(xen_session *session, time_t *result, xen_vmss vmss);


/**
 * Get the VMs field of the given VMSS.
 */
extern bool
xen_vmss_get_vms(xen_session *session, struct xen_vm_set **result, xen_vmss vmss);


/**
 * Set the name/label field of the given VMSS.
 */
extern bool
xen_vmss_set_name_label(xen_session *session, xen_vmss vmss, char *label);


/**
 * Set the name/description field of the given VMSS.
 */
extern bool
xen_vmss_set_name_description(xen_session *session, xen_vmss vmss, char *description);


/**
 * Set the enabled field of the given VMSS.
 */
extern bool
xen_vmss_set_enabled(xen_session *session, xen_vmss vmss, bool enabled);


/**
 * This call executes the snapshot schedule immediately.
 */
extern bool
xen_vmss_snapshot_now(xen_session *session, char **result, xen_vmss vmss);


/**
 * .
 */
extern bool
xen_vmss_set_retained_snapshots(xen_session *session, xen_vmss self, int64_t value);


/**
 * Set the value of the frequency field.
 */
extern bool
xen_vmss_set_frequency(xen_session *session, xen_vmss self, enum xen_vmss_frequency value);


/**
 * .
 */
extern bool
xen_vmss_set_schedule(xen_session *session, xen_vmss self, xen_string_string_map *value);


/**
 * .
 */
extern bool
xen_vmss_add_to_schedule(xen_session *session, xen_vmss self, char *key, char *value);


/**
 * .
 */
extern bool
xen_vmss_remove_from_schedule(xen_session *session, xen_vmss self, char *key);


/**
 * .
 */
extern bool
xen_vmss_set_last_run_time(xen_session *session, xen_vmss self, time_t value);


/**
 * .
 */
extern bool
xen_vmss_set_type(xen_session *session, xen_vmss self, enum xen_vmss_type value);


/**
 * Return a list of all the VMSSs known to the system.
 */
extern bool
xen_vmss_get_all(xen_session *session, struct xen_vmss_set **result);


/**
 * Return a map of VMSS references to VMSS records for all VMSSs known
 * to the system.
 */
extern bool
xen_vmss_get_all_records(xen_session *session, xen_vmss_xen_vmss_record_map **result);


#endif
