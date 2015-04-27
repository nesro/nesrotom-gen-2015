#include "debug.h"

int _debug_depth = 0;

void print_debug_depth() {
	(void) fprintf(stderr, "d ");
	for (int _debug_i = 0; _debug_i < _debug_depth; _debug_i++) {
		(void) fprintf(stderr, "|----");
	}
}

void __fn_inner(const char *file, int line, const char *function) {
	if (g_debug_level < 10) {
		return;
	}
	assert(0 <= _debug_depth);
	_debug_depth++;
	print_debug_depth();
	(void) fprintf(stderr, "%s:%d | %s\n", file, line, function);
}

void __return_inner(const char *file, int line, const char *function,
		const char *ret) {
	if (g_debug_level < 10) {
		return;
	}
	print_debug_depth();
	(void) fprintf(stderr, "%s:%d | %s - return(%s)\n", file, line, function,
			ret);
	_debug_depth--;
}

void __return_void_inner(const char *file, int line, const char *function) {
	if (g_debug_level < 10) {
		return;
	}
	print_debug_depth();
	(void) fprintf(stderr, "%s:%d | %s - return(void)\n", file, line, function);
	_debug_depth--;
}
