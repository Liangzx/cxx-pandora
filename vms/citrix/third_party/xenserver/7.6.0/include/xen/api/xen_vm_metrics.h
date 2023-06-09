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


#ifndef XEN_VM_METRICS_H
#define XEN_VM_METRICS_H

#include "xen_domain_type_internal.h"
#include <xen/api/xen_common.h>
#include <xen/api/xen_domain_type.h>
#include <xen/api/xen_int_float_map.h>
#include <xen/api/xen_int_int_map.h>
#include <xen/api/xen_int_string_set_map.h>
#include <xen/api/xen_string_set.h>
#include <xen/api/xen_string_string_map.h>
#include <xen/api/xen_vm_metrics_decl.h>
#include <xen/api/xen_vm_metrics_xen_vm_metrics_record_map.h>


/*
 * The VM_metrics class.
 * 
 * The metrics associated with a VM.
 */


/**
 * Free the given xen_vm_metrics.  The given handle must have been
 * allocated by this library.
 */
extern void
xen_vm_metrics_free(xen_vm_metrics vm_metrics);


typedef struct xen_vm_metrics_set
{
    size_t size;
    xen_vm_metrics *contents[];
} xen_vm_metrics_set;

/**
 * Allocate a xen_vm_metrics_set of the given size.
 */
extern xen_vm_metrics_set *
xen_vm_metrics_set_alloc(size_t size);

/**
 * Free the given xen_vm_metrics_set.  The given set must have been
 * allocated by this library.
 */
extern void
xen_vm_metrics_set_free(xen_vm_metrics_set *set);


typedef struct xen_vm_metrics_record
{
    xen_vm_metrics handle;
    char *uuid;
    int64_t memory_actual;
    int64_t vcpus_number;
    xen_int_float_map *vcpus_utilisation;
    xen_int_int_map *vcpus_cpu;
    xen_string_string_map *vcpus_params;
    xen_int_string_set_map *vcpus_flags;
    struct xen_string_set *state;
    time_t start_time;
    time_t install_time;
    time_t last_updated;
    xen_string_string_map *other_config;
    bool hvm;
    bool nested_virt;
    bool nomigrate;
    enum xen_domain_type current_domain_type;
} xen_vm_metrics_record;

/**
 * Allocate a xen_vm_metrics_record.
 */
extern xen_vm_metrics_record *
xen_vm_metrics_record_alloc(void);

/**
 * Free the given xen_vm_metrics_record, and all referenced values. 
 * The given record must have been allocated by this library.
 */
extern void
xen_vm_metrics_record_free(xen_vm_metrics_record *record);


typedef struct xen_vm_metrics_record_opt
{
    bool is_record;
    union
    {
        xen_vm_metrics handle;
        xen_vm_metrics_record *record;
    } u;
} xen_vm_metrics_record_opt;

/**
 * Allocate a xen_vm_metrics_record_opt.
 */
extern xen_vm_metrics_record_opt *
xen_vm_metrics_record_opt_alloc(void);

/**
 * Free the given xen_vm_metrics_record_opt, and all referenced values.
 *  The given record_opt must have been allocated by this library.
 */
extern void
xen_vm_metrics_record_opt_free(xen_vm_metrics_record_opt *record_opt);


typedef struct xen_vm_metrics_record_set
{
    size_t size;
    xen_vm_metrics_record *contents[];
} xen_vm_metrics_record_set;

/**
 * Allocate a xen_vm_metrics_record_set of the given size.
 */
extern xen_vm_metrics_record_set *
xen_vm_metrics_record_set_alloc(size_t size);

/**
 * Free the given xen_vm_metrics_record_set, and all referenced values.
 *  The given set must have been allocated by this library.
 */
extern void
xen_vm_metrics_record_set_free(xen_vm_metrics_record_set *set);



typedef struct xen_vm_metrics_record_opt_set
{
    size_t size;
    xen_vm_metrics_record_opt *contents[];
} xen_vm_metrics_record_opt_set;

/**
 * Allocate a xen_vm_metrics_record_opt_set of the given size.
 */
extern xen_vm_metrics_record_opt_set *
xen_vm_metrics_record_opt_set_alloc(size_t size);

/**
 * Free the given xen_vm_metrics_record_opt_set, and all referenced
 * values.  The given set must have been allocated by this library.
 */
extern void
xen_vm_metrics_record_opt_set_free(xen_vm_metrics_record_opt_set *set);


/**
 * Get a record containing the current state of the given VM_metrics.
 */
extern bool
xen_vm_metrics_get_record(xen_session *session, xen_vm_metrics_record **result, xen_vm_metrics vm_metrics);


/**
 * Get a reference to the VM_metrics instance with the specified UUID.
 */
extern bool
xen_vm_metrics_get_by_uuid(xen_session *session, xen_vm_metrics *result, char *uuid);


/**
 * Get the uuid field of the given VM_metrics.
 */
extern bool
xen_vm_metrics_get_uuid(xen_session *session, char **result, xen_vm_metrics vm_metrics);


/**
 * Get the memory/actual field of the given VM_metrics.
 */
extern bool
xen_vm_metrics_get_memory_actual(xen_session *session, int64_t *result, xen_vm_metrics vm_metrics);


/**
 * Get the VCPUs/number field of the given VM_metrics.
 */
extern bool
xen_vm_metrics_get_vcpus_number(xen_session *session, int64_t *result, xen_vm_metrics vm_metrics);


/**
 * Get the VCPUs/utilisation field of the given VM_metrics.
 */
extern bool
xen_vm_metrics_get_vcpus_utilisation(xen_session *session, xen_int_float_map **result, xen_vm_metrics vm_metrics);


/**
 * Get the VCPUs/CPU field of the given VM_metrics.
 */
extern bool
xen_vm_metrics_get_vcpus_cpu(xen_session *session, xen_int_int_map **result, xen_vm_metrics vm_metrics);


/**
 * Get the VCPUs/params field of the given VM_metrics.
 */
extern bool
xen_vm_metrics_get_vcpus_params(xen_session *session, xen_string_string_map **result, xen_vm_metrics vm_metrics);


/**
 * Get the VCPUs/flags field of the given VM_metrics.
 */
extern bool
xen_vm_metrics_get_vcpus_flags(xen_session *session, xen_int_string_set_map **result, xen_vm_metrics vm_metrics);


/**
 * Get the state field of the given VM_metrics.
 */
extern bool
xen_vm_metrics_get_state(xen_session *session, struct xen_string_set **result, xen_vm_metrics vm_metrics);


/**
 * Get the start_time field of the given VM_metrics.
 */
extern bool
xen_vm_metrics_get_start_time(xen_session *session, time_t *result, xen_vm_metrics vm_metrics);


/**
 * Get the install_time field of the given VM_metrics.
 */
extern bool
xen_vm_metrics_get_install_time(xen_session *session, time_t *result, xen_vm_metrics vm_metrics);


/**
 * Get the last_updated field of the given VM_metrics.
 */
extern bool
xen_vm_metrics_get_last_updated(xen_session *session, time_t *result, xen_vm_metrics vm_metrics);


/**
 * Get the other_config field of the given VM_metrics.
 */
extern bool
xen_vm_metrics_get_other_config(xen_session *session, xen_string_string_map **result, xen_vm_metrics vm_metrics);


/**
 * Get the hvm field of the given VM_metrics.
 */
extern bool
xen_vm_metrics_get_hvm(xen_session *session, bool *result, xen_vm_metrics vm_metrics);


/**
 * Get the nested_virt field of the given VM_metrics.
 */
extern bool
xen_vm_metrics_get_nested_virt(xen_session *session, bool *result, xen_vm_metrics vm_metrics);


/**
 * Get the nomigrate field of the given VM_metrics.
 */
extern bool
xen_vm_metrics_get_nomigrate(xen_session *session, bool *result, xen_vm_metrics vm_metrics);


/**
 * Get the current_domain_type field of the given VM_metrics.
 */
extern bool
xen_vm_metrics_get_current_domain_type(xen_session *session, enum xen_domain_type *result, xen_vm_metrics vm_metrics);


/**
 * Set the other_config field of the given VM_metrics.
 */
extern bool
xen_vm_metrics_set_other_config(xen_session *session, xen_vm_metrics vm_metrics, xen_string_string_map *other_config);


/**
 * Add the given key-value pair to the other_config field of the given
 * VM_metrics.
 */
extern bool
xen_vm_metrics_add_to_other_config(xen_session *session, xen_vm_metrics vm_metrics, char *key, char *value);


/**
 * Remove the given key and its corresponding value from the
 * other_config field of the given VM_metrics.  If the key is not in that Map,
 * then do nothing.
 */
extern bool
xen_vm_metrics_remove_from_other_config(xen_session *session, xen_vm_metrics vm_metrics, char *key);


/**
 * Return a list of all the VM_metrics instances known to the system.
 */
extern bool
xen_vm_metrics_get_all(xen_session *session, struct xen_vm_metrics_set **result);


/**
 * Return a map of VM_metrics references to VM_metrics records for all
 * VM_metrics instances known to the system.
 */
extern bool
xen_vm_metrics_get_all_records(xen_session *session, xen_vm_metrics_xen_vm_metrics_record_map **result);


#endif
