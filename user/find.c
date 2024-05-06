#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

/*
Write a simple version of the UNIX find program for xv6: 
find all the files in a directory tree with a specific name.
*/
void find(const char *path, const char* filename) {
  struct stat st;
  if (stat(path, &st) < 0) {
    fprintf(2, "find: cannot stat %s\n", path);
    // exit(1);
    return;
  }
  //fprintf(1, "filename %s\n", filename);
  if((st.type == T_DEVICE) || (st.type == T_FILE)) {
    if (strlen(path) >= strlen(filename)) {
      if(strcmp(path + strlen(path) - strlen(filename), filename) == 0) {
        printf("%s\n", path);
      }
    }
  } else {
    // "path" refers to a directory
    struct dirent de;
    char deeper_path[512];
    if (strlen(path) + 1 + DIRSIZ + 1 > sizeof(deeper_path)) {
      printf("find: path too long!\n");
      return;
    }
    char *p;
    strcpy(deeper_path, path);
    p = deeper_path + strlen(deeper_path);
    *p++ = '/';

    int fd = open(path, O_RDONLY);
    if(fd < 0) {
      fprintf(2, "find: cannot open the directory %s\n", path);
      exit(1);
    }

    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
      if (de.name[0] != '\0') {
        if ((strcmp(de.name, ".") == 0) || (strcmp(de.name, "..") == 0)) {
          continue;
        }
        strcpy(p, de.name);
        // printf("go into deeper path: %s\n", deeper_path);
        // recursive call
        find(deeper_path, filename);
      }
    }
    close(fd);
  }
}

int main(int argc, char const *argv[])
{
  if(3 != argc) {
    fprintf(2, "Usage: find <path> <filename>\n");
    exit(1);
  }

  find(argv[1], argv[2]);

  exit(0);
}
