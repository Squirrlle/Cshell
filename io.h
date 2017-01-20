#ifndef IO_H
#define IO_H

#ifdef XV6

#include "user.h"
#include "fcntl.h"

/* XXX no support for O_TRUNC yet */
#define O_TRUNC 0
/* XXX xv6 doesn't support file permissions */
#define S_IRWXU 0

#define EOF (-1)

#define fatal(...) printf(2, __VA_ARGS__), exit()
#define io_printf(...) printf(1, __VA_ARGS__)
#define io_getchar() ({ \
	char c; \
	int st = read(0, &c, 1); \
	(st < 1) ? EOF : (int) c; \
})

#else

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define fatal(...) fprintf(stderr, __VA_ARGS__), abort()
#define io_printf(...) printf(__VA_ARGS__)
#define io_getchar() getchar()

#endif

/* The macros above can be thought to provide these functions:

void fatal(char *s, ...);
int io_printf(char *fmt, ...);
int io_getchar(void);
*/

#endif
