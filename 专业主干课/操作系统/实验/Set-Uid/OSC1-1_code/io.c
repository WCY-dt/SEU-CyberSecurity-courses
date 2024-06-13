#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>

void do_work() {
    int fd = open("/tmp/file", O_WRONLY | O_CREAT | O_TRUNC, 
		  S_IRUSR | S_IWUSR);
    assert(fd >= 0);
    char buffer[20];
    sprintf(buffer, "hello world\n");
    int rc = write(fd, buffer, strlen(buffer));
    assert(rc == strlen(buffer));
    printf("wrote %d bytes\n", rc);
    fsync(fd);
    close(fd);
}

int main(int argc, char *argv[]) {
    do_work();
    return 0;
}

