/* This example shows how to do synchronous reads
 * and writes randomly over the first megabyte of an
 * NBD server.  Note this will destroy any existing
 * content on the NBD server.
 *
 * To test it with nbdkit and a RAM disk:
 *
 * nbdkit -U - memory 1M \
 *     --run './simple-reads-and-writes $unixsocket'
 */

#include <assert.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <libnbd.h>

int main(int argc, char* argv[])
{
  struct nbd_handle* nbd;
  char buf[512];
  size_t i;
  int64_t exportsize;
  uint64_t offset;

  srand(time(NULL));

  /* Create the libnbd handle. */
  nbd = nbd_create();
  if (nbd == NULL) {
    fprintf(stderr, "%s\n", nbd_get_error());
    exit(EXIT_FAILURE);
  }

  /* Connect to the NBD server over a
   * Unix domain socket.
   */
  if (nbd_connect_tcp(nbd, "172.20.30.176", "10809") == -1) {
    fprintf(stderr, "%s\n", nbd_get_error());
    exit(EXIT_FAILURE);
  }

  /* Get the size of the disk and check
   * it's large enough.
   */
  exportsize = nbd_get_size(nbd);
  if (exportsize == -1) {
    fprintf(stderr, "%s\n", nbd_get_error());
    exit(EXIT_FAILURE);
  }
  assert(exportsize >= sizeof buf);

  fprintf(stderr, "exportsize: %ld\n", exportsize);

  char *s = nbd_get_export_name (nbd);
  printf ("requested export name: %s\n", s ? s : "NULL");
  free (s);


  /* Sends a graceful shutdown to the server. */
  if (nbd_shutdown(nbd, 0) == -1) {
    fprintf(stderr, "%s\n", nbd_get_error());
    exit(EXIT_FAILURE);
  }

  nbd_close(nbd);

  exit(EXIT_SUCCESS);
}
