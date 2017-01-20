#ifndef EXEC_H
#define EXEC_H

#include "shtypes.h"

int handle_builtin(struct cmd *);
void exec_cmd(struct cmd *c);

#endif
