#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  char buf[1];
  int n;
  int pid;
  int p[2];
  pipe(p);
  if (fork() == 0) {
      pid = getpid();
      n = read(p[1], buf, 1);
      if (n) {
          printf("%d: received ping\n", pid);
      }
      write(p[0], buf, 1);
      exit(0);
  } else {
      pid = getpid();
      write(p[1], buf, 1);
      wait((int *) 0);
      n = read(p[0], buf, 1);
      if (n) {
          printf("%d: received pong\n", pid);
      }
      exit(0);
  }
  exit(0);
}