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


#ifndef XEN_VDI_NBD_SERVER_INFO_H
#define XEN_VDI_NBD_SERVER_INFO_H

#include <xen/api/xen_common.h>
#include <xen/api/xen_vdi_nbd_server_info_decl.h>


/*
 * The vdi_nbd_server_info class.
 * 
 * Details for connecting to a VDI using the Network Block Device protocol.
 */


/**
 * Free the given xen_vdi_nbd_server_info.  The given handle must have
 * been allocated by this library.
 */
extern void
xen_vdi_nbd_server_info_free(xen_vdi_nbd_server_info vdi_nbd_server_info);


typedef struct xen_vdi_nbd_server_info_set
{
    size_t size;
    xen_vdi_nbd_server_info *contents[];
} xen_vdi_nbd_server_info_set;

/**
 * Allocate a xen_vdi_nbd_server_info_set of the given size.
 */
extern xen_vdi_nbd_server_info_set *
xen_vdi_nbd_server_info_set_alloc(size_t size);

/**
 * Free the given xen_vdi_nbd_server_info_set.  The given set must have
 * been allocated by this library.
 */
extern void
xen_vdi_nbd_server_info_set_free(xen_vdi_nbd_server_info_set *set);


typedef struct xen_vdi_nbd_server_info_record
{
    xen_vdi_nbd_server_info handle;
    char *exportname;
    char *address;
    int64_t port;
    char *cert;
    char *subject;
} xen_vdi_nbd_server_info_record;

/**
 * Allocate a xen_vdi_nbd_server_info_record.
 */
extern xen_vdi_nbd_server_info_record *
xen_vdi_nbd_server_info_record_alloc(void);

/**
 * Free the given xen_vdi_nbd_server_info_record, and all referenced
 * values.  The given record must have been allocated by this library.
 */
extern void
xen_vdi_nbd_server_info_record_free(xen_vdi_nbd_server_info_record *record);


typedef struct xen_vdi_nbd_server_info_record_opt
{
    bool is_record;
    union
    {
        xen_vdi_nbd_server_info handle;
        xen_vdi_nbd_server_info_record *record;
    } u;
} xen_vdi_nbd_server_info_record_opt;

/**
 * Allocate a xen_vdi_nbd_server_info_record_opt.
 */
extern xen_vdi_nbd_server_info_record_opt *
xen_vdi_nbd_server_info_record_opt_alloc(void);

/**
 * Free the given xen_vdi_nbd_server_info_record_opt, and all
 * referenced values.  The given record_opt must have been allocated by this
 * library.
 */
extern void
xen_vdi_nbd_server_info_record_opt_free(xen_vdi_nbd_server_info_record_opt *record_opt);


typedef struct xen_vdi_nbd_server_info_record_set
{
    size_t size;
    xen_vdi_nbd_server_info_record *contents[];
} xen_vdi_nbd_server_info_record_set;

/**
 * Allocate a xen_vdi_nbd_server_info_record_set of the given size.
 */
extern xen_vdi_nbd_server_info_record_set *
xen_vdi_nbd_server_info_record_set_alloc(size_t size);

/**
 * Free the given xen_vdi_nbd_server_info_record_set, and all
 * referenced values.  The given set must have been allocated by this library.
 */
extern void
xen_vdi_nbd_server_info_record_set_free(xen_vdi_nbd_server_info_record_set *set);



typedef struct xen_vdi_nbd_server_info_record_opt_set
{
    size_t size;
    xen_vdi_nbd_server_info_record_opt *contents[];
} xen_vdi_nbd_server_info_record_opt_set;

/**
 * Allocate a xen_vdi_nbd_server_info_record_opt_set of the given size.
 */
extern xen_vdi_nbd_server_info_record_opt_set *
xen_vdi_nbd_server_info_record_opt_set_alloc(size_t size);

/**
 * Free the given xen_vdi_nbd_server_info_record_opt_set, and all
 * referenced values.  The given set must have been allocated by this library.
 */
extern void
xen_vdi_nbd_server_info_record_opt_set_free(xen_vdi_nbd_server_info_record_opt_set *set);




#endif
