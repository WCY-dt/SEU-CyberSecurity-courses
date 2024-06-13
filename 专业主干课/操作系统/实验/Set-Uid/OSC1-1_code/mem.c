#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "common.h"

int
main(int argc, char *argv[])
{
    if (argc != 2) { 
	fprintf(stderr, "usage: mem <value>\n"); 
	exit(1); 
    } 
    int *p;                   // memory for pointer is on "stack"
    p = malloc(sizeof(int));  // malloc'd memory is on "heap"
    assert(p != NULL);
    printf("(pid:%d) addr of p:        %llx\n", (int) getpid(), 
	   (unsigned long long) &p);
    printf("(pid:%d) addr stored in p: %llx\n", (int) getpid(), 
	   (unsigned long long) p);
    *p = atoi(argv[1]);       // assign value to addr stored in p
    while (1) {
	Spin(1);
	*p = *p + 1;
	printf("(pid:%d) value of p: %d\n", getpid(), *p);
    }

    return 0;
}

