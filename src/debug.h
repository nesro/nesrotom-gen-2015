/* nesrotom@fit.cvut.cz 2015
 * warning: this code causes eye bleeding */

#include <stdio.h>
#include <assert.h>

#ifndef _debug_h_
#define _debug_h_

extern int g_debug_level;
extern int _debug_depth;

/*****************************************************************************/

void print_debug_depth();
void __fn_inner(const char *file, int line, const char *function);
void __return_inner(const char *file, int line, const char *function,
		const char *ret);
void __return_void_inner(const char *file, int line, const char *function);

/*****************************************************************************/

#define _fn() \
	__fn_inner(__FILE__, __LINE__,  __FUNCTION__)

#define _return(r) do { \
	__return_inner(__FILE__, __LINE__,  __FUNCTION__, #r); \
	return (r); \
} while(0)

#define _return_void \
	__return_void_inner(__FILE__, __LINE__,  __FUNCTION__)

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define _debug(msg, ...) do { \
		print_debug_depth(); \
		fprintf(stderr, KRED msg KNRM, __VA_ARGS__); \
		fflush(stderr); \
} while (0);

#define _debug2(msg) do { \
		print_debug_depth(); \
		fprintf(stderr, KRED msg KNRM); \
		fflush(stderr); \
} while (0);

#endif

