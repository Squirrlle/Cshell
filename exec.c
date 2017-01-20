#include "shtypes.h"
#include "syscalls.h"
#include "io.h"

int
handle_builtin(struct cmd *c)
{
  if (!c)
    return 0;

  switch (c->tp)
  {
  case EXEC:
    if (strcmp(c->exec.argv[0], "echo") == 0) {
      for (int i = 1; ; i++)
        if (c->exec.argv[i])
          io_printf(c->exec.argv[i]);
        else
          break;
      printf("\n");
      return 1;
    } else {
      /* TODO: implement cd, pwd, exit */
      return 0;
    }
  default:
    return 0;
  }
}

void
exec_cmd(struct cmd *c)
{
  // don't try to execute a command if parsing failed.
  if (!c)
    return;

  int child;

  switch (c->tp)
  {
  case EXEC:
    if(child = fork1()) {
      while(wait1() != child) {
        /* Ignore spurious deaths */
      }
    } else {
      exec(c->exec.argv[0], c->exec.argv);
      fatal("exec failed! command not found or out of memory?\n");
    }
    break;

  case PIPE:
    /* TODO */
    break;

  case LIST:
    /* TODO */
    break;

  case REDIR:
    /* TODO */
    break;

  case BACK:
    /* TODO */
    break;

  default:
    fatal("BUG: exec_cmd unknown command type\n");
  }
}
