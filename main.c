#include "tok.h" // grabs characters from stdin and creates tokens 
#include "io.h" //importing syscalls & file handling including printing & scanning
#include "shtypes.h" //importing the command struct
#include "parser.h" //consumes tokens and creates command structs
#include "exec.h" // functions that handle built-ins and run programs (execvp)
#include "syscalls.h" // import more syscalls and define fork1

extern struct cmd** hs;

/*argc : number of argumnets, argv : the arguments itself */
int main(int argc, char **argv)
{
  struct cmd *c; // pointer to a command struct
  int child; // child process pid
  hs = (struct cmd **)malloc(100 * sizeof(struct cmd *));

  /* already written: Prints out a banner telling the user about the shell.*/
  printf("simple sh, " __DATE__ " " __TIME__ "\nPress control+C or control+D to exit.\n");
  
  char hostbuffer[256];
  int hostname;
  hostname = gethostname(hostbuffer, sizeof(hostbuffer));
  char cwdbuff[256];
  // TODO for students: you need to create the prompt, username, hostname and current working directory
  printf("%s@%s:~%s$ ", getlogin(), hostbuffer, getcwd(cwdbuff, sizeof(cwdbuff))); // implementation

  /* already written : initialize the tokenizer */
  advance();

  /* already written : part of the tokenizer and parser */
  while(get_cur() != TOK_EOF)
  {
    c = parse();
	  print_cmd(c);

    /* Note: you need to complete the implementation of this function inside exec.c
    built-ins need to be handled by the shell process */
    if (handle_builtin(c))
        goto done;

  /* fork : Copy yourself to a child and a parent
	/* already written: fork1 is essentialy fork but also quits the program if fork fails */
    if((child = fork1())) {
		/* already written: This block runs in the parent, with variable child equal to the PID of the
		 * child process below (in else). */
		int code;

		/* already written: Wait for specifically the child with the given PID
		 * to die. This *should* be our only child, but it doesn't hurt to be safe. */
        while(wait(&code) != child);

		/* already written : prints exit code of last command */
		printf("command exited with code %d\n", WEXITSTATUS(code));
    } 
    else {
		/* already written: This block runs in the child. runs exec_cmd and 
    exits with the return value of exec_cmd, supposed to return 0 */
    /* Note : you need to fill in the implementation of exec_cmd inside exec.c */
        exit(exec_cmd(c));
    }

done:
	/* already written: Release any memory associated with the command. */
    free_cmd(c);
  //TODO for students: This is a good place to free memory you allocated in this loop and you don't need anymore
  
  // TODO for students: you need to print the prompt, username, hostname and current working directory
	printf("%s@%s:~%s$ ", getlogin(), hostbuffer, getcwd(cwdbuff, sizeof(cwdbuff)));
  }

  return 0;
}