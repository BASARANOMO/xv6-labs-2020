#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  char buf[1];
  int n;
  int pid;
  int p1[2];
  int p2[2];
  pipe(p1);
  pipe(p2);
  if (fork() == 0) {
      pid = getpid();
      close(p1[1]);
      close(p2[0]);
      n = read(p1[0], buf, 1);
      if (n) {
          printf("%d: received ping\n", pid);
      } else {
          fprintf(2, "usage: pingpong parent--->child read error!\n");
          exit(1);
      }
      if (!write(p2[1], buf, 1)) {
          fprintf(2, "usage: pingpong child--->parent write error!\n");
          exit(1);
      }
      exit(0);
  } else {
      pid = getpid();
      close(p1[0]);
      close(p2[1]);
      if (!write(p1[1], buf, 1)) {
          fprintf(2, "usage: pingpong parent--->child write error!\n");
          exit(1);
      }
      n = read(p2[0], buf, 1);
      if (n) {
          printf("%d: received pong\n", pid);
      } else {
          fprintf(2, "usage: pingpong child--->parent read error!\n");
          exit(1);
      }
      exit(0);
  }
  exit(0);
}