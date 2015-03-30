#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include "lexan.h"
#include "parser.h"
#include "strom.h"
#include "zaspoc.h"

/* global variables */
int g_debug_level = 0;
int g_optimize_level = 0;
int g_verbose_level = 0;
char *g_input = NULL;
char *g_output = NULL;

static void
usage(void)
{
	printf("Usage: ./main -o outputfile inputfile\n");
	exit(EXIT_FAILURE);
}

int
main(int argc, char *argv[])
{
	int index;
	int c;
	char *strtol_tmp;

	/* parse arguments */
	opterr = 0;
	while ((c = getopt(argc, argv, "d:o:O:v:")) != -1) {
		switch (c) {
		case 'd':
			g_debug_level = strtol(optarg, &strtol_tmp, 10);
			break;
		case 'o':
			printf("out!\n");
			g_output = optarg;
			break;
		case 'O':
			g_optimize_level = strtol(optarg, &strtol_tmp, 10);
			break;
		case 'v':
			g_verbose_level = strtol(optarg, &strtol_tmp, 10);
			break;
		case '?':
			if (optopt == 'd' || optopt == 'o' || optopt == 'O' ||
			    optopt == 'v') {
				fprintf (stderr, "Option -%c requires an "
				   "argument.\n", optopt);
			} else if (isprint(optopt)) {
				fprintf (stderr, "Unknown option '-%c'.\n",
				    optopt);
			} else {
				fprintf (stderr,
				    "Unknown option character '\\x%x'.\n",
				    optopt);
			}
			usage();
			return (EXIT_FAILURE);
		default:
			abort();
		}
	}
	if (argc == 1 || argc <= optind) {
		fprintf(stderr, "Missing input file. (optind=%d, argc=%d)\n",
		    optind, argc);
		usage();
		return (EXIT_FAILURE);
	}
	index = optind;
	if (strcmp(argv[index], "-") != 0) {
		g_input = argv[index];
	}
	for (index++; index < argc; index++) {
		printf("Non-option argument %s\n", argv[index]);
	}

	if (0 < g_debug_level) {
		printf("args: input=%s, output=%s\n",
		    g_input ? g_input : "stdin", g_output ? g_input : "stdout");
	}

	InitLexan(g_input);
	CtiSymb();
	Prog *prog = Program();
	if (0 < g_optimize_level) {
		prog = (Prog*)(prog->Optimize());
	}
	prog->Translate();
	Print();
	/* Run(); ./interpreter/tm will run the translated code */

	return (EXIT_SUCCESS);
}

