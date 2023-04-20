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


#ifndef XEN_NETWORK_SRIOV_H
#define XEN_NETWORK_SRIOV_H

#include "xen_sriov_configuration_mode_internal.h"
#include <xen/api/xen_common.h>
#include <xen/api/xen_network_decl.h>
#include <xen/api/xen_network_sriov_decl.h>
#include <xen/api/xen_network_sriov_xen_network_sriov_record_map.h>
#include <xen/api/xen_pif.h>
#include <xen/api/xen_pif_decl.h>
#include <xen/api/xen_sriov_configuration_mode.h>
#include <xen/api/xen_task_decl.h>


/*
 * The network_sriov class.
 * 
 * network-sriov which connects logical pif and physical pif.
 */


/**
 * Free the given xen_network_sriov.  The given handle must have been
 * allocated by this library.
 */
extern void
xen_network_sriov_free(xen_network_sriov network_sriov);


typedef struct xen_network_sriov_set
{
    size_t size;
    xen_network_sriov *contents[];
} xen_network_sriov_set;

/**
 * Allocate a xen_network_sriov_set of the given size.
 */
extern xen_network_sriov_set *
xen_network_sriov_set_alloc(size_t size);

/**
 * Free the given xen_network_sriov_set.  The given set must have been
 * allocated by this library.
 */
extern void
xen_network_sriov_set_free(xen_network_sriov_set *set);


typedef struct xen_network_sriov_record
{
    xen_network_sriov handle;
    char *uuid;
    struct xen_pif_record_opt *physical_pif;
    struct xen_pif_record_opt *logical_pif;
    bool requires_reboot;
    enum xen_sriov_configuration_mode configuration_mode;
} xen_network_sriov_record;

/**
 * Allocate a xen_network_sriov_record.
 */
extern xen_network_sriov_record *
xen_network_sriov_record_alloc(void);

/**
 * Free the given xen_network_sriov_record, and all referenced values. 
 * The given record must have been allocated by this library.
 */
extern void
xen_network_sriov_record_free(xen_network_sriov_record *record);


typedef struct xen_network_sriov_record_opt
{
    bool is_record;
    union
    {
        xen_network_sriov handle;
        xen_network_sriov_record *record;
    } u;
} xen_network_sriov_record_opt;

/**
 * Allocate a xen_network_sriov_record_opt.
 */
extern xen_network_sriov_record_opt *
xen_network_sriov_record_opt_alloc(void);

/**
 * Free the given xen_network_sriov_record_opt, and all referenced
 * values.  The given record_opt must have been allocated by this library.
 */
extern void
xen_network_sriov_record_opt_free(xen_network_sriov_record_opt *record_opt);


typedef struct xen_network_sriov_record_set
{
    size_t size;
    xen_network_sriov_record *contents[];
} xen_network_sriov_record_set;

/**
 * Allocate a xen_network_sriov_record_set of the given size.
 */
extern xen_network_sriov_record_set *
xen_network_sriov_record_set_alloc(size_t size);

/**
 * Free the given xen_network_sriov_record_set, and all referenced
 * values.  The given set must have been allocated by this library.
 */
extern void
xen_network_sriov_record_set_free(xen_network_sriov_record_set *set);



typedef struct xen_network_sriov_record_opt_set
{
    size_t size;
    xen_network_sriov_record_opt *contents[];
} xen_network_sriov_record_opt_set;

/**
 * Allocate a xen_network_sriov_record_opt_set of the given size.
 */
extern xen_network_sriov_record_opt_set *
xen_network_sriov_record_opt_set_alloc(size_t size);

/**
 * Free the given xen_network_sriov_record_opt_set, and all referenced
 * values.  The given set must have been allocated by this library.
 */
extern void
xen_network_sriov_record_opt_set_free(xen_network_sriov_record_opt_set *set);


/**
 * Get a record containing the current state of the given
 * network_sriov.
 */
extern bool
xen_network_sriov_get_record(xen_session *session, xen_network_sriov_record **result, xen_network_sriov network_sriov);


/**
 * Get a reference to the network_sriov instance with the specified
 * UUID.
 */
extern bool
xen_network_sriov_get_by_uuid(xen_session *session, xen_network_sriov *result, char *uuid);


/**
 * Get the uuid field of the given network_sriov.
 */
extern bool
xen_network_sriov_get_uuid(xen_session *session, char **result, xen_network_sriov network_sriov);


/**
 * Get the physical_PIF field of the given network_sriov.
 */
extern bool
xen_network_sriov_get_physical_pif(xen_session *session, xen_pif *result, xen_network_sriov network_sriov);


/**
 * Get the logical_PIF field of the given network_sriov.
 */
extern bool
xen_network_sriov_get_logical_pif(xen_session *session, xen_pif *result, xen_network_sriov network_sriov);


/**
 * Get the requires_reboot field of the given network_sriov.
 */
extern bool
xen_network_sriov_get_requires_reboot(xen_session *session, bool *result, xen_network_sriov network_sriov);


/**
 * Get the configuration_mode field of the given network_sriov.
 */
extern bool
xen_network_sriov_get_configuration_mode(xen_session *session, enum xen_sriov_configuration_mode *result, xen_network_sriov network_sriov);


/**
 * Enable SR-IOV on the specific PIF. It will create a network-sriov
 * based on the specific PIF and automatically create a logical PIF to connect
 * the specific network.
 */
extern bool
xen_network_sriov_create(xen_session *session, xen_network_sriov *result, xen_pif pif, xen_network network);

/**
 * Enable SR-IOV on the specific PIF. It will create a network-sriov
 * based on the specific PIF and automatically create a logical PIF to connect
 * the specific network.
 */
extern bool
xen_network_sriov_create_async(xen_session *session, xen_task *result, xen_pif pif, xen_network network);


/**
 * Disable SR-IOV on the specific PIF. It will destroy the
 * network-sriov and the logical PIF accordingly.
 */
extern bool
xen_network_sriov_destroy(xen_session *session, xen_network_sriov self);

/**
 * Disable SR-IOV on the specific PIF. It will destroy the
 * network-sriov and the logical PIF accordingly.
 */
extern bool
xen_network_sriov_destroy_async(xen_session *session, xen_task *result, xen_network_sriov self);


/**
 * Get the number of free SR-IOV VFs on the associated PIF.
 */
extern bool
xen_network_sriov_get_remaining_capacity(xen_session *session, int64_t *result, xen_network_sriov self);

/**
 * Get the number of free SR-IOV VFs on the associated PIF.
 */
extern bool
xen_network_sriov_get_remaining_capacity_async(xen_session *session, xen_task *result, xen_network_sriov self);


/**
 * Return a list of all the network_sriovs known to the system.
 */
extern bool
xen_network_sriov_get_all(xen_session *session, struct xen_network_sriov_set **result);


/**
 * Return a map of network_sriov references to network_sriov records
 * for all network_sriovs known to the system.
 */
extern bool
xen_network_sriov_get_all_records(xen_session *session, xen_network_sriov_xen_network_sriov_record_map **result);


#endif
