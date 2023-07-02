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


#ifndef XEN_FEATURE_H
#define XEN_FEATURE_H

#include <xen/api/xen_common.h>
#include <xen/api/xen_feature_decl.h>
#include <xen/api/xen_feature_xen_feature_record_map.h>
#include <xen/api/xen_host.h>
#include <xen/api/xen_host_decl.h>


/*
 * The Feature class.
 * 
 * A new piece of functionality.
 */


/**
 * Free the given xen_feature.  The given handle must have been
 * allocated by this library.
 */
extern void
xen_feature_free(xen_feature feature);


typedef struct xen_feature_set
{
    size_t size;
    xen_feature *contents[];
} xen_feature_set;

/**
 * Allocate a xen_feature_set of the given size.
 */
extern xen_feature_set *
xen_feature_set_alloc(size_t size);

/**
 * Free the given xen_feature_set.  The given set must have been
 * allocated by this library.
 */
extern void
xen_feature_set_free(xen_feature_set *set);


typedef struct xen_feature_record
{
    xen_feature handle;
    char *uuid;
    char *name_label;
    char *name_description;
    bool enabled;
    bool experimental;
    char *version;
    struct xen_host_record_opt *host;
} xen_feature_record;

/**
 * Allocate a xen_feature_record.
 */
extern xen_feature_record *
xen_feature_record_alloc(void);

/**
 * Free the given xen_feature_record, and all referenced values.  The
 * given record must have been allocated by this library.
 */
extern void
xen_feature_record_free(xen_feature_record *record);


typedef struct xen_feature_record_opt
{
    bool is_record;
    union
    {
        xen_feature handle;
        xen_feature_record *record;
    } u;
} xen_feature_record_opt;

/**
 * Allocate a xen_feature_record_opt.
 */
extern xen_feature_record_opt *
xen_feature_record_opt_alloc(void);

/**
 * Free the given xen_feature_record_opt, and all referenced values. 
 * The given record_opt must have been allocated by this library.
 */
extern void
xen_feature_record_opt_free(xen_feature_record_opt *record_opt);


typedef struct xen_feature_record_set
{
    size_t size;
    xen_feature_record *contents[];
} xen_feature_record_set;

/**
 * Allocate a xen_feature_record_set of the given size.
 */
extern xen_feature_record_set *
xen_feature_record_set_alloc(size_t size);

/**
 * Free the given xen_feature_record_set, and all referenced values. 
 * The given set must have been allocated by this library.
 */
extern void
xen_feature_record_set_free(xen_feature_record_set *set);



typedef struct xen_feature_record_opt_set
{
    size_t size;
    xen_feature_record_opt *contents[];
} xen_feature_record_opt_set;

/**
 * Allocate a xen_feature_record_opt_set of the given size.
 */
extern xen_feature_record_opt_set *
xen_feature_record_opt_set_alloc(size_t size);

/**
 * Free the given xen_feature_record_opt_set, and all referenced
 * values.  The given set must have been allocated by this library.
 */
extern void
xen_feature_record_opt_set_free(xen_feature_record_opt_set *set);


/**
 * Get a record containing the current state of the given Feature.
 */
extern bool
xen_feature_get_record(xen_session *session, xen_feature_record **result, xen_feature feature);


/**
 * Get a reference to the Feature instance with the specified UUID.
 */
extern bool
xen_feature_get_by_uuid(xen_session *session, xen_feature *result, char *uuid);


/**
 * Get all the Feature instances with the given label.
 */
extern bool
xen_feature_get_by_name_label(xen_session *session, struct xen_feature_set **result, char *label);


/**
 * Get the uuid field of the given Feature.
 */
extern bool
xen_feature_get_uuid(xen_session *session, char **result, xen_feature feature);


/**
 * Get the name/label field of the given Feature.
 */
extern bool
xen_feature_get_name_label(xen_session *session, char **result, xen_feature feature);


/**
 * Get the name/description field of the given Feature.
 */
extern bool
xen_feature_get_name_description(xen_session *session, char **result, xen_feature feature);


/**
 * Get the enabled field of the given Feature.
 */
extern bool
xen_feature_get_enabled(xen_session *session, bool *result, xen_feature feature);


/**
 * Get the experimental field of the given Feature.
 */
extern bool
xen_feature_get_experimental(xen_session *session, bool *result, xen_feature feature);


/**
 * Get the version field of the given Feature.
 */
extern bool
xen_feature_get_version(xen_session *session, char **result, xen_feature feature);


/**
 * Get the host field of the given Feature.
 */
extern bool
xen_feature_get_host(xen_session *session, xen_host *result, xen_feature feature);


/**
 * Return a list of all the Features known to the system.
 */
extern bool
xen_feature_get_all(xen_session *session, struct xen_feature_set **result);


/**
 * Return a map of Feature references to Feature records for all
 * Features known to the system.
 */
extern bool
xen_feature_get_all_records(xen_session *session, xen_feature_xen_feature_record_map **result);


#endif
