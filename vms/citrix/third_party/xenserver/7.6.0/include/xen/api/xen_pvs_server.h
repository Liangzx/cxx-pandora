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


#ifndef XEN_PVS_SERVER_H
#define XEN_PVS_SERVER_H

#include <xen/api/xen_common.h>
#include <xen/api/xen_pvs_server_decl.h>
#include <xen/api/xen_pvs_server_xen_pvs_server_record_map.h>
#include <xen/api/xen_pvs_site.h>
#include <xen/api/xen_pvs_site_decl.h>
#include <xen/api/xen_string_set.h>
#include <xen/api/xen_task_decl.h>


/*
 * The PVS_server class.
 * 
 * individual machine serving provisioning (block) data.
 */


/**
 * Free the given xen_pvs_server.  The given handle must have been
 * allocated by this library.
 */
extern void
xen_pvs_server_free(xen_pvs_server pvs_server);


typedef struct xen_pvs_server_set
{
    size_t size;
    xen_pvs_server *contents[];
} xen_pvs_server_set;

/**
 * Allocate a xen_pvs_server_set of the given size.
 */
extern xen_pvs_server_set *
xen_pvs_server_set_alloc(size_t size);

/**
 * Free the given xen_pvs_server_set.  The given set must have been
 * allocated by this library.
 */
extern void
xen_pvs_server_set_free(xen_pvs_server_set *set);


typedef struct xen_pvs_server_record
{
    xen_pvs_server handle;
    char *uuid;
    struct xen_string_set *addresses;
    int64_t first_port;
    int64_t last_port;
    struct xen_pvs_site_record_opt *site;
} xen_pvs_server_record;

/**
 * Allocate a xen_pvs_server_record.
 */
extern xen_pvs_server_record *
xen_pvs_server_record_alloc(void);

/**
 * Free the given xen_pvs_server_record, and all referenced values. 
 * The given record must have been allocated by this library.
 */
extern void
xen_pvs_server_record_free(xen_pvs_server_record *record);


typedef struct xen_pvs_server_record_opt
{
    bool is_record;
    union
    {
        xen_pvs_server handle;
        xen_pvs_server_record *record;
    } u;
} xen_pvs_server_record_opt;

/**
 * Allocate a xen_pvs_server_record_opt.
 */
extern xen_pvs_server_record_opt *
xen_pvs_server_record_opt_alloc(void);

/**
 * Free the given xen_pvs_server_record_opt, and all referenced values.
 *  The given record_opt must have been allocated by this library.
 */
extern void
xen_pvs_server_record_opt_free(xen_pvs_server_record_opt *record_opt);


typedef struct xen_pvs_server_record_set
{
    size_t size;
    xen_pvs_server_record *contents[];
} xen_pvs_server_record_set;

/**
 * Allocate a xen_pvs_server_record_set of the given size.
 */
extern xen_pvs_server_record_set *
xen_pvs_server_record_set_alloc(size_t size);

/**
 * Free the given xen_pvs_server_record_set, and all referenced values.
 *  The given set must have been allocated by this library.
 */
extern void
xen_pvs_server_record_set_free(xen_pvs_server_record_set *set);



typedef struct xen_pvs_server_record_opt_set
{
    size_t size;
    xen_pvs_server_record_opt *contents[];
} xen_pvs_server_record_opt_set;

/**
 * Allocate a xen_pvs_server_record_opt_set of the given size.
 */
extern xen_pvs_server_record_opt_set *
xen_pvs_server_record_opt_set_alloc(size_t size);

/**
 * Free the given xen_pvs_server_record_opt_set, and all referenced
 * values.  The given set must have been allocated by this library.
 */
extern void
xen_pvs_server_record_opt_set_free(xen_pvs_server_record_opt_set *set);


/**
 * Get a record containing the current state of the given PVS_server.
 */
extern bool
xen_pvs_server_get_record(xen_session *session, xen_pvs_server_record **result, xen_pvs_server pvs_server);


/**
 * Get a reference to the PVS_server instance with the specified UUID.
 */
extern bool
xen_pvs_server_get_by_uuid(xen_session *session, xen_pvs_server *result, char *uuid);


/**
 * Get the uuid field of the given PVS_server.
 */
extern bool
xen_pvs_server_get_uuid(xen_session *session, char **result, xen_pvs_server pvs_server);


/**
 * Get the addresses field of the given PVS_server.
 */
extern bool
xen_pvs_server_get_addresses(xen_session *session, struct xen_string_set **result, xen_pvs_server pvs_server);


/**
 * Get the first_port field of the given PVS_server.
 */
extern bool
xen_pvs_server_get_first_port(xen_session *session, int64_t *result, xen_pvs_server pvs_server);


/**
 * Get the last_port field of the given PVS_server.
 */
extern bool
xen_pvs_server_get_last_port(xen_session *session, int64_t *result, xen_pvs_server pvs_server);


/**
 * Get the site field of the given PVS_server.
 */
extern bool
xen_pvs_server_get_site(xen_session *session, xen_pvs_site *result, xen_pvs_server pvs_server);


/**
 * introduce new PVS server.
 */
extern bool
xen_pvs_server_introduce(xen_session *session, xen_pvs_server *result, struct xen_string_set *addresses, int64_t first_port, int64_t last_port, xen_pvs_site site);

/**
 * introduce new PVS server.
 */
extern bool
xen_pvs_server_introduce_async(xen_session *session, xen_task *result, struct xen_string_set *addresses, int64_t first_port, int64_t last_port, xen_pvs_site site);


/**
 * forget a PVS server.
 */
extern bool
xen_pvs_server_forget(xen_session *session, xen_pvs_server self);

/**
 * forget a PVS server.
 */
extern bool
xen_pvs_server_forget_async(xen_session *session, xen_task *result, xen_pvs_server self);


/**
 * Return a list of all the PVS_servers known to the system.
 */
extern bool
xen_pvs_server_get_all(xen_session *session, struct xen_pvs_server_set **result);


/**
 * Return a map of PVS_server references to PVS_server records for all
 * PVS_servers known to the system.
 */
extern bool
xen_pvs_server_get_all_records(xen_session *session, xen_pvs_server_xen_pvs_server_record_map **result);


#endif
