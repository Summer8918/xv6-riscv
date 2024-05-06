/*
 * Write a concurrent prime sieve program for xv6 using pipes and the design illustrated in the picture halfway down this page(https://swtch.com/~rsc/thread/) and the surrounding text 
 * */
#include "kernel/types.h"
#include "user/user.h"

/* It takes a file descriptor fd as input and recursively 
filters out non-prime numbers from the input stream.
*/
void sieve(int fd) {
  int prime, i;
  if (read(fd, &prime, sizeof(prime)) != sizeof(prime)) {
    return;
  }
  printf("prime %d\n", prime);
  printf("prime %d, pid=%d\n", prime, getpid());

  int pipe_fds[2];
  pipe(pipe_fds);

  int pid = fork();
  /*Child Process recursively calls sieve() with a new pipe for communication.*/
  if (0 == pid) {
    close(pipe_fds[1]);
    sieve(pipe_fds[0]);
    close(pipe_fds[0]);
  }
  /* Parent Process reads integers from the pipe. If the integer is 
  not divisible by prime, it writes it to the pipe. 
  Then it waits for the child process to finish. */
  else {
    close(pipe_fds[0]);
    while (read(fd, &i, sizeof(i)) == sizeof(i)) {
      if (i % prime != 0) {
        write(pipe_fds[1], &i, sizeof(i));
      }
    }  
    close(pipe_fds[1]);
    wait(0);
  }
}

int main()
{
  int pipe_fds[2];
  pipe(pipe_fds);

  int pid = fork();
  if (0 == pid) {
    close(pipe_fds[1]);
    sieve(pipe_fds[0]);
    close(pipe_fds[0]);
  } else {
    close(pipe_fds[0]);
    for (int i = 2; i < 35; i++) {
      write(pipe_fds[1], &i, sizeof(i));
    }
    close(pipe_fds[1]);
    wait(0);
  }
  exit(0);
}
