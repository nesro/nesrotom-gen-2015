/* tabsym.cpp */

#include "tabsym.h"
#include "debug.h"
#include "code.h"
#include <string.h>
#include <stdio.h>

/* from main.cpp */
extern Storage *g_s;

struct PrvekTab {
	char *ident;
	DruhId druh;
	int hodn;
	PrvekTab *dalsi;
	PrvekTab(char *i, DruhId d, int h, PrvekTab *n);
};

PrvekTab::PrvekTab(char *i, DruhId d, int h, PrvekTab *n) {
	_fn();
	/* TODO: strdup? :D */
	ident = new char[strlen(i) + 1];
	strcpy(ident, i);
	druh = d;
	hodn = h;
	dalsi = n;
	g_s->mem_top++;
	_return_void;
}

static PrvekTab *TabSym;

static void Chyba(char *id, const char *txt) {
	_fn();
	printf("identifikator %s: %s\n", id, txt);
	_return_void;
}

PrvekTab *hledejId(char *id) {
	_fn();
	PrvekTab *p = TabSym;
	while (p) {
		if (strcmp(id, p->ident) == 0) {
			_return(p);
		} else {
			p = p->dalsi;
		}
	}
	_return(NULL);
}

void deklKonst(char *id, int val) {
	_fn();
	PrvekTab *p = hledejId(id);
	if (p) {
		Chyba(id, "druha deklarace");
		_return_void;
	}
	TabSym = new PrvekTab(id, IdKonst, val, TabSym);
	_return_void;
}

void deklProm(char *id) {
	_fn();
	static int volna_adr;
	PrvekTab *p = hledejId(id);
	if (p) {
		Chyba(id, "druha deklarace");
		_return_void;
	}
	TabSym = new PrvekTab(id, IdProm, volna_adr, TabSym);
	volna_adr++;
	_return_void;
}

int adrProm(char *id) {
	_fn();
	PrvekTab *p = hledejId(id);
	if (!p) {
		Chyba(id, "neni deklarovan");
		_return(0);
	} else if (p->druh != IdProm) {
		Chyba(id, "neni identifikatorem promenne");
		_return(0);
	} else
		_return(p->hodn);
}

DruhId idPromKonst(char *id, int *v) {
	_fn();
	PrvekTab *p = TabSym;
	while (p) {
		if (strcmp(id, p->ident) == 0) {
			*v = p->hodn;
			_return(p->druh);
		} else {
			p = p->dalsi;
		}
	}
	Chyba(id, "neni deklarovan");
	_return(Nedef);
}
