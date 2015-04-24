/* nesrotom@fit.cvut.cz 2015
 * warning: this code causes eye bleeding */

#include <stdio.h>
#include <assert.h>

#ifndef _debug_h_
#define _debug_h_

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

#define _return(r) \
	__return_inner(__FILE__, __LINE__,  __FUNCTION__, #r); return (r)

#define _return_void \
	__return_void_inner(__FILE__, __LINE__,  __FUNCTION__)

/*****************************************************************************/

/* #define _return(r) do { \
	assert(0 <= _debug_depth); \
	print_debug_depth(); \
	(void) fprintf(stderr, "%s:%d | %s return\n", __FILE__, __LINE__, \
	     __FUNCTION__); \
	_debug_depth--; \
	return (r); \
} while (0) */

/* #define _return_void do { \
	assert(0 <= _debug_depth); \
	print_debug_depth(); \
	(void) fprintf(stderr, "%s:%d | %s return void\n", __FILE__, __LINE__, \
	     __FUNCTION__); \
	_debug_depth--; \
} while (0) */

#endif

