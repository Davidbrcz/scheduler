#ifndef __DEBUG_H__
#define __DEBUG_H__

#include<stdio.h>

#define debug_printf(fmt, ...) fprintf(stderr, fmt, ## __VA_ARGS__)
/* Debug printf */

void print_timestamp (void);
/* Print a timestamp */

#define DO_PRAGMA(x) _Pragma (#x)
#define TODO(x) DO_PRAGMA(message ("TODO - " #x))
/* Print information during           */

#endif /* __DEBUG_H__ */
