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


#ifndef XEN_PUSB_H
#define XEN_PUSB_H

#include <xen/api/xen_common.h>
#include <xen/api/xen_host.h>
#include <xen/api/xen_host_decl.h>
#include <xen/api/xen_pusb_decl.h>
#include <xen/api/xen_pusb_xen_pusb_record_map.h>
#include <xen/api/xen_string_string_map.h>
#include <xen/api/xen_task_decl.h>
#include <xen/api/xen_usb_group.h>
#include <xen/api/xen_usb_group_decl.h>


/*
 * The PUSB class.
 * 
 * A physical USB device.
 */


/**
 * Free the given xen_pusb.  The given handle must have been allocated
 * by this library.
 */
extern void
xen_pusb_free(xen_pusb pusb);


typedef struct xen_pusb_set
{
    size_t size;
    xen_pusb *contents[];
} xen_pusb_set;

/**
 * Allocate a xen_pusb_set of the given size.
 */
extern xen_pusb_set *
xen_pusb_set_alloc(size_t size);

/**
 * Free the given xen_pusb_set.  The given set must have been allocated
 * by this library.
 */
extern void
xen_pusb_set_free(xen_pusb_set *set);


typedef struct xen_pusb_record
{
    xen_pusb handle;
    char *uuid;
    struct xen_usb_group_record_opt *usb_group;
    struct xen_host_record_opt *host;
    char *path;
    char *vendor_id;
    char *vendor_desc;
    char *product_id;
    char *product_desc;
    char *serial;
    char *version;
    char *description;
    bool passthrough_enabled;
    xen_string_string_map *other_config;
} xen_pusb_record;

/**
 * Allocate a xen_pusb_record.
 */
extern xen_pusb_record *
xen_pusb_record_alloc(void);

/**
 * Free the given xen_pusb_record, and all referenced values.  The
 * given record must have been allocated by this library.
 */
extern void
xen_pusb_record_free(xen_pusb_record *record);


typedef struct xen_pusb_record_opt
{
    bool is_record;
    union
    {
        xen_pusb handle;
        xen_pusb_record *record;
    } u;
} xen_pusb_record_opt;

/**
 * Allocate a xen_pusb_record_opt.
 */
extern xen_pusb_record_opt *
xen_pusb_record_opt_alloc(void);

/**
 * Free the given xen_pusb_record_opt, and all referenced values.  The
 * given record_opt must have been allocated by this library.
 */
extern void
xen_pusb_record_opt_free(xen_pusb_record_opt *record_opt);


typedef struct xen_pusb_record_set
{
    size_t size;
    xen_pusb_record *contents[];
} xen_pusb_record_set;

/**
 * Allocate a xen_pusb_record_set of the given size.
 */
extern xen_pusb_record_set *
xen_pusb_record_set_alloc(size_t size);

/**
 * Free the given xen_pusb_record_set, and all referenced values.  The
 * given set must have been allocated by this library.
 */
extern void
xen_pusb_record_set_free(xen_pusb_record_set *set);



typedef struct xen_pusb_record_opt_set
{
    size_t size;
    xen_pusb_record_opt *contents[];
} xen_pusb_record_opt_set;

/**
 * Allocate a xen_pusb_record_opt_set of the given size.
 */
extern xen_pusb_record_opt_set *
xen_pusb_record_opt_set_alloc(size_t size);

/**
 * Free the given xen_pusb_record_opt_set, and all referenced values. 
 * The given set must have been allocated by this library.
 */
extern void
xen_pusb_record_opt_set_free(xen_pusb_record_opt_set *set);


/**
 * Get a record containing the current state of the given PUSB.
 */
extern bool
xen_pusb_get_record(xen_session *session, xen_pusb_record **result, xen_pusb pusb);


/**
 * Get a reference to the PUSB instance with the specified UUID.
 */
extern bool
xen_pusb_get_by_uuid(xen_session *session, xen_pusb *result, char *uuid);


/**
 * Get the uuid field of the given PUSB.
 */
extern bool
xen_pusb_get_uuid(xen_session *session, char **result, xen_pusb pusb);


/**
 * Get the USB_group field of the given PUSB.
 */
extern bool
xen_pusb_get_usb_group(xen_session *session, xen_usb_group *result, xen_pusb pusb);


/**
 * Get the host field of the given PUSB.
 */
extern bool
xen_pusb_get_host(xen_session *session, xen_host *result, xen_pusb pusb);


/**
 * Get the path field of the given PUSB.
 */
extern bool
xen_pusb_get_path(xen_session *session, char **result, xen_pusb pusb);


/**
 * Get the vendor_id field of the given PUSB.
 */
extern bool
xen_pusb_get_vendor_id(xen_session *session, char **result, xen_pusb pusb);


/**
 * Get the vendor_desc field of the given PUSB.
 */
extern bool
xen_pusb_get_vendor_desc(xen_session *session, char **result, xen_pusb pusb);


/**
 * Get the product_id field of the given PUSB.
 */
extern bool
xen_pusb_get_product_id(xen_session *session, char **result, xen_pusb pusb);


/**
 * Get the product_desc field of the given PUSB.
 */
extern bool
xen_pusb_get_product_desc(xen_session *session, char **result, xen_pusb pusb);


/**
 * Get the serial field of the given PUSB.
 */
extern bool
xen_pusb_get_serial(xen_session *session, char **result, xen_pusb pusb);


/**
 * Get the version field of the given PUSB.
 */
extern bool
xen_pusb_get_version(xen_session *session, char **result, xen_pusb pusb);


/**
 * Get the description field of the given PUSB.
 */
extern bool
xen_pusb_get_description(xen_session *session, char **result, xen_pusb pusb);


/**
 * Get the passthrough_enabled field of the given PUSB.
 */
extern bool
xen_pusb_get_passthrough_enabled(xen_session *session, bool *result, xen_pusb pusb);


/**
 * Get the other_config field of the given PUSB.
 */
extern bool
xen_pusb_get_other_config(xen_session *session, xen_string_string_map **result, xen_pusb pusb);


/**
 * Set the other_config field of the given PUSB.
 */
extern bool
xen_pusb_set_other_config(xen_session *session, xen_pusb pusb, xen_string_string_map *other_config);


/**
 * Add the given key-value pair to the other_config field of the given
 * PUSB.
 */
extern bool
xen_pusb_add_to_other_config(xen_session *session, xen_pusb pusb, char *key, char *value);


/**
 * Remove the given key and its corresponding value from the
 * other_config field of the given PUSB.  If the key is not in that Map, then
 * do nothing.
 */
extern bool
xen_pusb_remove_from_other_config(xen_session *session, xen_pusb pusb, char *key);


/**
 * .
 */
extern bool
xen_pusb_scan(xen_session *session, xen_host host);

/**
 * .
 */
extern bool
xen_pusb_scan_async(xen_session *session, xen_task *result, xen_host host);


/**
 * .
 */
extern bool
xen_pusb_set_passthrough_enabled(xen_session *session, xen_pusb self, bool value);

/**
 * .
 */
extern bool
xen_pusb_set_passthrough_enabled_async(xen_session *session, xen_task *result, xen_pusb self, bool value);


/**
 * Return a list of all the PUSBs known to the system.
 */
extern bool
xen_pusb_get_all(xen_session *session, struct xen_pusb_set **result);


/**
 * Return a map of PUSB references to PUSB records for all PUSBs known
 * to the system.
 */
extern bool
xen_pusb_get_all_records(xen_session *session, xen_pusb_xen_pusb_record_map **result);


#endif
