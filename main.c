#include "tok.h"
#include "io.h"
#include "shtypes.h"
#include "parser.h"
#include "exec.h"
#include "syscalls.h"

#ifdef XV6
#define END_PROG exit()
#else
#define END_PROG return 0
#endif

int
main(int argc, char **argv)
{
  struct cmd *c;
  int child;
  write(1, "> ", 2);
  advance(); // initialize the tokenizer, setting the current token

  while(get_cur() != TOK_EOF)
  {
    c = parse();
    /* Execute in a subprocess so as to avoid cluttering our file descriptors
     * due to redirections--note that yet more subprocesses may also spawn
     */

    /* builtins need to be handled by the shell process */
    if (handle_builtin(c))
        goto done;

    if(child = fork1()) {
        while(wait1() != child)
            ;
    } else {
        exec_cmd(c);
        exit1();
    }

done:
    free_cmd(c);
    write(1, "> ", 2);
  }

  END_PROG;
}
