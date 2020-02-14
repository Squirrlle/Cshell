#include "shtypes.h"
#include "syscalls.h"
#include <dirent.h> 
#include "io.h"
#include "exec.h"
//Very Jank History Implimentation 
char *hist[100];
struct cmd **hs;
int pos = 0;
int pd = 0;
extern struct Node* head;
pid_t jbs[100];
int nj = 0;

int addNode(struct Node* n){
  struct Node* temp = head;
  while (temp->next != NULL)
  {
    temp = temp->next;
  }
  temp->next = n;
  return 0;
}

int printNode(){
  struct Node* temp = head;
  while (temp->next != NULL)
  {
    temp = temp->next;
    printf("%d\t%s\n", temp->pid, temp->pro);
  }
  return 0;
}

int handle_builtin(struct cmd *c) /* c is the pointer to a cmd struct */
{
  /* already written : make sure c parsed properly and if not return 0 */
  if (!c)
    return 0;
  /* already written : tp is the type of the command, like Redir, background, 
  exec, list, or pipe (shtypes.h) */
  switch (c->tp) /* tp is the field out of the struct that we are pointing to (using the ->) */
  {
  /* Note : only if c is an exec then it could be a built-in */
  case EXEC: // Note : EXEC is an enum (shtypes.h)
    /* echo command already written */
    /* if comparing c->exec.argv[0] and "echo" gives 0 then they are 
    equal and you loop over the other arguments and print them out */
    if (strcmp(c->exec.argv[0], "echo") == 0) 
    {
      for (int i = 1; ; i++)
        if (c->exec.argv[i])
          printf("%s ", c->exec.argv[i]);
        else
          break;
      printf("\n");
      char temp[80];
      sprintf(temp, "echo %s", c->exec.argv[1]);
      puts(temp);
      hist[pos] = strdup(temp);
      pos++;
      return 1;
	  }
    else if (strcmp(c->exec.argv[0], "pwd") == 0) 
    {
      char cwdbuff[256];
      printf("%s", getcwd(cwdbuff, sizeof(cwdbuff)));
      printf("\n");
      hist[pos] = "pwd";
      pos++;
      return 1;
	  }
    else if (strcmp(c->exec.argv[0], "cd") == 0) 
    {
      chdir(c->exec.argv[1]);
      char temp[80] = "cd ";
      sprintf(temp, "cd %s", c->exec.argv[1]);
      puts(temp);
      hist[pos] = strdup(temp);
      pos++;
      return 1;
    }  
    else if (strcmp(c->exec.argv[0], "clear") == 0) 
    {
      system("clear");
      hist[pos] = "clear";
      pos++;
      return 1;
    }
    else if (strcmp(c->exec.argv[0], "jobs") == 0) 
    {
      struct Node* temp = head;
      printf("Got here\n");
      //if(printNode()){}
      pid_t tp;
      printNode();
      hist[pos] = "jobs";
      pos++;
      return 1;
    }
    else if (strcmp(c->exec.argv[0], "history") == 0) 
    {
      for(int i = 0; i < pos; i++){
        printf("%d %s\n", i, hist[i]);
      }
      hist[pos] = "history";
      pos++;
      return 1;
    }
    else if (c->exec.argv[0][0] == '!')
    {
      if(c->exec.argv[0]){
        printf("Got here\n");
        int temp = atoi(c->exec.argv[0]+1);
        printf("%d, going to yeet %p\n", temp, hs[temp]);
        struct cmd* old_cmd = hs[temp];
        old_cmd->tp;
        print_cmd(old_cmd);
        if(handle_builtin(hs[temp])){
          return 1;
        }
      }
      return 1;
    } 
    else if (strcmp(c->exec.argv[0], "kill") == 0) 
    {
      return 1;
    }
    else if (strcmp(c->exec.argv[0], "help") == 0) 
    {
      printf("jobs: provide a numbered list of processes currently executing in the background.\n");
      printf("cd [Directory]: changes the working directory\n");
      printf("pwd: print working directory\n");
      printf("exit: terminate your shell\n");
      printf("echo [String]: prints out the input\n");
      printf("kill [num]: terminates the process\n");
      printf("help");
      printf("clear: clears the terminal\n");
      hist[pos] = "help";
      pos++;
      return 1;
    }
    /* exit command already written */ 
    else if (strcmp(c->exec.argv[0], "exit") == 0) 
    {
      int status;
      pid_t w;
      while((w = waitpid(-1, &status, WNOHANG)) > 0){
        printf("Waiting on process to end\n");
      }
		  int code = 0;
		  if(c->exec.argv[1]) {
			  code = atoi(c->exec.argv[1]);
		  }
	  	exit(code); /* kills your process and returns code */
    }
    // TODO for students: implement kill
    else {
      return 0;
    }
  default:
    return 0; //Note : returns 0 when you call handle_builtin in main
  }
}

/* Most of your implementation needs to be in here, so a description of this
 * function is in order:
 *
 * int exec_cmd(struct cmd *c)
 *
 * Executes a command structure. See shtypes.h for the definition of the
 * `struct cmd` structure.
 *
 * The return value is the exit code of a subprocess, if a subprocess was
 * executed and waited for. (Id est, it should be the low byte of the return
 * value from that program's main() function.) If no return code is collected,
 * the return value is 0.
 *
 * For pipes and lists, the return value is the exit code of the rightmost
 * process.
 *
 * The function does not change the assignment of file descriptors of the
 * current process--but it may fork new processes and change their file
 * descriptors. On return, the shell is expected to remain connected to
 * its usual controlling terminal (via stdin, stdout, and stderr).
 *
 * This will not be called for builtins (values for which the function above
 * returns a nonzero value).
 */
int exec_cmd(struct cmd *c)
{
  /* don't try to execute a command if parsing failed. */
  if (!c)
    return -1;
  
  int child;

  switch (c->tp)
  {
  case EXEC:
	  // TODO for students: What will run a command? (Hint: use execvp syscall)
    if (handle_builtin(c))
        goto done;
    hist[pos] = c->exec.argv[0];
    pos++;
    pid_t pid;
    int status;
    if((pid = fork()) < 0){
          printf("*** ERROR: forking child process failed\n");
          exit(1);
     }
     else if (pid == 0) {          
          if (execvp(c->exec.argv[0], c->exec.argv) < 0) {
               printf("*** ERROR: exec failed\n");
               exit(1);
          }
     }
     else {
          while (wait(&status) != pid);
     }
    break;

  case PIPE:
	/* TODO for students: Run *two* commands, but with the stdout chained to the stdin
	 * of the next command. How? Use the syscall pipe: */
	{
		int pipefds[2];
		pipe(pipefds);
    int p;
    pid_t pt = fork();
    if (pt < 0){
      printf("Fork Failed\n");
      return -1;
    }
    if(pt == 0){
      close(pipefds[1]);
      dup2(pipefds[0], 0);
      if(execvp(c->pipe.right->exec.argv[0], c->pipe.right->exec.argv)){
        return 1;
      }
    }
    else
    {
      close(pipefds[0]);
      dup2(pipefds[1], 1);
      if(execvp(c->pipe.left->exec.argv[0], c->pipe.left->exec.argv)){
        return 1;
      } 
    }
		/* At this point, pipefds[0] is a read-only FD, and pipefds[1] is a
		 * write-only FD. Make *absolutely sure* you close all instances of
		 * pipefds[1] in processes that aren't using it, or the reading child
		 * will never know when it needs to terminate! */ 
	}
    break;

  case LIST:
	/* The LIST case is already written */
	exec_cmd(c->list.first);
	return exec_cmd(c->list.next);
    break;

  case REDIR:
   //You need to use the fields in the c->redir struct here; fd is the file 
   //descriptor you are going to redirect with (STDIN or STDOUR), the path 
   //is the file you need to open for redirecting, the mode is how to open
   //the file you are going to use, and the cmd field is what command to run
  {  
    pid_t pt = fork();
    if (pt < 0){
      printf("Fork Failed\n");
      return -1;
    }
    if (pt == 0) {
      int fd = open(c->redir.path, c->redir.mode, 0777);
      dup2(fd, c->redir.fd);
      close(fd);
      exit(exec_cmd(c->redir.cmd));
    }
    int t;
    while (wait(&t) != pt){}
    return 1;   
  }
    break;

  case BACK:
	/* TODO for students: This should be easy--what's something you can remove from EXEC to have
	 * this function return before the child exits? */
  {
    struct Node* n = NULL;
    n = (struct Node*)calloc(1, sizeof(struct Node));
    n->pid = 0;
    n->pro = "cmd";
    if (addNode(n)){}
    pid_t pt = fork();
    jbs[nj++] = pt;
    if (pt < 0){
      printf("Fork Failed\n");
      return -1;
    }
    if (pt == 0) {
      exit(exec_cmd(c->back.cmd));
    }
  }
    break;

  default:
    fatal("BUG: exec_cmd unknown command type\n");
  }
  done:
    free_cmd(c);
}