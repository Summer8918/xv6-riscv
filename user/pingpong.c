#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char * argv[])
{
    // pipe fd
    int fd[2];
    if (pipe(fd) == -1) {
        fprintf(2, "Error: pipe(fd) == -1 \n");
    }
    if (fork() == 0) { // child process
        char buffer[1];
        read(fd[0], buffer, 1);
        fprintf(0, "%d: received ping\n", getpid());
        write(fd[1], buffer, 1);
    } else { // parent process
        char buffer[1];
        buffer[0] = 'a';
        write(fd[1], buffer, 1);
        close(fd[1]);
        read(fd[0], buffer, 1);
        fprintf(0, "%d: received pong\n", getpid());
    }
    exit(0);
}