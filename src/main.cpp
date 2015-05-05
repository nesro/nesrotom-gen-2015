#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

#include "debug.h"
#include "lexan.h"
#include "parser.h"
#include "strom.h"
#include "code.h"

/* global variables */
TmSource *g_ts = NULL;
Storage *g_s = NULL;
int g_debug_level = 0;
int g_optimize_level = 0;
int g_verbose_level = 0;
char *g_input = NULL;
const char *g_output = NULL;
int g_run_stack_machine = 0;
extern int g_debug_level;

static void usage(void) {
	_fn();
	printf("Usage: ./main -o outputfile inputfile\n");
	exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
	_fn();
	int index;
	int c;
	char *strtol_tmp;
	/* parse arguments */
	opterr = 0;

	while ((c = getopt(argc, argv, "d:o:O:sv:")) != -1) {
		switch (c) {
		case 'd':
			g_debug_level = strtol(optarg, &strtol_tmp, 10);
			break;

		case 'o':
			g_output = optarg;
			break;

		case 'O':
			g_optimize_level = strtol(optarg, &strtol_tmp, 10);
			break;

		case 'v':
			g_verbose_level = strtol(optarg, &strtol_tmp, 10);
			break;

		case 's':
			g_run_stack_machine = 1;
			break;

		case '?':
			if (optopt == 'd' || optopt == 'o' || optopt == 'O'
					|| optopt == 'v') {
				fprintf(stderr, "Option -%c requires an "
						"argument.\n", optopt);
			} else if (isprint(optopt)) {
				fprintf(stderr, "Unknown option '-%c'.\n", optopt);
			} else {
				fprintf(stderr, "Unknown option character '\\x%x'.\n", optopt);
			}

			usage();
			_return(EXIT_FAILURE);
			break;

		default:
			abort();
		}
	}

	if (argc == 1 || argc <= optind) {
		fprintf(stderr, "Missing input file. (optind=%d, argc=%d)\n", optind,
				argc);
		usage();
		_return(EXIT_FAILURE);
	}

	index = optind;

	if (strcmp(argv[index], "-") != 0) {
		g_input = argv[index];
	}

	for (index++; index < argc; index++) {
		printf("Non-option argument %s\n", argv[index]);
	}

	if (0 < g_debug_level) {
		printf("args: input=%s, output=%s\n", g_input ? g_input : "stdin",
				g_output ? g_input : "stdout");
	}

	g_ts = new TmSource();
	g_s = new Storage();

	InitLexan(g_input);
	CtiSymb();
	Prog *prog = Program();

	if (0 < g_optimize_level) {
		prog = (Prog *) (prog->Optimize());
	}

	prog->Translate();

	g_ts->print(g_output, true);

	delete g_ts;
	delete g_s;

	_return(EXIT_SUCCESS);
}

