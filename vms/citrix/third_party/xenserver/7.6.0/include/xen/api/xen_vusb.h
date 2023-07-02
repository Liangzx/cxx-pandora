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


#ifndef XEN_VUSB_H
#define XEN_VUSB_H

#include "xen_string_vusb_operations_map_internal.h"
#include "xen_vusb_operations_internal.h"
#include <xen/api/xen_common.h>
#include <xen/api/xen_string_string_map.h>
#include <xen/api/xen_string_vusb_operations_map.h>
#include <xen/api/xen_task_decl.h>
#include <xen/api/xen_usb_group.h>
#include <xen/api/xen_usb_group_decl.h>
#include <xen/api/xen_vm.h>
#include <xen/api/xen_vm_decl.h>
#include <xen/api/xen_vusb_decl.h>
#include <xen/api/xen_vusb_operations.h>
#include <xen/api/xen_vusb_xen_vusb_record_map.h>


/*
 * The VUSB class.
 * 
 * Describes the vusb device.
 */


/**
 * Free the given xen_vusb.  The given handle must have been allocated
 * by this library.
 */
extern void
xen_vusb_free(xen_vusb vusb);


typedef struct xen_vusb_set
{
    size_t size;
    xen_vusb *contents[];
} xen_vusb_set;

/**
 * Allocate a xen_vusb_set of the given size.
 */
extern xen_vusb_set *
xen_vusb_set_alloc(size_t size);

/**
 * Free the given xen_vusb_set.  The given set must have been allocated
 * by this library.
 */
extern void
xen_vusb_set_free(xen_vusb_set *set);


typedef struct xen_vusb_record
{
    xen_vusb handle;
    char *uuid;
    struct xen_vusb_operations_set *allowed_operations;
    xen_string_vusb_operations_map *current_operations;
    struct xen_vm_record_opt *vm;
    struct xen_usb_group_record_opt *usb_group;
    xen_string_string_map *other_config;
    bool currently_attached;
} xen_vusb_record;

/**
 * Allocate a xen_vusb_record.
 */
extern xen_vusb_record *
xen_vusb_record_alloc(void);

/**
 * Free the given xen_vusb_record, and all referenced values.  The
 * given record must have been allocated by this library.
 */
extern void
xen_vusb_record_free(xen_vusb_record *record);


typedef struct xen_vusb_record_opt
{
    bool is_record;
    union
    {
        xen_vusb handle;
        xen_vusb_record *record;
    } u;
} xen_vusb_record_opt;

/**
 * Allocate a xen_vusb_record_opt.
 */
extern xen_vusb_record_opt *
xen_vusb_record_opt_alloc(void);

/**
 * Free the given xen_vusb_record_opt, and all referenced values.  The
 * given record_opt must have been allocated by this library.
 */
extern void
xen_vusb_record_opt_free(xen_vusb_record_opt *record_opt);


typedef struct xen_vusb_record_set
{
    size_t size;
    xen_vusb_record *contents[];
} xen_vusb_record_set;

/**
 * Allocate a xen_vusb_record_set of the given size.
 */
extern xen_vusb_record_set *
xen_vusb_record_set_alloc(size_t size);

/**
 * Free the given xen_vusb_record_set, and all referenced values.  The
 * given set must have been allocated by this library.
 */
extern void
xen_vusb_record_set_free(xen_vusb_record_set *set);



typedef struct xen_vusb_record_opt_set
{
    size_t size;
    xen_vusb_record_opt *contents[];
} xen_vusb_record_opt_set;

/**
 * Allocate a xen_vusb_record_opt_set of the given size.
 */
extern xen_vusb_record_opt_set *
xen_vusb_record_opt_set_alloc(size_t size);

/**
 * Free the given xen_vusb_record_opt_set, and all referenced values. 
 * The given set must have been allocated by this library.
 */
extern void
xen_vusb_record_opt_set_free(xen_vusb_record_opt_set *set);


/**
 * Get a record containing the current state of the given VUSB.
 */
extern bool
xen_vusb_get_record(xen_session *session, xen_vusb_record **result, xen_vusb vusb);


/**
 * Get a reference to the VUSB instance with the specified UUID.
 */
extern bool
xen_vusb_get_by_uuid(xen_session *session, xen_vusb *result, char *uuid);


/**
 * Get the uuid field of the given VUSB.
 */
extern bool
xen_vusb_get_uuid(xen_session *session, char **result, xen_vusb vusb);


/**
 * Get the allowed_operations field of the given VUSB.
 */
extern bool
xen_vusb_get_allowed_operations(xen_session *session, struct xen_vusb_operations_set **result, xen_vusb vusb);


/**
 * Get the current_operations field of the given VUSB.
 */
extern bool
xen_vusb_get_current_operations(xen_session *session, xen_string_vusb_operations_map **result, xen_vusb vusb);


/**
 * Get the VM field of the given VUSB.
 */
extern bool
xen_vusb_get_vm(xen_session *session, xen_vm *result, xen_vusb vusb);


/**
 * Get the USB_group field of the given VUSB.
 */
extern bool
xen_vusb_get_usb_group(xen_session *session, xen_usb_group *result, xen_vusb vusb);


/**
 * Get the other_config field of the given VUSB.
 */
extern bool
xen_vusb_get_other_config(xen_session *session, xen_string_string_map **result, xen_vusb vusb);


/**
 * Get the currently_attached field of the given VUSB.
 */
extern bool
xen_vusb_get_currently_attached(xen_session *session, bool *result, xen_vusb vusb);


/**
 * Set the other_config field of the given VUSB.
 */
extern bool
xen_vusb_set_other_config(xen_session *session, xen_vusb vusb, xen_string_string_map *other_config);


/**
 * Add the given key-value pair to the other_config field of the given
 * VUSB.
 */
extern bool
xen_vusb_add_to_other_config(xen_session *session, xen_vusb vusb, char *key, char *value);


/**
 * Remove the given key and its corresponding value from the
 * other_config field of the given VUSB.  If the key is not in that Map, then
 * do nothing.
 */
extern bool
xen_vusb_remove_from_other_config(xen_session *session, xen_vusb vusb, char *key);


/**
 * Create a new VUSB record in the database only.
 */
extern bool
xen_vusb_create(xen_session *session, xen_vusb *result, xen_vm vm, xen_usb_group usb_group, xen_string_string_map *other_config);

/**
 * Create a new VUSB record in the database only.
 */
extern bool
xen_vusb_create_async(xen_session *session, xen_task *result, xen_vm vm, xen_usb_group usb_group, xen_string_string_map *other_config);


/**
 * Unplug the vusb device from the vm.
 */
extern bool
xen_vusb_unplug(xen_session *session, xen_vusb self);

/**
 * Unplug the vusb device from the vm.
 */
extern bool
xen_vusb_unplug_async(xen_session *session, xen_task *result, xen_vusb self);


/**
 * Removes a VUSB record from the database.
 */
extern bool
xen_vusb_destroy(xen_session *session, xen_vusb self);

/**
 * Removes a VUSB record from the database.
 */
extern bool
xen_vusb_destroy_async(xen_session *session, xen_task *result, xen_vusb self);


/**
 * Return a list of all the VUSBs known to the system.
 */
extern bool
xen_vusb_get_all(xen_session *session, struct xen_vusb_set **result);


/**
 * Return a map of VUSB references to VUSB records for all VUSBs known
 * to the system.
 */
extern bool
xen_vusb_get_all_records(xen_session *session, xen_vusb_xen_vusb_record_map **result);


#endif
