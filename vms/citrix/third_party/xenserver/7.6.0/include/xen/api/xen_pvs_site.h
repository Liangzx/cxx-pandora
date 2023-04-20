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


#ifndef XEN_PVS_SITE_H
#define XEN_PVS_SITE_H

#include <xen/api/xen_common.h>
#include <xen/api/xen_pvs_cache_storage.h>
#include <xen/api/xen_pvs_cache_storage_decl.h>
#include <xen/api/xen_pvs_proxy.h>
#include <xen/api/xen_pvs_proxy_decl.h>
#include <xen/api/xen_pvs_server.h>
#include <xen/api/xen_pvs_server_decl.h>
#include <xen/api/xen_pvs_site_decl.h>
#include <xen/api/xen_pvs_site_xen_pvs_site_record_map.h>
#include <xen/api/xen_task_decl.h>


/*
 * The PVS_site class.
 * 
 * machines serving blocks of data for provisioning VMs.
 */


/**
 * Free the given xen_pvs_site.  The given handle must have been
 * allocated by this library.
 */
extern void
xen_pvs_site_free(xen_pvs_site pvs_site);


typedef struct xen_pvs_site_set
{
    size_t size;
    xen_pvs_site *contents[];
} xen_pvs_site_set;

/**
 * Allocate a xen_pvs_site_set of the given size.
 */
extern xen_pvs_site_set *
xen_pvs_site_set_alloc(size_t size);

/**
 * Free the given xen_pvs_site_set.  The given set must have been
 * allocated by this library.
 */
extern void
xen_pvs_site_set_free(xen_pvs_site_set *set);


typedef struct xen_pvs_site_record
{
    xen_pvs_site handle;
    char *uuid;
    char *name_label;
    char *name_description;
    char *pvs_uuid;
    struct xen_pvs_cache_storage_record_opt_set *cache_storage;
    struct xen_pvs_server_record_opt_set *servers;
    struct xen_pvs_proxy_record_opt_set *proxies;
} xen_pvs_site_record;

/**
 * Allocate a xen_pvs_site_record.
 */
extern xen_pvs_site_record *
xen_pvs_site_record_alloc(void);

/**
 * Free the given xen_pvs_site_record, and all referenced values.  The
 * given record must have been allocated by this library.
 */
extern void
xen_pvs_site_record_free(xen_pvs_site_record *record);


typedef struct xen_pvs_site_record_opt
{
    bool is_record;
    union
    {
        xen_pvs_site handle;
        xen_pvs_site_record *record;
    } u;
} xen_pvs_site_record_opt;

/**
 * Allocate a xen_pvs_site_record_opt.
 */
extern xen_pvs_site_record_opt *
xen_pvs_site_record_opt_alloc(void);

/**
 * Free the given xen_pvs_site_record_opt, and all referenced values. 
 * The given record_opt must have been allocated by this library.
 */
extern void
xen_pvs_site_record_opt_free(xen_pvs_site_record_opt *record_opt);


typedef struct xen_pvs_site_record_set
{
    size_t size;
    xen_pvs_site_record *contents[];
} xen_pvs_site_record_set;

/**
 * Allocate a xen_pvs_site_record_set of the given size.
 */
extern xen_pvs_site_record_set *
xen_pvs_site_record_set_alloc(size_t size);

/**
 * Free the given xen_pvs_site_record_set, and all referenced values. 
 * The given set must have been allocated by this library.
 */
extern void
xen_pvs_site_record_set_free(xen_pvs_site_record_set *set);



typedef struct xen_pvs_site_record_opt_set
{
    size_t size;
    xen_pvs_site_record_opt *contents[];
} xen_pvs_site_record_opt_set;

/**
 * Allocate a xen_pvs_site_record_opt_set of the given size.
 */
extern xen_pvs_site_record_opt_set *
xen_pvs_site_record_opt_set_alloc(size_t size);

/**
 * Free the given xen_pvs_site_record_opt_set, and all referenced
 * values.  The given set must have been allocated by this library.
 */
extern void
xen_pvs_site_record_opt_set_free(xen_pvs_site_record_opt_set *set);


/**
 * Get a record containing the current state of the given PVS_site.
 */
extern bool
xen_pvs_site_get_record(xen_session *session, xen_pvs_site_record **result, xen_pvs_site pvs_site);


/**
 * Get a reference to the PVS_site instance with the specified UUID.
 */
extern bool
xen_pvs_site_get_by_uuid(xen_session *session, xen_pvs_site *result, char *uuid);


/**
 * Get all the PVS_site instances with the given label.
 */
extern bool
xen_pvs_site_get_by_name_label(xen_session *session, struct xen_pvs_site_set **result, char *label);


/**
 * Get the uuid field of the given PVS_site.
 */
extern bool
xen_pvs_site_get_uuid(xen_session *session, char **result, xen_pvs_site pvs_site);


/**
 * Get the name/label field of the given PVS_site.
 */
extern bool
xen_pvs_site_get_name_label(xen_session *session, char **result, xen_pvs_site pvs_site);


/**
 * Get the name/description field of the given PVS_site.
 */
extern bool
xen_pvs_site_get_name_description(xen_session *session, char **result, xen_pvs_site pvs_site);


/**
 * Get the PVS_uuid field of the given PVS_site.
 */
extern bool
xen_pvs_site_get_pvs_uuid(xen_session *session, char **result, xen_pvs_site pvs_site);


/**
 * Get the cache_storage field of the given PVS_site.
 */
extern bool
xen_pvs_site_get_cache_storage(xen_session *session, struct xen_pvs_cache_storage_set **result, xen_pvs_site pvs_site);


/**
 * Get the servers field of the given PVS_site.
 */
extern bool
xen_pvs_site_get_servers(xen_session *session, struct xen_pvs_server_set **result, xen_pvs_site pvs_site);


/**
 * Get the proxies field of the given PVS_site.
 */
extern bool
xen_pvs_site_get_proxies(xen_session *session, struct xen_pvs_proxy_set **result, xen_pvs_site pvs_site);


/**
 * Set the name/label field of the given PVS_site.
 */
extern bool
xen_pvs_site_set_name_label(xen_session *session, xen_pvs_site pvs_site, char *label);


/**
 * Set the name/description field of the given PVS_site.
 */
extern bool
xen_pvs_site_set_name_description(xen_session *session, xen_pvs_site pvs_site, char *description);


/**
 * Introduce new PVS site.
 */
extern bool
xen_pvs_site_introduce(xen_session *session, xen_pvs_site *result, char *name_label, char *name_description, char *pvs_uuid);

/**
 * Introduce new PVS site.
 */
extern bool
xen_pvs_site_introduce_async(xen_session *session, xen_task *result, char *name_label, char *name_description, char *pvs_uuid);


/**
 * Remove a site's meta data.
 */
extern bool
xen_pvs_site_forget(xen_session *session, xen_pvs_site self);

/**
 * Remove a site's meta data.
 */
extern bool
xen_pvs_site_forget_async(xen_session *session, xen_task *result, xen_pvs_site self);


/**
 * Update the PVS UUID of the PVS site.
 */
extern bool
xen_pvs_site_set_pvs_uuid(xen_session *session, xen_pvs_site self, char *value);

/**
 * Update the PVS UUID of the PVS site.
 */
extern bool
xen_pvs_site_set_pvs_uuid_async(xen_session *session, xen_task *result, xen_pvs_site self, char *value);


/**
 * Return a list of all the PVS_sites known to the system.
 */
extern bool
xen_pvs_site_get_all(xen_session *session, struct xen_pvs_site_set **result);


/**
 * Return a map of PVS_site references to PVS_site records for all
 * PVS_sites known to the system.
 */
extern bool
xen_pvs_site_get_all_records(xen_session *session, xen_pvs_site_xen_pvs_site_record_map **result);


#endif
