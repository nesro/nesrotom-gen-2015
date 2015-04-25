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

#define _return(r) do { \
	__return_inner(__FILE__, __LINE__,  __FUNCTION__, #r); \
	return (r); \
} while(0)

#define _return_void \
	__return_void_inner(__FILE__, __LINE__,  __FUNCTION__)

#endif

