#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int i;
  int t;

  if (argc <= 1) {
      fprintf(2, "usage: sleep...\n");
      exit(1);
  }

  for(i = 1; i < argc; i++){
    t = atoi(argv[i]);
    sleep(t);
  }
  exit(0);
}