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


#ifndef XEN_POOL_UPDATE_H
#define XEN_POOL_UPDATE_H

#include "xen_update_after_apply_guidance_internal.h"
#include <xen/api/xen_common.h>
#include <xen/api/xen_host.h>
#include <xen/api/xen_host_decl.h>
#include <xen/api/xen_livepatch_status.h>
#include <xen/api/xen_pool_update_decl.h>
#include <xen/api/xen_pool_update_xen_pool_update_record_map.h>
#include <xen/api/xen_string_string_map.h>
#include <xen/api/xen_task_decl.h>
#include <xen/api/xen_update_after_apply_guidance.h>
#include <xen/api/xen_vdi.h>
#include <xen/api/xen_vdi_decl.h>


/*
 * The pool_update class.
 * 
 * Pool-wide updates to the host software.
 */


/**
 * Free the given xen_pool_update.  The given handle must have been
 * allocated by this library.
 */
extern void
xen_pool_update_free(xen_pool_update pool_update);


typedef struct xen_pool_update_set
{
    size_t size;
    xen_pool_update *contents[];
} xen_pool_update_set;

/**
 * Allocate a xen_pool_update_set of the given size.
 */
extern xen_pool_update_set *
xen_pool_update_set_alloc(size_t size);

/**
 * Free the given xen_pool_update_set.  The given set must have been
 * allocated by this library.
 */
extern void
xen_pool_update_set_free(xen_pool_update_set *set);


typedef struct xen_pool_update_record
{
    xen_pool_update handle;
    char *uuid;
    char *name_label;
    char *name_description;
    char *version;
    int64_t installation_size;
    char *key;
    struct xen_update_after_apply_guidance_set *after_apply_guidance;
    struct xen_vdi_record_opt *vdi;
    struct xen_host_record_opt_set *hosts;
    xen_string_string_map *other_config;
    bool enforce_homogeneity;
} xen_pool_update_record;

/**
 * Allocate a xen_pool_update_record.
 */
extern xen_pool_update_record *
xen_pool_update_record_alloc(void);

/**
 * Free the given xen_pool_update_record, and all referenced values. 
 * The given record must have been allocated by this library.
 */
extern void
xen_pool_update_record_free(xen_pool_update_record *record);


typedef struct xen_pool_update_record_opt
{
    bool is_record;
    union
    {
        xen_pool_update handle;
        xen_pool_update_record *record;
    } u;
} xen_pool_update_record_opt;

/**
 * Allocate a xen_pool_update_record_opt.
 */
extern xen_pool_update_record_opt *
xen_pool_update_record_opt_alloc(void);

/**
 * Free the given xen_pool_update_record_opt, and all referenced
 * values.  The given record_opt must have been allocated by this library.
 */
extern void
xen_pool_update_record_opt_free(xen_pool_update_record_opt *record_opt);


typedef struct xen_pool_update_record_set
{
    size_t size;
    xen_pool_update_record *contents[];
} xen_pool_update_record_set;

/**
 * Allocate a xen_pool_update_record_set of the given size.
 */
extern xen_pool_update_record_set *
xen_pool_update_record_set_alloc(size_t size);

/**
 * Free the given xen_pool_update_record_set, and all referenced
 * values.  The given set must have been allocated by this library.
 */
extern void
xen_pool_update_record_set_free(xen_pool_update_record_set *set);



typedef struct xen_pool_update_record_opt_set
{
    size_t size;
    xen_pool_update_record_opt *contents[];
} xen_pool_update_record_opt_set;

/**
 * Allocate a xen_pool_update_record_opt_set of the given size.
 */
extern xen_pool_update_record_opt_set *
xen_pool_update_record_opt_set_alloc(size_t size);

/**
 * Free the given xen_pool_update_record_opt_set, and all referenced
 * values.  The given set must have been allocated by this library.
 */
extern void
xen_pool_update_record_opt_set_free(xen_pool_update_record_opt_set *set);


/**
 * Get a record containing the current state of the given pool_update.
 */
extern bool
xen_pool_update_get_record(xen_session *session, xen_pool_update_record **result, xen_pool_update pool_update);


/**
 * Get a reference to the pool_update instance with the specified UUID.
 */
extern bool
xen_pool_update_get_by_uuid(xen_session *session, xen_pool_update *result, char *uuid);


/**
 * Get all the pool_update instances with the given label.
 */
extern bool
xen_pool_update_get_by_name_label(xen_session *session, struct xen_pool_update_set **result, char *label);


/**
 * Get the uuid field of the given pool_update.
 */
extern bool
xen_pool_update_get_uuid(xen_session *session, char **result, xen_pool_update pool_update);


/**
 * Get the name/label field of the given pool_update.
 */
extern bool
xen_pool_update_get_name_label(xen_session *session, char **result, xen_pool_update pool_update);


/**
 * Get the name/description field of the given pool_update.
 */
extern bool
xen_pool_update_get_name_description(xen_session *session, char **result, xen_pool_update pool_update);


/**
 * Get the version field of the given pool_update.
 */
extern bool
xen_pool_update_get_version(xen_session *session, char **result, xen_pool_update pool_update);


/**
 * Get the installation_size field of the given pool_update.
 */
extern bool
xen_pool_update_get_installation_size(xen_session *session, int64_t *result, xen_pool_update pool_update);


/**
 * Get the key field of the given pool_update.
 */
extern bool
xen_pool_update_get_key(xen_session *session, char **result, xen_pool_update pool_update);


/**
 * Get the after_apply_guidance field of the given pool_update.
 */
extern bool
xen_pool_update_get_after_apply_guidance(xen_session *session, struct xen_update_after_apply_guidance_set **result, xen_pool_update pool_update);


/**
 * Get the vdi field of the given pool_update.
 */
extern bool
xen_pool_update_get_vdi(xen_session *session, xen_vdi *result, xen_pool_update pool_update);


/**
 * Get the hosts field of the given pool_update.
 */
extern bool
xen_pool_update_get_hosts(xen_session *session, struct xen_host_set **result, xen_pool_update pool_update);


/**
 * Get the other_config field of the given pool_update.
 */
extern bool
xen_pool_update_get_other_config(xen_session *session, xen_string_string_map **result, xen_pool_update pool_update);


/**
 * Get the enforce_homogeneity field of the given pool_update.
 */
extern bool
xen_pool_update_get_enforce_homogeneity(xen_session *session, bool *result, xen_pool_update pool_update);


/**
 * Set the other_config field of the given pool_update.
 */
extern bool
xen_pool_update_set_other_config(xen_session *session, xen_pool_update pool_update, xen_string_string_map *other_config);


/**
 * Add the given key-value pair to the other_config field of the given
 * pool_update.
 */
extern bool
xen_pool_update_add_to_other_config(xen_session *session, xen_pool_update pool_update, char *key, char *value);


/**
 * Remove the given key and its corresponding value from the
 * other_config field of the given pool_update.  If the key is not in that
 * Map, then do nothing.
 */
extern bool
xen_pool_update_remove_from_other_config(xen_session *session, xen_pool_update pool_update, char *key);


/**
 * Introduce update VDI.
 */
extern bool
xen_pool_update_introduce(xen_session *session, xen_pool_update *result, xen_vdi vdi);

/**
 * Introduce update VDI.
 */
extern bool
xen_pool_update_introduce_async(xen_session *session, xen_task *result, xen_vdi vdi);


/**
 * Execute the precheck stage of the selected update on a host.
 */
extern bool
xen_pool_update_precheck(xen_session *session, enum xen_livepatch_status *result, xen_pool_update self, xen_host host);

/**
 * Execute the precheck stage of the selected update on a host.
 */
extern bool
xen_pool_update_precheck_async(xen_session *session, xen_task *result, xen_pool_update self, xen_host host);


/**
 * Apply the selected update to a host.
 */
extern bool
xen_pool_update_apply(xen_session *session, xen_pool_update self, xen_host host);

/**
 * Apply the selected update to a host.
 */
extern bool
xen_pool_update_apply_async(xen_session *session, xen_task *result, xen_pool_update self, xen_host host);


/**
 * Apply the selected update to all hosts in the pool.
 */
extern bool
xen_pool_update_pool_apply(xen_session *session, xen_pool_update self);

/**
 * Apply the selected update to all hosts in the pool.
 */
extern bool
xen_pool_update_pool_apply_async(xen_session *session, xen_task *result, xen_pool_update self);


/**
 * Removes the update's files from all hosts in the pool, but does not
 * revert the update.
 */
extern bool
xen_pool_update_pool_clean(xen_session *session, xen_pool_update self);

/**
 * Removes the update's files from all hosts in the pool, but does not
 * revert the update.
 */
extern bool
xen_pool_update_pool_clean_async(xen_session *session, xen_task *result, xen_pool_update self);


/**
 * Removes the database entry. Only works on unapplied update.
 */
extern bool
xen_pool_update_destroy(xen_session *session, xen_pool_update self);

/**
 * Removes the database entry. Only works on unapplied update.
 */
extern bool
xen_pool_update_destroy_async(xen_session *session, xen_task *result, xen_pool_update self);


/**
 * Return a list of all the pool_updates known to the system.
 */
extern bool
xen_pool_update_get_all(xen_session *session, struct xen_pool_update_set **result);


/**
 * Return a map of pool_update references to pool_update records for
 * all pool_updates known to the system.
 */
extern bool
xen_pool_update_get_all_records(xen_session *session, xen_pool_update_xen_pool_update_record_map **result);


#endif
