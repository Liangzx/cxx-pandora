#include <fcntl.h>
#include <fcntl.h> /* For O_* constants */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/stat.h> /* For mode constants */
#include <sys/types.h>
#include <unistd.h>

int main() {
    int fd = shm_open("posix_shm", O_RDONLY, 0666);
    size_t size = 1024 * 1024;// 1MB
    ftruncate(fd, size);

    char *p = mmap(NULL, size, PROT_READ, MAP_SHARED, fd, 0);

    printf("%c %c %c %c\n", p[0], p[1], p[2], p[3]);
    munmap(p, size);

    return 0;
}
