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


#ifndef XEN_SDN_CONTROLLER_H
#define XEN_SDN_CONTROLLER_H

#include "xen_sdn_controller_protocol_internal.h"
#include <xen/api/xen_common.h>
#include <xen/api/xen_sdn_controller_decl.h>
#include <xen/api/xen_sdn_controller_protocol.h>
#include <xen/api/xen_sdn_controller_xen_sdn_controller_record_map.h>
#include <xen/api/xen_task_decl.h>


/*
 * The SDN_controller class.
 * 
 * Describes the SDN controller that is to connect with the pool.
 */


/**
 * Free the given xen_sdn_controller.  The given handle must have been
 * allocated by this library.
 */
extern void
xen_sdn_controller_free(xen_sdn_controller sdn_controller);


typedef struct xen_sdn_controller_set
{
    size_t size;
    xen_sdn_controller *contents[];
} xen_sdn_controller_set;

/**
 * Allocate a xen_sdn_controller_set of the given size.
 */
extern xen_sdn_controller_set *
xen_sdn_controller_set_alloc(size_t size);

/**
 * Free the given xen_sdn_controller_set.  The given set must have been
 * allocated by this library.
 */
extern void
xen_sdn_controller_set_free(xen_sdn_controller_set *set);


typedef struct xen_sdn_controller_record
{
    xen_sdn_controller handle;
    char *uuid;
    enum xen_sdn_controller_protocol protocol;
    char *address;
    int64_t port;
} xen_sdn_controller_record;

/**
 * Allocate a xen_sdn_controller_record.
 */
extern xen_sdn_controller_record *
xen_sdn_controller_record_alloc(void);

/**
 * Free the given xen_sdn_controller_record, and all referenced values.
 *  The given record must have been allocated by this library.
 */
extern void
xen_sdn_controller_record_free(xen_sdn_controller_record *record);


typedef struct xen_sdn_controller_record_opt
{
    bool is_record;
    union
    {
        xen_sdn_controller handle;
        xen_sdn_controller_record *record;
    } u;
} xen_sdn_controller_record_opt;

/**
 * Allocate a xen_sdn_controller_record_opt.
 */
extern xen_sdn_controller_record_opt *
xen_sdn_controller_record_opt_alloc(void);

/**
 * Free the given xen_sdn_controller_record_opt, and all referenced
 * values.  The given record_opt must have been allocated by this library.
 */
extern void
xen_sdn_controller_record_opt_free(xen_sdn_controller_record_opt *record_opt);


typedef struct xen_sdn_controller_record_set
{
    size_t size;
    xen_sdn_controller_record *contents[];
} xen_sdn_controller_record_set;

/**
 * Allocate a xen_sdn_controller_record_set of the given size.
 */
extern xen_sdn_controller_record_set *
xen_sdn_controller_record_set_alloc(size_t size);

/**
 * Free the given xen_sdn_controller_record_set, and all referenced
 * values.  The given set must have been allocated by this library.
 */
extern void
xen_sdn_controller_record_set_free(xen_sdn_controller_record_set *set);



typedef struct xen_sdn_controller_record_opt_set
{
    size_t size;
    xen_sdn_controller_record_opt *contents[];
} xen_sdn_controller_record_opt_set;

/**
 * Allocate a xen_sdn_controller_record_opt_set of the given size.
 */
extern xen_sdn_controller_record_opt_set *
xen_sdn_controller_record_opt_set_alloc(size_t size);

/**
 * Free the given xen_sdn_controller_record_opt_set, and all referenced
 * values.  The given set must have been allocated by this library.
 */
extern void
xen_sdn_controller_record_opt_set_free(xen_sdn_controller_record_opt_set *set);


/**
 * Get a record containing the current state of the given
 * SDN_controller.
 */
extern bool
xen_sdn_controller_get_record(xen_session *session, xen_sdn_controller_record **result, xen_sdn_controller sdn_controller);


/**
 * Get a reference to the SDN_controller instance with the specified
 * UUID.
 */
extern bool
xen_sdn_controller_get_by_uuid(xen_session *session, xen_sdn_controller *result, char *uuid);


/**
 * Get the uuid field of the given SDN_controller.
 */
extern bool
xen_sdn_controller_get_uuid(xen_session *session, char **result, xen_sdn_controller sdn_controller);


/**
 * Get the protocol field of the given SDN_controller.
 */
extern bool
xen_sdn_controller_get_protocol(xen_session *session, enum xen_sdn_controller_protocol *result, xen_sdn_controller sdn_controller);


/**
 * Get the address field of the given SDN_controller.
 */
extern bool
xen_sdn_controller_get_address(xen_session *session, char **result, xen_sdn_controller sdn_controller);


/**
 * Get the port field of the given SDN_controller.
 */
extern bool
xen_sdn_controller_get_port(xen_session *session, int64_t *result, xen_sdn_controller sdn_controller);


/**
 * Introduce an SDN controller to the pool.
 */
extern bool
xen_sdn_controller_introduce(xen_session *session, xen_sdn_controller *result, enum xen_sdn_controller_protocol protocol, char *address, int64_t port);

/**
 * Introduce an SDN controller to the pool.
 */
extern bool
xen_sdn_controller_introduce_async(xen_session *session, xen_task *result, enum xen_sdn_controller_protocol protocol, char *address, int64_t port);


/**
 * Remove the OVS manager of the pool and destroy the db record.
 */
extern bool
xen_sdn_controller_forget(xen_session *session, xen_sdn_controller self);

/**
 * Remove the OVS manager of the pool and destroy the db record.
 */
extern bool
xen_sdn_controller_forget_async(xen_session *session, xen_task *result, xen_sdn_controller self);


/**
 * Return a list of all the SDN_controllers known to the system.
 */
extern bool
xen_sdn_controller_get_all(xen_session *session, struct xen_sdn_controller_set **result);


/**
 * Return a map of SDN_controller references to SDN_controller records
 * for all SDN_controllers known to the system.
 */
extern bool
xen_sdn_controller_get_all_records(xen_session *session, xen_sdn_controller_xen_sdn_controller_record_map **result);


#endif
