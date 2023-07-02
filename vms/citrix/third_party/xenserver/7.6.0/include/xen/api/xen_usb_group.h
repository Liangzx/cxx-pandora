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


#ifndef XEN_USB_GROUP_H
#define XEN_USB_GROUP_H

#include <xen/api/xen_common.h>
#include <xen/api/xen_pusb.h>
#include <xen/api/xen_pusb_decl.h>
#include <xen/api/xen_string_string_map.h>
#include <xen/api/xen_task_decl.h>
#include <xen/api/xen_usb_group_decl.h>
#include <xen/api/xen_usb_group_xen_usb_group_record_map.h>
#include <xen/api/xen_vusb.h>
#include <xen/api/xen_vusb_decl.h>


/*
 * The USB_group class.
 * 
 * A group of compatible USBs across the resource pool.
 */


/**
 * Free the given xen_usb_group.  The given handle must have been
 * allocated by this library.
 */
extern void
xen_usb_group_free(xen_usb_group usb_group);


typedef struct xen_usb_group_set
{
    size_t size;
    xen_usb_group *contents[];
} xen_usb_group_set;

/**
 * Allocate a xen_usb_group_set of the given size.
 */
extern xen_usb_group_set *
xen_usb_group_set_alloc(size_t size);

/**
 * Free the given xen_usb_group_set.  The given set must have been
 * allocated by this library.
 */
extern void
xen_usb_group_set_free(xen_usb_group_set *set);


typedef struct xen_usb_group_record
{
    xen_usb_group handle;
    char *uuid;
    char *name_label;
    char *name_description;
    struct xen_pusb_record_opt_set *pusbs;
    struct xen_vusb_record_opt_set *vusbs;
    xen_string_string_map *other_config;
} xen_usb_group_record;

/**
 * Allocate a xen_usb_group_record.
 */
extern xen_usb_group_record *
xen_usb_group_record_alloc(void);

/**
 * Free the given xen_usb_group_record, and all referenced values.  The
 * given record must have been allocated by this library.
 */
extern void
xen_usb_group_record_free(xen_usb_group_record *record);


typedef struct xen_usb_group_record_opt
{
    bool is_record;
    union
    {
        xen_usb_group handle;
        xen_usb_group_record *record;
    } u;
} xen_usb_group_record_opt;

/**
 * Allocate a xen_usb_group_record_opt.
 */
extern xen_usb_group_record_opt *
xen_usb_group_record_opt_alloc(void);

/**
 * Free the given xen_usb_group_record_opt, and all referenced values. 
 * The given record_opt must have been allocated by this library.
 */
extern void
xen_usb_group_record_opt_free(xen_usb_group_record_opt *record_opt);


typedef struct xen_usb_group_record_set
{
    size_t size;
    xen_usb_group_record *contents[];
} xen_usb_group_record_set;

/**
 * Allocate a xen_usb_group_record_set of the given size.
 */
extern xen_usb_group_record_set *
xen_usb_group_record_set_alloc(size_t size);

/**
 * Free the given xen_usb_group_record_set, and all referenced values. 
 * The given set must have been allocated by this library.
 */
extern void
xen_usb_group_record_set_free(xen_usb_group_record_set *set);



typedef struct xen_usb_group_record_opt_set
{
    size_t size;
    xen_usb_group_record_opt *contents[];
} xen_usb_group_record_opt_set;

/**
 * Allocate a xen_usb_group_record_opt_set of the given size.
 */
extern xen_usb_group_record_opt_set *
xen_usb_group_record_opt_set_alloc(size_t size);

/**
 * Free the given xen_usb_group_record_opt_set, and all referenced
 * values.  The given set must have been allocated by this library.
 */
extern void
xen_usb_group_record_opt_set_free(xen_usb_group_record_opt_set *set);


/**
 * Get a record containing the current state of the given USB_group.
 */
extern bool
xen_usb_group_get_record(xen_session *session, xen_usb_group_record **result, xen_usb_group usb_group);


/**
 * Get a reference to the USB_group instance with the specified UUID.
 */
extern bool
xen_usb_group_get_by_uuid(xen_session *session, xen_usb_group *result, char *uuid);


/**
 * Get all the USB_group instances with the given label.
 */
extern bool
xen_usb_group_get_by_name_label(xen_session *session, struct xen_usb_group_set **result, char *label);


/**
 * Get the uuid field of the given USB_group.
 */
extern bool
xen_usb_group_get_uuid(xen_session *session, char **result, xen_usb_group usb_group);


/**
 * Get the name/label field of the given USB_group.
 */
extern bool
xen_usb_group_get_name_label(xen_session *session, char **result, xen_usb_group usb_group);


/**
 * Get the name/description field of the given USB_group.
 */
extern bool
xen_usb_group_get_name_description(xen_session *session, char **result, xen_usb_group usb_group);


/**
 * Get the PUSBs field of the given USB_group.
 */
extern bool
xen_usb_group_get_pusbs(xen_session *session, struct xen_pusb_set **result, xen_usb_group usb_group);


/**
 * Get the VUSBs field of the given USB_group.
 */
extern bool
xen_usb_group_get_vusbs(xen_session *session, struct xen_vusb_set **result, xen_usb_group usb_group);


/**
 * Get the other_config field of the given USB_group.
 */
extern bool
xen_usb_group_get_other_config(xen_session *session, xen_string_string_map **result, xen_usb_group usb_group);


/**
 * Set the name/label field of the given USB_group.
 */
extern bool
xen_usb_group_set_name_label(xen_session *session, xen_usb_group usb_group, char *label);


/**
 * Set the name/description field of the given USB_group.
 */
extern bool
xen_usb_group_set_name_description(xen_session *session, xen_usb_group usb_group, char *description);


/**
 * Set the other_config field of the given USB_group.
 */
extern bool
xen_usb_group_set_other_config(xen_session *session, xen_usb_group usb_group, xen_string_string_map *other_config);


/**
 * Add the given key-value pair to the other_config field of the given
 * USB_group.
 */
extern bool
xen_usb_group_add_to_other_config(xen_session *session, xen_usb_group usb_group, char *key, char *value);


/**
 * Remove the given key and its corresponding value from the
 * other_config field of the given USB_group.  If the key is not in that Map,
 * then do nothing.
 */
extern bool
xen_usb_group_remove_from_other_config(xen_session *session, xen_usb_group usb_group, char *key);


/**
 * .
 */
extern bool
xen_usb_group_create(xen_session *session, xen_usb_group *result, char *name_label, char *name_description, xen_string_string_map *other_config);

/**
 * .
 */
extern bool
xen_usb_group_create_async(xen_session *session, xen_task *result, char *name_label, char *name_description, xen_string_string_map *other_config);


/**
 * .
 */
extern bool
xen_usb_group_destroy(xen_session *session, xen_usb_group self);

/**
 * .
 */
extern bool
xen_usb_group_destroy_async(xen_session *session, xen_task *result, xen_usb_group self);


/**
 * Return a list of all the USB_groups known to the system.
 */
extern bool
xen_usb_group_get_all(xen_session *session, struct xen_usb_group_set **result);


/**
 * Return a map of USB_group references to USB_group records for all
 * USB_groups known to the system.
 */
extern bool
xen_usb_group_get_all_records(xen_session *session, xen_usb_group_xen_usb_group_record_map **result);


#endif
