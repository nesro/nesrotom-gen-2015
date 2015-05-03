/* vstup.cpp */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int g_debug_level;

enum {
	MaxDelkaRadku = 256
};

char Radek[MaxDelkaRadku];
int CisloRadku;
char *Aktualni = Radek;
FILE *Vstup;

void InitVstup(char* jmeno) {
	if (!jmeno)
		Vstup = stdin;
	else {
		Vstup = fopen(jmeno, "rt");
		if (!Vstup) {
			printf("vstupni soubor %s nenalezen\n", jmeno);
			exit(1);
		}
	}
}

int CtiZnak() {
	int delka;
	if (!*Aktualni) {
		if (!fgets(Radek, MaxDelkaRadku, Vstup))
			return EOF;
		Aktualni = Radek;
		CisloRadku++;
		delka = strlen(Radek);
		if (Radek[delka - 1] != '\n') {
			Radek[delka] = '\n';
			Radek[delka + 1] = 0;
		}

		if (g_debug_level > 0)
			fprintf(stderr, "%-3d %s", CisloRadku, Radek);
	}
	return *Aktualni++;
}
