#include "syscalls.h"
#include "io.h"

#ifdef XV6

#else

void
exec(char *p, char **argv)
{
  execvp(p, argv);
}

int
wait1(void)
{
  return wait(NULL);
}

void
exit1(void)
{
  exit(0);
}

#endif

int
fork1(void)
{
  int pid = fork();
  if (pid == -1)
    fatal("fork failed! out of memory?\n");
  return pid;
}

