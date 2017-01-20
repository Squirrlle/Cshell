#ifdef XV6
#include "types.h"
#include "user.h"

#define wait1 wait
#define exec1 exec
#define exit1 exit

#else

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

void exec(char *p, char **argv);
int wait1(void);
void exit1(void);

#endif

int fork1(void);
