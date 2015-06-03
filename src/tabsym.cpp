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

	int value;
	bool value_is_set;
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

	value = 0;
	value_is_set = false;

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

void nastav(int adr, int val) {
	_fn();
	PrvekTab *p = TabSym;
	while (p) {
		if (adr == p->hodn) {
			p->value = val;
			p->value_is_set = true;
			_return_void;
		} else {
			p = p->dalsi;
		}
	}
	assert(0);
	_return_void;
}
void odnastav(int adr) {
	_fn();
	PrvekTab *p = TabSym;
	while (p) {
		if (adr == p->hodn) {
			p->value_is_set = false;
			_return_void;
		} else {
			p = p->dalsi;
		}
	}
	assert(0);
	_return_void;
}
bool je_nastaveno(int adr) {
	_fn();
	PrvekTab *p = TabSym;
	while (p) {
		if (adr == p->hodn) {
			_return(p->value_is_set);
		} else {
			p = p->dalsi;
		}
	}
	assert(0);
	_return(false);
}
int vrat(int adr) {
	_fn();
	PrvekTab *p = TabSym;
	while (p) {
		if (adr == p->hodn) {
			_return(p->value);
		} else {
			p = p->dalsi;
		}
	}
	assert(0);
	_return(0);
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
