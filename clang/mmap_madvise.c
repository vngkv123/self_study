#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]){
    int ret = open("./a.out", O_RDONLY);
    if( ret < 0 ){
        perror("open");
        exit(-1);
    }

   void *mmap_res = mmap(0, 0x1000, PROT_READ, MAP_PRIVATE, ret, 0);
   printf("%lx\n", mmap_res);

    madvise( mmap_res, 0x1000, 4);
//       int madvise(void *addr, size_t length, int advice);
//       MADV_DONTNEED = 4
//       asm-generic/mman-common.h
//

    char buf;
    while(read(ret, &buf, 1) > 0){
        printf("%c", buf);
    }

    return 0;
}
