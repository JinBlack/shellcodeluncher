#define _GNU_SOURCE
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<sys/mman.h>
#include<fcntl.h>
#define FILEMODE S_IRWXU | S_IRGRP | S_IROTH
#define MAX 150

int main(int argc, char const *argv[])
{
    int fd, ret;
    size_t len_file, len;
    struct stat st;
    char *addr, *jmppoint;
    char buf[MAX];

    if (argc < 2)
    {
        printf("Usage a.out <filename>\n");
        return EXIT_FAILURE;
    }

    if ((fd = open(argv[1],O_RDWR | O_CREAT, FILEMODE)) < 0)
    {
        perror("Error in file opening");
        return EXIT_FAILURE;
    }

    if ((ret = fstat(fd,&st)) < 0)
    {
        perror("Error in fstat");
        return EXIT_FAILURE;
    }

    len_file = st.st_size;

    /*len_file having the total length of the file(fd).*/

    if ((addr = mmap(NULL,len_file,PROT_READ|PROT_WRITE|PROT_EXEC,MAP_SHARED,fd,0)) == MAP_FAILED)
    {
        perror("Error in mmap");
        return EXIT_FAILURE;
    }
    jmppoint = addr;
    char b;
    while (read(fd, &b, 1)){
        *(addr++) = b;
    }
     asm ("mov %1, %0\n\t"
     "jmp *%0"
     : "=r" (addr)
     : "r" (jmppoint));
     
    return 0;
}