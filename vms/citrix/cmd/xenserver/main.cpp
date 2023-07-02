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

/* PURPOSE:
 * ========
 *
 * This sample program demonstrates how to create a VM, create a blank
 * disk image in a named Storage Repository, and attach the disk to the
 * VM.
 *
 * Before creating the VM, the host's capabilities are queried and
 * displayed.
 *
 * After creating the VM, some powercycle operations are demonstrated:
 * starting the VM, suspending it, resuming it and shutting it down.
 * After each powercycle operation the server is queried and the powerstate
 * displayed.
 *
 */
#include <cinttypes>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <iostream>

#include <curl/curl.h>     // curl
#include <libxml/parser.h> // xml
// xen server api
#ifdef __cplusplus
extern "C" {
#endif
#include <xen/api/xen_all.h>
#ifdef __cplusplus
}
#endif
typedef struct {
  xen_result_func func;
  void *handle;
} xen_comms;

const char *url = "172.20.60.131";

static void print_error(xen_session *session) {
  fprintf(stderr, "Error: %d\n", session->error_description_count);
  for (int i = 0; i < session->error_description_count; i++) {
    fprintf(stderr, "%s ", session->error_description[i]);
  }
  fprintf(stderr, "\n");
}

static size_t write_func(void *ptr, size_t size, size_t nmemb,
                         xen_comms *comms) {
  size_t n = size * nmemb;
  //    printf("\n\n---Result from server -----------------------\n");
  //    printf("%s\n", ((char *) ptr));
  //    fflush(stdout);

  return comms->func(ptr, n, comms->handle) ? n : 0;
}

static int call_func(const void *data, size_t len, void *user_handle,
                     void *result_handle, xen_result_func result_func) {

  (void)user_handle;

  CURL *curl = curl_easy_init();
  if (!curl) {
    return -1;
  }

  xen_comms comms{};
  comms.func = result_func;
  comms.handle = result_handle;

  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &write_func);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &comms);
  curl_easy_setopt(curl, CURLOPT_POST, 1);
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
  curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, len);
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);

  CURLcode result = curl_easy_perform(curl);

  curl_easy_cleanup(curl);

  return result;
}

std::string find_host_by_vm_uuid(xen_session *session, char *uuid) {
  // get vm by vm uuid
  xen_vm vm{};
  xen_vm_record *vm_record = nullptr;
  //    std::shared_ptr<xen_vm_record> shr_vm_record{vm_record, [vm_record] {
  //        if (vm_record != nullptr) {
  //            xen_vm_record_free(vm_record);
  //        }
  //    }};
  xen_vm_get_by_uuid(session, &vm, uuid);
  // get record of the vm
  if (!xen_vm_get_record(session, &vm_record, vm)) {
    std::cout << "xen_vm_get_record" << std::endl;
    return std::string{};
  };
  auto handle = vm_record->resident_on->u.handle;
  char *host_uuid = nullptr;
  // get host uuid of the vm
  if (!xen_host_get_uuid(session, &host_uuid, handle)) {
    std::cout << "xen_host_get_uuid" << std::endl;
    return std::string{};
  };

  std::string strHost{};
  if (host_uuid != nullptr) {
    strHost = host_uuid;
    free(host_uuid);
  }
  //
  return strHost;
}

int main(int argc, char **argv) {

#define CLEANUP                                                                \
  do {                                                                         \
    xen_session_logout(session);                                               \
    curl_global_cleanup();                                                     \
    xen_fini();                                                                \
    xmlCleanupParser();                                                        \
  } while (0);

  xmlInitParser();
  xmlKeepBlanksDefault(0);
  xen_init();
  curl_global_init(CURL_GLOBAL_ALL);
  //
  const char *username = "root";
  const char *password = "Dingjia123";

  xen_session *session = xen_session_login_with_password(
      call_func, nullptr, username, password, xen_api_latest_version);
  //
  if (session == nullptr) {
    std::cout << "session == nullptr" << std::endl;
    CLEANUP
    return -1;
  }

  printf("\n\nQuerying host...\n");

  xen_host host;
  if (!xen_session_get_this_host(session, &host, session)) {
    print_error(session);
    CLEANUP;
    return 1;
  }

  xen_string_string_map *versions;
  if (!xen_host_get_software_version(session, &versions, host)) {
    print_error(session);
    xen_host_free(host);
    CLEANUP;
    return 1;
  }

  xen_string_set *supported_bootloaders;
  if (!xen_host_get_supported_bootloaders(session, &supported_bootloaders,
                                          host)) {
    print_error(session);
    xen_string_string_map_free(versions);
    xen_host_free(host);
    CLEANUP;
    return 1;
  }

  xen_string_set *capabilities;
  if (!xen_host_get_capabilities(session, &capabilities, host)) {
    print_error(session);
    xen_string_set_free(supported_bootloaders);
    xen_string_string_map_free(versions);
    xen_host_free(host);
    CLEANUP;
    return 1;
  }

  for (size_t i = 0; i < versions->size; i++) {
    printf("%s -> %s.\n", versions->contents[i].key, versions->contents[i].val);
  }

  printf("Host supports the following bootloaders:");
  for (size_t i = 0; i < supported_bootloaders->size; i++) {
    printf(" %s", supported_bootloaders->contents[i]);
  }
  printf("\n");

  printf("Host has the following capabilities:");
  for (size_t i = 0; i < capabilities->size; i++) {
    printf(" %s", capabilities->contents[i]);
  }
  printf("\n");
  //
  std::cout << "--------------" << std::endl;
  //    char * vmUUID = "18383923-d7f9-7a31-8ce1-5ae4daafc362";
  char *vmUUID = "7a7ea270-5f70-5036-1cd3-bd905d30fd47";
  auto strHost = find_host_by_vm_uuid(session, vmUUID);
  std::cout << strHost << std::endl;
  // clean
  xen_session_logout(session);
  curl_global_cleanup();
  xen_fini();
  xmlCleanupParser();
}