/* parser.cpp */

#include "debug.h"
#include "strom.h"
#include "parser.h"
#include "lexan.h"
#include "tabsym.h"
#include <stdio.h>
#include <string.h>

Prog *Program();
void Dekl();
void DeklKonst();
void ZbDeklKonst();
void DeklProm();
void ZbDeklProm();
StatmList *SlozPrikaz();
StatmList *ZbPrikazu();
Statm *Prikaz();
Statm *CastElse();
Expr *Podminka();
Operator RelOp();
Expr *Vyraz();
Expr *ZbVyrazu(Expr *);
Expr *Term();
Expr *ZbTermu(Expr *);
Expr *Faktor();

void Srovnani(LexSymbol s) {
	_fn();

	if (Symb == s) {
		CtiSymb();
	} else {
		ChybaSrovnani(s);
	}

	_return_void;
}

void Srovnani_IDENT(char *id) {
	_fn();

	if (Symb == IDENT) {
		strcpy(id, Ident);
		CtiSymb();
	} else {
		ChybaSrovnani(IDENT);
	}

	_return_void;
}

void Srovnani_NUMB(int *h) {
	_fn();

	if (Symb == NUMB) {
		*h = Cislo;
		CtiSymb();
	} else {
		ChybaSrovnani(NUMB);
	}

	_return_void;
}

Prog *Program() {
	_fn();
	Dekl();
	_return(new Prog(SlozPrikaz()));
}

void Dekl() {
	_fn();

	switch (Symb) {
	case kwVAR:
		DeklProm();
		Dekl();
		break;
	case kwCONST:
		DeklKonst();
		Dekl();
		break;
	default:
		break;
	}

	_return_void;
}

void DeklKonst() {
	_fn();
	char id[MaxLenIdent];
	int hod = -666;
	CtiSymb();
	Srovnani_IDENT(id);
	Srovnani(EQ);
	Srovnani_NUMB(&hod);
	deklKonst(id, hod);
	ZbDeklKonst();
	Srovnani(SEMICOLON);
	_return_void;
}

void ZbDeklKonst() {
	_fn();

	if (Symb == COMMA) {
		char id[MaxLenIdent];
		int hod = -666;
		CtiSymb();
		Srovnani_IDENT(id);
		Srovnani(EQ);
		Srovnani_NUMB(&hod);
		deklKonst(id, hod);
		ZbDeklKonst();
	}

	_return_void;
}

void DeklProm() {
	_fn();
	char id[MaxLenIdent];
	CtiSymb();
	Srovnani_IDENT(id);
	deklProm(id);
	ZbDeklProm();
	Srovnani(SEMICOLON);
	_return_void;
}

void ZbDeklProm() {
	_fn();

	if (Symb == COMMA) {
		char id[MaxLenIdent];
		CtiSymb();
		Srovnani_IDENT(id);
		deklProm(id);
		ZbDeklProm();
	}

	_return_void;
}

StatmList *SlozPrikaz() {
	_fn();
	Srovnani(kwBEGIN);
	Statm *p = Prikaz();
	StatmList *su = new StatmList(p, ZbPrikazu());
	Srovnani(kwEND);
	_return(su);
}

StatmList *ZbPrikazu() {
	_fn();

	if (Symb == SEMICOLON) {
		CtiSymb();
		Statm *p = Prikaz();
		_return(new StatmList(p, ZbPrikazu()));
	}

	_return(0);
}

Statm *Prikaz() {
	_fn();
	if (g_debug_level > 5)
		fprintf(stderr, "Symb: %d\n", Symb);

	switch (Symb) {
	case IDENT: {
		Var *var = new Var(adrProm(Ident), false, Ident);
		CtiSymb();
		Srovnani(ASSGN);
		_return(new Assign(var, Vyraz()));
		break;
	}

	case kwWRITE:
		CtiSymb();
		_return(new Write(Vyraz()));
		break;

	case kwREAD: {
		CtiSymb();
		Var *var = new Var(adrProm(Ident), false, Ident);
		CtiSymb();
		_return(new Read(var));
		break;
	}

	case kwIF: {
		CtiSymb();
		Expr *cond = Podminka();
		Srovnani(kwTHEN);
		Statm *prikaz = Prikaz();
		_return(new If(cond, prikaz, CastElse()));
		break;
	}

	case kwWHILE: {
		Expr *cond;
		CtiSymb();
		cond = Podminka();
		Srovnani(kwDO);
		_return(new While(cond, Prikaz()));
		break;
	}

	case kwBEGIN:
		_return(SlozPrikaz());
		break;

	default:
		_return(new Empty);
		break;
	}
}

Statm *CastElse() {
	_fn();

	if (Symb == kwELSE) {
		CtiSymb();
		_return(Prikaz());
	}

	_return(0);
}

Expr *Podminka() {
	_fn();
	Expr *left = Vyraz();
	Operator op = RelOp();
	Expr *right = Vyraz();
	_return(new Bop(op, left, right));
}

Operator RelOp() {
	_fn();

	switch (Symb) {
	case EQ:
		CtiSymb();
		_return(Eq);
		break;

	case NEQ:
		CtiSymb();
		_return(NotEq);
		break;

	case LT:
		CtiSymb();
		_return(Less);
		break;

	case GT:
		CtiSymb();
		_return(Greater);
		break;

	case LTE:
		CtiSymb();
		_return(LessOrEq);
		break;

	case GTE:
		CtiSymb();
		_return(GreaterOrEq);
		break;

	default:
		Chyba("neocekavany symbol");
		_return(Error);
		break;
	}
}

Expr *Vyraz() {
	_fn();

	if (Symb == MINUS) {
		CtiSymb();
		_return(ZbVyrazu(new UnMinus(Term())));
	}

	_return(ZbVyrazu(Term()));
}

Expr *ZbVyrazu(Expr *du) {
	_fn();

	switch (Symb) {
	case PLUS:
		CtiSymb();
		_return(ZbVyrazu(new Bop(Plus, du, Term())));
		break;

	case MINUS:
		CtiSymb();
		_return(ZbVyrazu(new Bop(Minus, du, Term())));
		break;

	default:
		_return(du);
		break;
	}
}

Expr *Term() {
	_fn();
	_return(ZbTermu(Faktor()));
}

Expr *ZbTermu(Expr *du) {
	_fn();

	switch (Symb) {
	case TIMES:
		CtiSymb();
		_return(ZbTermu(new Bop(Times, du, Faktor())))
		;
		break;

	case DIVIDE:
		CtiSymb();
		_return(ZbTermu(new Bop(Divide, du, Faktor())))
		;
		break;

	default:
		_return(du)
		;
	}
}

Expr *Faktor() {
	_fn();

	switch (Symb) {
	case IDENT:
		char id[MaxLenIdent];
		Srovnani_IDENT(id);
		_return(VarOrConst(id));
		break;

	case NUMB: {
		int hodn = -666;
		Srovnani_NUMB(&hodn);
		_return(new Numb(hodn));
		break;
	}

	case LPAR: {
		CtiSymb();
		Expr *su = Vyraz();
		Srovnani(RPAR);
		_return(su);
		break;
	}

	default:
		Chyba("neocekavany symbol");
		_return(0);
		break;
	}
}
