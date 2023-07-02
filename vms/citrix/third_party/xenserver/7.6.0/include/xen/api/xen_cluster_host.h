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


#ifndef XEN_CLUSTER_HOST_H
#define XEN_CLUSTER_HOST_H

#include "xen_cluster_host_operation_internal.h"
#include "xen_string_cluster_host_operation_map_internal.h"
#include <xen/api/xen_cluster.h>
#include <xen/api/xen_cluster_decl.h>
#include <xen/api/xen_cluster_host_decl.h>
#include <xen/api/xen_cluster_host_operation.h>
#include <xen/api/xen_cluster_host_xen_cluster_host_record_map.h>
#include <xen/api/xen_common.h>
#include <xen/api/xen_host.h>
#include <xen/api/xen_host_decl.h>
#include <xen/api/xen_pif.h>
#include <xen/api/xen_pif_decl.h>
#include <xen/api/xen_string_cluster_host_operation_map.h>
#include <xen/api/xen_string_string_map.h>
#include <xen/api/xen_task_decl.h>


/*
 * The Cluster_host class.
 * 
 * Cluster member metadata.
 */


/**
 * Free the given xen_cluster_host.  The given handle must have been
 * allocated by this library.
 */
extern void
xen_cluster_host_free(xen_cluster_host cluster_host);


typedef struct xen_cluster_host_set
{
    size_t size;
    xen_cluster_host *contents[];
} xen_cluster_host_set;

/**
 * Allocate a xen_cluster_host_set of the given size.
 */
extern xen_cluster_host_set *
xen_cluster_host_set_alloc(size_t size);

/**
 * Free the given xen_cluster_host_set.  The given set must have been
 * allocated by this library.
 */
extern void
xen_cluster_host_set_free(xen_cluster_host_set *set);


typedef struct xen_cluster_host_record
{
    xen_cluster_host handle;
    char *uuid;
    struct xen_cluster_record_opt *cluster;
    struct xen_host_record_opt *host;
    bool enabled;
    struct xen_pif_record_opt *pif;
    bool joined;
    struct xen_cluster_host_operation_set *allowed_operations;
    xen_string_cluster_host_operation_map *current_operations;
    xen_string_string_map *other_config;
} xen_cluster_host_record;

/**
 * Allocate a xen_cluster_host_record.
 */
extern xen_cluster_host_record *
xen_cluster_host_record_alloc(void);

/**
 * Free the given xen_cluster_host_record, and all referenced values. 
 * The given record must have been allocated by this library.
 */
extern void
xen_cluster_host_record_free(xen_cluster_host_record *record);


typedef struct xen_cluster_host_record_opt
{
    bool is_record;
    union
    {
        xen_cluster_host handle;
        xen_cluster_host_record *record;
    } u;
} xen_cluster_host_record_opt;

/**
 * Allocate a xen_cluster_host_record_opt.
 */
extern xen_cluster_host_record_opt *
xen_cluster_host_record_opt_alloc(void);

/**
 * Free the given xen_cluster_host_record_opt, and all referenced
 * values.  The given record_opt must have been allocated by this library.
 */
extern void
xen_cluster_host_record_opt_free(xen_cluster_host_record_opt *record_opt);


typedef struct xen_cluster_host_record_set
{
    size_t size;
    xen_cluster_host_record *contents[];
} xen_cluster_host_record_set;

/**
 * Allocate a xen_cluster_host_record_set of the given size.
 */
extern xen_cluster_host_record_set *
xen_cluster_host_record_set_alloc(size_t size);

/**
 * Free the given xen_cluster_host_record_set, and all referenced
 * values.  The given set must have been allocated by this library.
 */
extern void
xen_cluster_host_record_set_free(xen_cluster_host_record_set *set);



typedef struct xen_cluster_host_record_opt_set
{
    size_t size;
    xen_cluster_host_record_opt *contents[];
} xen_cluster_host_record_opt_set;

/**
 * Allocate a xen_cluster_host_record_opt_set of the given size.
 */
extern xen_cluster_host_record_opt_set *
xen_cluster_host_record_opt_set_alloc(size_t size);

/**
 * Free the given xen_cluster_host_record_opt_set, and all referenced
 * values.  The given set must have been allocated by this library.
 */
extern void
xen_cluster_host_record_opt_set_free(xen_cluster_host_record_opt_set *set);


/**
 * Get a record containing the current state of the given Cluster_host.
 */
extern bool
xen_cluster_host_get_record(xen_session *session, xen_cluster_host_record **result, xen_cluster_host cluster_host);


/**
 * Get a reference to the Cluster_host instance with the specified
 * UUID.
 */
extern bool
xen_cluster_host_get_by_uuid(xen_session *session, xen_cluster_host *result, char *uuid);


/**
 * Get the uuid field of the given Cluster_host.
 */
extern bool
xen_cluster_host_get_uuid(xen_session *session, char **result, xen_cluster_host cluster_host);


/**
 * Get the cluster field of the given Cluster_host.
 */
extern bool
xen_cluster_host_get_cluster(xen_session *session, xen_cluster *result, xen_cluster_host cluster_host);


/**
 * Get the host field of the given Cluster_host.
 */
extern bool
xen_cluster_host_get_host(xen_session *session, xen_host *result, xen_cluster_host cluster_host);


/**
 * Get the enabled field of the given Cluster_host.
 */
extern bool
xen_cluster_host_get_enabled(xen_session *session, bool *result, xen_cluster_host cluster_host);


/**
 * Get the PIF field of the given Cluster_host.
 */
extern bool
xen_cluster_host_get_pif(xen_session *session, xen_pif *result, xen_cluster_host cluster_host);


/**
 * Get the joined field of the given Cluster_host.
 */
extern bool
xen_cluster_host_get_joined(xen_session *session, bool *result, xen_cluster_host cluster_host);


/**
 * Get the allowed_operations field of the given Cluster_host.
 */
extern bool
xen_cluster_host_get_allowed_operations(xen_session *session, struct xen_cluster_host_operation_set **result, xen_cluster_host cluster_host);


/**
 * Get the current_operations field of the given Cluster_host.
 */
extern bool
xen_cluster_host_get_current_operations(xen_session *session, xen_string_cluster_host_operation_map **result, xen_cluster_host cluster_host);


/**
 * Get the other_config field of the given Cluster_host.
 */
extern bool
xen_cluster_host_get_other_config(xen_session *session, xen_string_string_map **result, xen_cluster_host cluster_host);


/**
 * Add a new host to an existing cluster.
 */
extern bool
xen_cluster_host_create(xen_session *session, xen_cluster_host *result, xen_cluster cluster, xen_host host, xen_pif pif);

/**
 * Add a new host to an existing cluster.
 */
extern bool
xen_cluster_host_create_async(xen_session *session, xen_task *result, xen_cluster cluster, xen_host host, xen_pif pif);


/**
 * Remove a host from an existing cluster.
 */
extern bool
xen_cluster_host_destroy(xen_session *session, xen_cluster_host self);

/**
 * Remove a host from an existing cluster.
 */
extern bool
xen_cluster_host_destroy_async(xen_session *session, xen_task *result, xen_cluster_host self);


/**
 * Enable cluster membership for a disabled cluster host.
 */
extern bool
xen_cluster_host_enable(xen_session *session, xen_cluster_host self);

/**
 * Enable cluster membership for a disabled cluster host.
 */
extern bool
xen_cluster_host_enable_async(xen_session *session, xen_task *result, xen_cluster_host self);


/**
 * Remove a host from an existing cluster forcefully.
 */
extern bool
xen_cluster_host_force_destroy(xen_session *session, xen_cluster_host self);

/**
 * Remove a host from an existing cluster forcefully.
 */
extern bool
xen_cluster_host_force_destroy_async(xen_session *session, xen_task *result, xen_cluster_host self);


/**
 * Disable cluster membership for an enabled cluster host.
 */
extern bool
xen_cluster_host_disable(xen_session *session, xen_cluster_host self);

/**
 * Disable cluster membership for an enabled cluster host.
 */
extern bool
xen_cluster_host_disable_async(xen_session *session, xen_task *result, xen_cluster_host self);


/**
 * Return a list of all the Cluster_hosts known to the system.
 */
extern bool
xen_cluster_host_get_all(xen_session *session, struct xen_cluster_host_set **result);


/**
 * Return a map of Cluster_host references to Cluster_host records for
 * all Cluster_hosts known to the system.
 */
extern bool
xen_cluster_host_get_all_records(xen_session *session, xen_cluster_host_xen_cluster_host_record_map **result);


#endif
