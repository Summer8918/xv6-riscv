
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "kernel/param.h"

/*
Write a simple version of the UNIX xargs program for xv6: 
its arguments describe a command to run, 
it reads lines from the standard input, 
and it runs the command for each line, 
appending the line to the command's arguments. 
*/
int main(int argc, char *argv[])
{
  if (argc < 2) {
    fprintf(2, "Usage: xargs <command> [args...]\n");
    exit(1);
  }

  char buf[512];
  char *p = buf;
  char ch;

  char* new_argv[MAXARG];

  if(argc + 1 >= MAXARG) {
    fprintf(2, "xargs: too many arguments\n");
    exit(1);
  }
  for(int i = 1; i < argc; ++i) {
    new_argv[i-1] = argv[i];
  }

  // Caution: content of buf may change
  new_argv[argc - 1] = buf;
  new_argv[argc] = 0;

  while (read(0, &ch, 1) == 1) {
    if('\n' != ch) {
      *p++ = ch;
    } else {
      *p = '\0';
      int pid = fork();
      if (0 == pid) {
        exec(argv[1], new_argv);

        printf("exec() failed\n");
        exit(1);
      } else {
        wait(0);
      }

      p = buf;
    }
  }
  exit(0);
}

