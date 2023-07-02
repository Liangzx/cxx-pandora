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


#ifndef XEN_CLUSTER_H
#define XEN_CLUSTER_H

#include "xen_cluster_operation_internal.h"
#include "xen_string_cluster_operation_map_internal.h"
#include <xen/api/xen_cluster_decl.h>
#include <xen/api/xen_cluster_host.h>
#include <xen/api/xen_cluster_host_decl.h>
#include <xen/api/xen_cluster_operation.h>
#include <xen/api/xen_cluster_xen_cluster_record_map.h>
#include <xen/api/xen_common.h>
#include <xen/api/xen_network_decl.h>
#include <xen/api/xen_pif_decl.h>
#include <xen/api/xen_string_cluster_operation_map.h>
#include <xen/api/xen_string_set.h>
#include <xen/api/xen_string_string_map.h>
#include <xen/api/xen_task_decl.h>


/*
 * The Cluster class.
 * 
 * Cluster-wide Cluster metadata.
 */


/**
 * Free the given xen_cluster.  The given handle must have been
 * allocated by this library.
 */
extern void
xen_cluster_free(xen_cluster cluster);


typedef struct xen_cluster_set
{
    size_t size;
    xen_cluster *contents[];
} xen_cluster_set;

/**
 * Allocate a xen_cluster_set of the given size.
 */
extern xen_cluster_set *
xen_cluster_set_alloc(size_t size);

/**
 * Free the given xen_cluster_set.  The given set must have been
 * allocated by this library.
 */
extern void
xen_cluster_set_free(xen_cluster_set *set);


typedef struct xen_cluster_record
{
    xen_cluster handle;
    char *uuid;
    struct xen_cluster_host_record_opt_set *cluster_hosts;
    struct xen_string_set *pending_forget;
    char *cluster_token;
    char *cluster_stack;
    struct xen_cluster_operation_set *allowed_operations;
    xen_string_cluster_operation_map *current_operations;
    bool pool_auto_join;
    double token_timeout;
    double token_timeout_coefficient;
    xen_string_string_map *cluster_config;
    xen_string_string_map *other_config;
} xen_cluster_record;

/**
 * Allocate a xen_cluster_record.
 */
extern xen_cluster_record *
xen_cluster_record_alloc(void);

/**
 * Free the given xen_cluster_record, and all referenced values.  The
 * given record must have been allocated by this library.
 */
extern void
xen_cluster_record_free(xen_cluster_record *record);


typedef struct xen_cluster_record_opt
{
    bool is_record;
    union
    {
        xen_cluster handle;
        xen_cluster_record *record;
    } u;
} xen_cluster_record_opt;

/**
 * Allocate a xen_cluster_record_opt.
 */
extern xen_cluster_record_opt *
xen_cluster_record_opt_alloc(void);

/**
 * Free the given xen_cluster_record_opt, and all referenced values. 
 * The given record_opt must have been allocated by this library.
 */
extern void
xen_cluster_record_opt_free(xen_cluster_record_opt *record_opt);


typedef struct xen_cluster_record_set
{
    size_t size;
    xen_cluster_record *contents[];
} xen_cluster_record_set;

/**
 * Allocate a xen_cluster_record_set of the given size.
 */
extern xen_cluster_record_set *
xen_cluster_record_set_alloc(size_t size);

/**
 * Free the given xen_cluster_record_set, and all referenced values. 
 * The given set must have been allocated by this library.
 */
extern void
xen_cluster_record_set_free(xen_cluster_record_set *set);



typedef struct xen_cluster_record_opt_set
{
    size_t size;
    xen_cluster_record_opt *contents[];
} xen_cluster_record_opt_set;

/**
 * Allocate a xen_cluster_record_opt_set of the given size.
 */
extern xen_cluster_record_opt_set *
xen_cluster_record_opt_set_alloc(size_t size);

/**
 * Free the given xen_cluster_record_opt_set, and all referenced
 * values.  The given set must have been allocated by this library.
 */
extern void
xen_cluster_record_opt_set_free(xen_cluster_record_opt_set *set);


/**
 * Get a record containing the current state of the given Cluster.
 */
extern bool
xen_cluster_get_record(xen_session *session, xen_cluster_record **result, xen_cluster cluster);


/**
 * Get a reference to the Cluster instance with the specified UUID.
 */
extern bool
xen_cluster_get_by_uuid(xen_session *session, xen_cluster *result, char *uuid);


/**
 * Get the uuid field of the given Cluster.
 */
extern bool
xen_cluster_get_uuid(xen_session *session, char **result, xen_cluster cluster);


/**
 * Get the cluster_hosts field of the given Cluster.
 */
extern bool
xen_cluster_get_cluster_hosts(xen_session *session, struct xen_cluster_host_set **result, xen_cluster cluster);


/**
 * Get the pending_forget field of the given Cluster.
 */
extern bool
xen_cluster_get_pending_forget(xen_session *session, struct xen_string_set **result, xen_cluster cluster);


/**
 * Get the cluster_token field of the given Cluster.
 */
extern bool
xen_cluster_get_cluster_token(xen_session *session, char **result, xen_cluster cluster);


/**
 * Get the cluster_stack field of the given Cluster.
 */
extern bool
xen_cluster_get_cluster_stack(xen_session *session, char **result, xen_cluster cluster);


/**
 * Get the allowed_operations field of the given Cluster.
 */
extern bool
xen_cluster_get_allowed_operations(xen_session *session, struct xen_cluster_operation_set **result, xen_cluster cluster);


/**
 * Get the current_operations field of the given Cluster.
 */
extern bool
xen_cluster_get_current_operations(xen_session *session, xen_string_cluster_operation_map **result, xen_cluster cluster);


/**
 * Get the pool_auto_join field of the given Cluster.
 */
extern bool
xen_cluster_get_pool_auto_join(xen_session *session, bool *result, xen_cluster cluster);


/**
 * Get the token_timeout field of the given Cluster.
 */
extern bool
xen_cluster_get_token_timeout(xen_session *session, double *result, xen_cluster cluster);


/**
 * Get the token_timeout_coefficient field of the given Cluster.
 */
extern bool
xen_cluster_get_token_timeout_coefficient(xen_session *session, double *result, xen_cluster cluster);


/**
 * Get the cluster_config field of the given Cluster.
 */
extern bool
xen_cluster_get_cluster_config(xen_session *session, xen_string_string_map **result, xen_cluster cluster);


/**
 * Get the other_config field of the given Cluster.
 */
extern bool
xen_cluster_get_other_config(xen_session *session, xen_string_string_map **result, xen_cluster cluster);


/**
 * Set the other_config field of the given Cluster.
 */
extern bool
xen_cluster_set_other_config(xen_session *session, xen_cluster cluster, xen_string_string_map *other_config);


/**
 * Add the given key-value pair to the other_config field of the given
 * Cluster.
 */
extern bool
xen_cluster_add_to_other_config(xen_session *session, xen_cluster cluster, char *key, char *value);


/**
 * Remove the given key and its corresponding value from the
 * other_config field of the given Cluster.  If the key is not in that Map,
 * then do nothing.
 */
extern bool
xen_cluster_remove_from_other_config(xen_session *session, xen_cluster cluster, char *key);


/**
 * Creates a Cluster object and one Cluster_host object as its first
 * member.
 */
extern bool
xen_cluster_create(xen_session *session, xen_cluster *result, xen_pif pif, char *cluster_stack, bool pool_auto_join, double token_timeout, double token_timeout_coefficient);

/**
 * Creates a Cluster object and one Cluster_host object as its first
 * member.
 */
extern bool
xen_cluster_create_async(xen_session *session, xen_task *result, xen_pif pif, char *cluster_stack, bool pool_auto_join, double token_timeout, double token_timeout_coefficient);


/**
 * Destroys a Cluster object and the one remaining Cluster_host member.
 */
extern bool
xen_cluster_destroy(xen_session *session, xen_cluster self);

/**
 * Destroys a Cluster object and the one remaining Cluster_host member.
 */
extern bool
xen_cluster_destroy_async(xen_session *session, xen_task *result, xen_cluster self);


/**
 * Returns the network used by the cluster for inter-host
 * communication, i.e. the network shared by all cluster host PIFs.
 */
extern bool
xen_cluster_get_network(xen_session *session, xen_network *result, xen_cluster self);

/**
 * Returns the network used by the cluster for inter-host
 * communication, i.e. the network shared by all cluster host PIFs.
 */
extern bool
xen_cluster_get_network_async(xen_session *session, xen_task *result, xen_cluster self);


/**
 * Attempt to create a Cluster from the entire pool.
 */
extern bool
xen_cluster_pool_create(xen_session *session, xen_cluster *result, xen_network network, char *cluster_stack, double token_timeout, double token_timeout_coefficient);

/**
 * Attempt to create a Cluster from the entire pool.
 */
extern bool
xen_cluster_pool_create_async(xen_session *session, xen_task *result, xen_network network, char *cluster_stack, double token_timeout, double token_timeout_coefficient);


/**
 * Attempt to force destroy the Cluster_host objects, and then destroy
 * the Cluster.
 */
extern bool
xen_cluster_pool_force_destroy(xen_session *session, xen_cluster self);

/**
 * Attempt to force destroy the Cluster_host objects, and then destroy
 * the Cluster.
 */
extern bool
xen_cluster_pool_force_destroy_async(xen_session *session, xen_task *result, xen_cluster self);


/**
 * Attempt to destroy the Cluster_host objects for all hosts in the
 * pool and then destroy the Cluster.
 */
extern bool
xen_cluster_pool_destroy(xen_session *session, xen_cluster self);

/**
 * Attempt to destroy the Cluster_host objects for all hosts in the
 * pool and then destroy the Cluster.
 */
extern bool
xen_cluster_pool_destroy_async(xen_session *session, xen_task *result, xen_cluster self);


/**
 * Resynchronise the cluster_host objects across the pool. Creates them
 * where they need creating and then plugs them.
 */
extern bool
xen_cluster_pool_resync(xen_session *session, xen_cluster self);

/**
 * Resynchronise the cluster_host objects across the pool. Creates them
 * where they need creating and then plugs them.
 */
extern bool
xen_cluster_pool_resync_async(xen_session *session, xen_task *result, xen_cluster self);


/**
 * Return a list of all the Clusters known to the system.
 */
extern bool
xen_cluster_get_all(xen_session *session, struct xen_cluster_set **result);


/**
 * Return a map of Cluster references to Cluster records for all
 * Clusters known to the system.
 */
extern bool
xen_cluster_get_all_records(xen_session *session, xen_cluster_xen_cluster_record_map **result);


#endif
