#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void
prime(int input_fd)
{
  int base;
  if (read(input_fd, &base, sizeof(int)) == 0) { // exit if last child
    exit(0);
  }
  printf("prime %d\n", base);
  int p[2];
  pipe(p);
  if (fork() == 0) { // child
    close(p[1]); // close write end
    prime(p[0]);
  } else { // parent
    close(p[0]); // close read end
    int n;
    int eof;
    do {
      eof = read(input_fd, &n, sizeof(int));
      if (n % base != 0) {
        write(p[1], &n, sizeof(int));
      }
    } while (eof); // while eof != 0
    close(p[1]);
  }
  wait(0);
  exit(0);
}

int
main(int argc, char *argv[])
{
  int parent_fd[2];
  pipe(parent_fd);
  if (fork()) { // parent
    close(parent_fd[0]); // write only
    int i;
    for (i = 2; i < 36; i++) {
      write(parent_fd[1], &i, sizeof(int));
    }
    close(parent_fd[1]);
  } else { // child
    close(parent_fd[1]); // read only
    prime(parent_fd[0]);
  }
  wait((int *) 0);
  exit(0);
}