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

#ifndef XEN_API_VERSION_H
#define XEN_API_VERSION_H

#include <stdint.h>

typedef enum xen_api_version
{
    xen_api_version_1_1 = 1,
    xen_api_version_1_2 = 2,
    xen_api_version_1_3 = 3,
    xen_api_version_1_4 = 4,
    xen_api_version_1_5 = 5,
    xen_api_version_1_6 = 6,
    xen_api_version_1_7 = 7,
    xen_api_version_1_8 = 8,
    xen_api_version_1_9 = 9,
    xen_api_version_1_10 = 10,
    xen_api_version_2_0 = 11,
    xen_api_version_2_1 = 12,
    xen_api_version_2_2 = 13,
    xen_api_version_2_3 = 14,
    xen_api_version_2_4 = 15,
    xen_api_version_2_5 = 16,
    xen_api_version_2_6 = 17,
    xen_api_version_2_7 = 18,
    xen_api_version_2_8 = 19,
    xen_api_version_2_9 = 20,
    xen_api_version_2_10 = 21,
    xen_api_version_2_11 = 22,
    xen_api_latest_version = 22,
    xen_api_unknown_version = 99,
} xen_api_version;

extern const char *
xen_api_version_to_string(xen_api_version version);

extern xen_api_version
xen_api_version_from_int(int64_t major_version, int64_t minor_version);

#endif
