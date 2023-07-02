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


#ifndef XEN_PVS_CACHE_STORAGE_H
#define XEN_PVS_CACHE_STORAGE_H

#include <xen/api/xen_common.h>
#include <xen/api/xen_host.h>
#include <xen/api/xen_host_decl.h>
#include <xen/api/xen_pvs_cache_storage_decl.h>
#include <xen/api/xen_pvs_cache_storage_xen_pvs_cache_storage_record_map.h>
#include <xen/api/xen_pvs_site.h>
#include <xen/api/xen_pvs_site_decl.h>
#include <xen/api/xen_sr.h>
#include <xen/api/xen_sr_decl.h>
#include <xen/api/xen_task_decl.h>
#include <xen/api/xen_vdi.h>
#include <xen/api/xen_vdi_decl.h>


/*
 * The PVS_cache_storage class.
 * 
 * Describes the storage that is available to a PVS site for caching purposes.
 */


/**
 * Free the given xen_pvs_cache_storage.  The given handle must have
 * been allocated by this library.
 */
extern void
xen_pvs_cache_storage_free(xen_pvs_cache_storage pvs_cache_storage);


typedef struct xen_pvs_cache_storage_set
{
    size_t size;
    xen_pvs_cache_storage *contents[];
} xen_pvs_cache_storage_set;

/**
 * Allocate a xen_pvs_cache_storage_set of the given size.
 */
extern xen_pvs_cache_storage_set *
xen_pvs_cache_storage_set_alloc(size_t size);

/**
 * Free the given xen_pvs_cache_storage_set.  The given set must have
 * been allocated by this library.
 */
extern void
xen_pvs_cache_storage_set_free(xen_pvs_cache_storage_set *set);


typedef struct xen_pvs_cache_storage_record
{
    xen_pvs_cache_storage handle;
    char *uuid;
    struct xen_host_record_opt *host;
    struct xen_sr_record_opt *sr;
    struct xen_pvs_site_record_opt *site;
    int64_t size;
    struct xen_vdi_record_opt *vdi;
} xen_pvs_cache_storage_record;

/**
 * Allocate a xen_pvs_cache_storage_record.
 */
extern xen_pvs_cache_storage_record *
xen_pvs_cache_storage_record_alloc(void);

/**
 * Free the given xen_pvs_cache_storage_record, and all referenced
 * values.  The given record must have been allocated by this library.
 */
extern void
xen_pvs_cache_storage_record_free(xen_pvs_cache_storage_record *record);


typedef struct xen_pvs_cache_storage_record_opt
{
    bool is_record;
    union
    {
        xen_pvs_cache_storage handle;
        xen_pvs_cache_storage_record *record;
    } u;
} xen_pvs_cache_storage_record_opt;

/**
 * Allocate a xen_pvs_cache_storage_record_opt.
 */
extern xen_pvs_cache_storage_record_opt *
xen_pvs_cache_storage_record_opt_alloc(void);

/**
 * Free the given xen_pvs_cache_storage_record_opt, and all referenced
 * values.  The given record_opt must have been allocated by this library.
 */
extern void
xen_pvs_cache_storage_record_opt_free(xen_pvs_cache_storage_record_opt *record_opt);


typedef struct xen_pvs_cache_storage_record_set
{
    size_t size;
    xen_pvs_cache_storage_record *contents[];
} xen_pvs_cache_storage_record_set;

/**
 * Allocate a xen_pvs_cache_storage_record_set of the given size.
 */
extern xen_pvs_cache_storage_record_set *
xen_pvs_cache_storage_record_set_alloc(size_t size);

/**
 * Free the given xen_pvs_cache_storage_record_set, and all referenced
 * values.  The given set must have been allocated by this library.
 */
extern void
xen_pvs_cache_storage_record_set_free(xen_pvs_cache_storage_record_set *set);



typedef struct xen_pvs_cache_storage_record_opt_set
{
    size_t size;
    xen_pvs_cache_storage_record_opt *contents[];
} xen_pvs_cache_storage_record_opt_set;

/**
 * Allocate a xen_pvs_cache_storage_record_opt_set of the given size.
 */
extern xen_pvs_cache_storage_record_opt_set *
xen_pvs_cache_storage_record_opt_set_alloc(size_t size);

/**
 * Free the given xen_pvs_cache_storage_record_opt_set, and all
 * referenced values.  The given set must have been allocated by this library.
 */
extern void
xen_pvs_cache_storage_record_opt_set_free(xen_pvs_cache_storage_record_opt_set *set);


/**
 * Get a record containing the current state of the given
 * PVS_cache_storage.
 */
extern bool
xen_pvs_cache_storage_get_record(xen_session *session, xen_pvs_cache_storage_record **result, xen_pvs_cache_storage pvs_cache_storage);


/**
 * Get a reference to the PVS_cache_storage instance with the specified
 * UUID.
 */
extern bool
xen_pvs_cache_storage_get_by_uuid(xen_session *session, xen_pvs_cache_storage *result, char *uuid);


/**
 * Create a new PVS_cache_storage instance, and return its handle.
 */
extern bool
xen_pvs_cache_storage_create(xen_session *session, xen_pvs_cache_storage *result, xen_pvs_cache_storage_record *record);

/**
 * Create a new PVS_cache_storage instance, and return its handle.
 */
extern bool
xen_pvs_cache_storage_create_async(xen_session *session, xen_task *result, xen_pvs_cache_storage_record *record);


/**
 * Destroy the specified PVS_cache_storage instance.
 */
extern bool
xen_pvs_cache_storage_destroy(xen_session *session, xen_pvs_cache_storage pvs_cache_storage);

/**
 * Destroy the specified PVS_cache_storage instance.
 */
extern bool
xen_pvs_cache_storage_destroy_async(xen_session *session, xen_task *result, xen_pvs_cache_storage pvs_cache_storage);


/**
 * Get the uuid field of the given PVS_cache_storage.
 */
extern bool
xen_pvs_cache_storage_get_uuid(xen_session *session, char **result, xen_pvs_cache_storage pvs_cache_storage);


/**
 * Get the host field of the given PVS_cache_storage.
 */
extern bool
xen_pvs_cache_storage_get_host(xen_session *session, xen_host *result, xen_pvs_cache_storage pvs_cache_storage);


/**
 * Get the SR field of the given PVS_cache_storage.
 */
extern bool
xen_pvs_cache_storage_get_sr(xen_session *session, xen_sr *result, xen_pvs_cache_storage pvs_cache_storage);


/**
 * Get the site field of the given PVS_cache_storage.
 */
extern bool
xen_pvs_cache_storage_get_site(xen_session *session, xen_pvs_site *result, xen_pvs_cache_storage pvs_cache_storage);


/**
 * Get the size field of the given PVS_cache_storage.
 */
extern bool
xen_pvs_cache_storage_get_size(xen_session *session, int64_t *result, xen_pvs_cache_storage pvs_cache_storage);


/**
 * Get the VDI field of the given PVS_cache_storage.
 */
extern bool
xen_pvs_cache_storage_get_vdi(xen_session *session, xen_vdi *result, xen_pvs_cache_storage pvs_cache_storage);


/**
 * Return a list of all the PVS_cache_storages known to the system.
 */
extern bool
xen_pvs_cache_storage_get_all(xen_session *session, struct xen_pvs_cache_storage_set **result);


/**
 * Return a map of PVS_cache_storage references to PVS_cache_storage
 * records for all PVS_cache_storages known to the system.
 */
extern bool
xen_pvs_cache_storage_get_all_records(xen_session *session, xen_pvs_cache_storage_xen_pvs_cache_storage_record_map **result);


#endif
