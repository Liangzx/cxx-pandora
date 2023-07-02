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


#ifndef XEN_PVS_PROXY_H
#define XEN_PVS_PROXY_H

#include "xen_pvs_proxy_status_internal.h"
#include <xen/api/xen_common.h>
#include <xen/api/xen_pvs_proxy_decl.h>
#include <xen/api/xen_pvs_proxy_status.h>
#include <xen/api/xen_pvs_proxy_xen_pvs_proxy_record_map.h>
#include <xen/api/xen_pvs_site.h>
#include <xen/api/xen_pvs_site_decl.h>
#include <xen/api/xen_task_decl.h>
#include <xen/api/xen_vif.h>
#include <xen/api/xen_vif_decl.h>


/*
 * The PVS_proxy class.
 * 
 * a proxy connects a VM/VIF with a PVS site.
 */


/**
 * Free the given xen_pvs_proxy.  The given handle must have been
 * allocated by this library.
 */
extern void
xen_pvs_proxy_free(xen_pvs_proxy pvs_proxy);


typedef struct xen_pvs_proxy_set
{
    size_t size;
    xen_pvs_proxy *contents[];
} xen_pvs_proxy_set;

/**
 * Allocate a xen_pvs_proxy_set of the given size.
 */
extern xen_pvs_proxy_set *
xen_pvs_proxy_set_alloc(size_t size);

/**
 * Free the given xen_pvs_proxy_set.  The given set must have been
 * allocated by this library.
 */
extern void
xen_pvs_proxy_set_free(xen_pvs_proxy_set *set);


typedef struct xen_pvs_proxy_record
{
    xen_pvs_proxy handle;
    char *uuid;
    struct xen_pvs_site_record_opt *site;
    struct xen_vif_record_opt *vif;
    bool currently_attached;
    enum xen_pvs_proxy_status status;
} xen_pvs_proxy_record;

/**
 * Allocate a xen_pvs_proxy_record.
 */
extern xen_pvs_proxy_record *
xen_pvs_proxy_record_alloc(void);

/**
 * Free the given xen_pvs_proxy_record, and all referenced values.  The
 * given record must have been allocated by this library.
 */
extern void
xen_pvs_proxy_record_free(xen_pvs_proxy_record *record);


typedef struct xen_pvs_proxy_record_opt
{
    bool is_record;
    union
    {
        xen_pvs_proxy handle;
        xen_pvs_proxy_record *record;
    } u;
} xen_pvs_proxy_record_opt;

/**
 * Allocate a xen_pvs_proxy_record_opt.
 */
extern xen_pvs_proxy_record_opt *
xen_pvs_proxy_record_opt_alloc(void);

/**
 * Free the given xen_pvs_proxy_record_opt, and all referenced values. 
 * The given record_opt must have been allocated by this library.
 */
extern void
xen_pvs_proxy_record_opt_free(xen_pvs_proxy_record_opt *record_opt);


typedef struct xen_pvs_proxy_record_set
{
    size_t size;
    xen_pvs_proxy_record *contents[];
} xen_pvs_proxy_record_set;

/**
 * Allocate a xen_pvs_proxy_record_set of the given size.
 */
extern xen_pvs_proxy_record_set *
xen_pvs_proxy_record_set_alloc(size_t size);

/**
 * Free the given xen_pvs_proxy_record_set, and all referenced values. 
 * The given set must have been allocated by this library.
 */
extern void
xen_pvs_proxy_record_set_free(xen_pvs_proxy_record_set *set);



typedef struct xen_pvs_proxy_record_opt_set
{
    size_t size;
    xen_pvs_proxy_record_opt *contents[];
} xen_pvs_proxy_record_opt_set;

/**
 * Allocate a xen_pvs_proxy_record_opt_set of the given size.
 */
extern xen_pvs_proxy_record_opt_set *
xen_pvs_proxy_record_opt_set_alloc(size_t size);

/**
 * Free the given xen_pvs_proxy_record_opt_set, and all referenced
 * values.  The given set must have been allocated by this library.
 */
extern void
xen_pvs_proxy_record_opt_set_free(xen_pvs_proxy_record_opt_set *set);


/**
 * Get a record containing the current state of the given PVS_proxy.
 */
extern bool
xen_pvs_proxy_get_record(xen_session *session, xen_pvs_proxy_record **result, xen_pvs_proxy pvs_proxy);


/**
 * Get a reference to the PVS_proxy instance with the specified UUID.
 */
extern bool
xen_pvs_proxy_get_by_uuid(xen_session *session, xen_pvs_proxy *result, char *uuid);


/**
 * Get the uuid field of the given PVS_proxy.
 */
extern bool
xen_pvs_proxy_get_uuid(xen_session *session, char **result, xen_pvs_proxy pvs_proxy);


/**
 * Get the site field of the given PVS_proxy.
 */
extern bool
xen_pvs_proxy_get_site(xen_session *session, xen_pvs_site *result, xen_pvs_proxy pvs_proxy);


/**
 * Get the VIF field of the given PVS_proxy.
 */
extern bool
xen_pvs_proxy_get_vif(xen_session *session, xen_vif *result, xen_pvs_proxy pvs_proxy);


/**
 * Get the currently_attached field of the given PVS_proxy.
 */
extern bool
xen_pvs_proxy_get_currently_attached(xen_session *session, bool *result, xen_pvs_proxy pvs_proxy);


/**
 * Get the status field of the given PVS_proxy.
 */
extern bool
xen_pvs_proxy_get_status(xen_session *session, enum xen_pvs_proxy_status *result, xen_pvs_proxy pvs_proxy);


/**
 * Configure a VM/VIF to use a PVS proxy.
 */
extern bool
xen_pvs_proxy_create(xen_session *session, xen_pvs_proxy *result, xen_pvs_site site, xen_vif vif);

/**
 * Configure a VM/VIF to use a PVS proxy.
 */
extern bool
xen_pvs_proxy_create_async(xen_session *session, xen_task *result, xen_pvs_site site, xen_vif vif);


/**
 * remove (or switch off) a PVS proxy for this VM.
 */
extern bool
xen_pvs_proxy_destroy(xen_session *session, xen_pvs_proxy self);

/**
 * remove (or switch off) a PVS proxy for this VM.
 */
extern bool
xen_pvs_proxy_destroy_async(xen_session *session, xen_task *result, xen_pvs_proxy self);


/**
 * Return a list of all the PVS_proxys known to the system.
 */
extern bool
xen_pvs_proxy_get_all(xen_session *session, struct xen_pvs_proxy_set **result);


/**
 * Return a map of PVS_proxy references to PVS_proxy records for all
 * PVS_proxys known to the system.
 */
extern bool
xen_pvs_proxy_get_all_records(xen_session *session, xen_pvs_proxy_xen_pvs_proxy_record_map **result);


#endif
