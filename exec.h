#ifndef EXEC_H
#define EXEC_H

#include "shtypes.h"

int handle_builtin(struct cmd *);
int exec_cmd(struct cmd *c);
struct Node
{
  int pid;
  char *pro;
  struct Node* next;
};
struct Node* head;

#endif
