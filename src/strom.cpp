/* strom.cpp */

#include <vector>

#include "debug.h"
#include "strom.h"
#include "tabsym.h"

#include "code.h"

extern TmSource *g_ts;
extern Storage *g_s;

/******************************************************************************/

/******************************************************************************/

Var::Var(int a, bool rv) {
	_fn();
	addr = a;
	rvalue = rv;
	_return_void;
}

Numb::Numb(int v) {
	_fn();
	value = v;
	_return_void;
}

int Numb::Value() {
	_fn();
	_return(value);
}

Bop::Bop(Operator o, Expr *l, Expr *r) {
	_fn();
	op = o;
	left = l;
	right = r;
	_return_void;
}

Bop::~Bop() {
	_fn();
	delete left;
	delete right;
	_return_void;
}

UnMinus::UnMinus(Expr *e) {
	_fn();
	expr = e;
	_return_void;
}

UnMinus::~UnMinus() {
	_fn();
	delete expr;
	_return_void;
}

Assign::Assign(Var *v, Expr *e) {
	_fn();
	var = v;
	expr = e;
	_return_void;
}

Assign::~Assign() {
	_fn();
	delete var;
	delete expr;
	_return_void;
}

Write::Write(Expr *e) {
	_fn();
	expr = e;
	_return_void;
}

Write::~Write() {
	_fn();
	delete expr;
	_return_void;
}

Read::Read(Var *v) {
	_fn();
	var = v;
	_return_void;
}

Read::~Read() {
	_fn();
	delete var;
	_return_void;
}

If::If(Expr *c, Statm *ts, Statm *es) {
	_fn();
	cond = c;
	thenstm = ts;
	elsestm = es;
	_return_void;
}

If::~If() {
	_fn();
	delete cond;

	if (thenstm) {
		delete thenstm;
	}

	if (elsestm) {
		delete elsestm;
	}

	_return_void;
}

While::While(Expr *c, Statm *b) {
	_fn();
	cond = c;
	body = b;
	_return_void;
}

While::~While() {
	_fn();
	delete body;
	_return_void;
}

StatmList::StatmList(Statm *s, StatmList *n) {
	_fn();
	statm = s;
	next = n;
	_return_void;
}

StatmList::~StatmList() {
	_fn();
	delete statm;
	delete next;
	_return_void;
}

Prog::Prog(StatmList *s) {
	_fn();
	stm = s;
	_return_void;
}

Prog::~Prog() {
	_fn();
	delete stm;
	_return_void;
}

/****************************************************************** OPTIMIZE */

Node *Bop::Optimize() {
	_fn();
	left = (Expr *) (left->Optimize());
	right = (Expr *) (right->Optimize());
	Numb *l = dynamic_cast<Numb *>(left);
	Numb *r = dynamic_cast<Numb *>(right);

	if (!l || !r) {
		return this;
	}

	int res;
	int leftval = l->Value();
	int rightval = r->Value();

	switch (op) {
	case Plus:
		res = leftval + rightval;
		break;

	case Minus:
		res = leftval - rightval;
		break;

	case Times:
		res = leftval * rightval;
		break;

	case Divide:
		res = leftval / rightval;
		break;

	case Eq:
		res = leftval == rightval;
		break;

	case NotEq:
		res = leftval != rightval;
		break;

	case Less:
		res = leftval < rightval;
		break;

	case Greater:
		res = leftval > rightval;
		break;

	case LessOrEq:
		res = leftval <= rightval;
		break;

	case GreaterOrEq:
		res = leftval >= rightval;
		break;

	case Error: //nenastane
	default:
		res = 0;
		assert(0);
		break;
	}

	delete this;
	_return(new Numb(res));
}

Node *UnMinus::Optimize() {
	_fn();
	expr = (Expr *) expr->Optimize();
	Numb *e = dynamic_cast<Numb *>(expr);

	if (!e) {
		_return(this);
	}

	e = new Numb(-e->Value());
	delete this;
	_return(e);
}

Node *Assign::Optimize() {
	_fn();
	expr = (Var *) (expr->Optimize());
	_return(this);
}

Node *Write::Optimize() {
	_fn();
	expr = (Expr *) (expr->Optimize());
	_return(this);
}

Node *Read::Optimize() {
	_fn();
	//FIXME?
	_return(this);
}

Node *If::Optimize() {
	_fn();
	cond = (Expr *) (cond->Optimize());
	thenstm = (Statm *) (thenstm->Optimize());

	if (elsestm) {
		elsestm = (Statm *) (elsestm->Optimize());
	}

	Numb *c = dynamic_cast<Numb *>(cond);

	if (!c) {
		_return(this);
	}

	Node *res;

	if (c->Value()) {
		res = thenstm;
		thenstm = 0;
	} else {
		res = elsestm;
		elsestm = 0;
	}

	delete this;
	_return(res);
}

Node *While::Optimize() {
	_fn();
	cond = (Expr *) (cond->Optimize());
	body = (Statm *) (body->Optimize());
	Numb *c = dynamic_cast<Numb *>(cond);

	if (!c) {
		_return(this);
	}

	if (!c->Value()) {
		delete this;
		_return(new Empty);
	}

	_return(this);
}

Node *StatmList::Optimize() {
	_fn();
	StatmList *s = this;

	do {
		s->statm = (Statm *) (s->statm->Optimize());
		s = s->next;
	} while (s);

	_return(this);
}

Node *Prog::Optimize() {
	_fn();
	stm = (StatmList *) (stm->Optimize());
	_return(this);
}

/***************************************************************** TRANSLATE */

void Var::Translate() {
	_fn();
	g_ts->addInstr(LD, g_s->push(), addr, Storage::zero);
	_return_void;
}

void Numb::Translate() {
	_fn();
	if (g_debug_level > 5)
		fprintf(stderr, "numb=%d\n", value);
	g_ts->addInstr(LDC, g_s->push(), value, 0);
	_return_void;
}

/* vysledek bopu na vrcholu zasobniku */
void Bop::Translate() {
	_fn();

	if (g_debug_level > 5)
		fprintf(stderr, "left translate\n");
	left->Translate(); //result of the left is on the top

	if (g_debug_level > 5)
		fprintf(stderr, "right translate\n");
	right->Translate(); // result of the right is on the top

	int ra = g_s->pop();
	int rb = g_s->top();

	switch (op) {
	case Plus:
		g_ts->addInstr(ADD, rb, rb, ra, "op +");
		break;
	case Minus:
		g_ts->addInstr(SUB, rb, rb, ra, "op -");
		break;
	case Times:
		g_ts->addInstr(MUL, rb, rb, ra, "op *");
		break;
	case Divide:
		g_ts->addInstr(DIV, rb, rb, ra, "op /");
		break;
	case Less:
		g_ts->addInstr(SUB, rb, ra, rb, "op <");
		g_ts->addInstr(JLT, rb, 2, Storage::pc);
		g_ts->addInstr(LDC, rb, 0, rb);
		g_ts->addInstr(LDA, Storage::pc, 1, Storage::pc);
		g_ts->addInstr(LDC, rb, 1, rb);
		break;
	case Eq:
		g_ts->addInstr(SUB, rb, ra, rb, "op ==");
		g_ts->addInstr(JEQ, rb, 2, Storage::pc);
		g_ts->addInstr(LDC, rb, 0, rb);
		g_ts->addInstr(LDA, Storage::pc, 1, Storage::pc);
		g_ts->addInstr(LDC, rb, 1, rb);
		break;
	case NotEq:
		g_ts->addInstr(SUB, rb, ra, rb, "op !=");
		g_ts->addInstr(JNE, rb, 2, Storage::pc);
		g_ts->addInstr(LDC, rb, 0, rb);
		g_ts->addInstr(LDA, Storage::pc, 1, Storage::pc);
		g_ts->addInstr(LDC, rb, 1, rb);
		break;
	case Greater:
		g_ts->addInstr(SUB, rb, ra, rb, "op >");
		g_ts->addInstr(JGT, rb, 2, Storage::pc);
		g_ts->addInstr(LDC, rb, 0, rb);
		g_ts->addInstr(LDA, Storage::pc, 1, Storage::pc);
		g_ts->addInstr(LDC, rb, 1, rb);
		break;
	case LessOrEq:
		g_ts->addInstr(SUB, rb, ra, rb, "op <=");
		g_ts->addInstr(JLE, rb, 2, Storage::pc);
		g_ts->addInstr(LDC, rb, 0, rb);
		g_ts->addInstr(LDA, Storage::pc, 1, Storage::pc);
		g_ts->addInstr(LDC, rb, 1, rb);
		break;
	case GreaterOrEq:
		g_ts->addInstr(SUB, rb, ra, rb, "op >=");
		g_ts->addInstr(JGE, rb, 2, Storage::pc);
		g_ts->addInstr(LDC, rb, 0, rb);
		g_ts->addInstr(LDA, Storage::pc, 1, Storage::pc);
		g_ts->addInstr(LDC, rb, 1, rb);
		break;
	default:
		break;
	}

	_return_void;
}

void UnMinus::Translate() {
	_fn();
	int p;
	int t;
	expr->Translate();
	g_ts->addInstr(LDC, g_s->push(), 0, 0);
	p = g_s->pop();
	t = g_s->top();
	g_ts->addInstr(SUB, t, p, t, "op -");
	_return_void;
}

void Assign::Translate() {
	_fn();
	expr->Translate(); // pushes result on top of the stack
	g_ts->addInstr(ST, g_s->top(), var->addr, Storage::zero);
	(void) g_s->pop();
	_return_void;
}

void Write::Translate() {
	_fn();
	expr->Translate();
	g_ts->addInstr(OUT, g_s->pop(), 0, 0);
	_return_void;
}

void Read::Translate() {
	_fn();
	int reg = g_s->push();
	g_ts->addInstr(IN, reg, 0, 0);
	g_ts->addInstr(ST, reg, var->addr, Storage::zero);
	(void) g_s->pop();
	_return_void;
}

void If::Translate() {
	_fn();

	int a1 = 0;
	int a2 = 0;

	a1 = a2;
	a2 = a1;

	cond->Translate();

	thenstm->Translate();

	if (elsestm) {
		elsestm->Translate();
	} else {
		//
	}

	_return_void;
}

void While::Translate() {
	_fn();
	int a1 = 0;
	int a2 = 0;

	a1 = a2;
	a2 = a1;

	cond->Translate();
	body->Translate();
	_return_void;
}

void StatmList::Translate() {
	_fn();
	StatmList *s = this;

	do {
		s->statm->Translate();
		s = s->next;
	} while (s);

	_return_void;
}

void Prog::Translate() {
	_fn();
	if (0 < g_debug_level)
		fprintf(stderr, "-----------------------------------------TRANSLATE\n");
	//TODO: kontrola, jestli mam dost pameti - pridat do readme
//	g_ts->addInstr(LD, 6, 0, 0);
//	g_ts->addInstr(ST, 0, 0, 0);
	stm->Translate();
	g_ts->addInstr(HALT, 0, 0, 0);
	_return_void;
}

Expr *VarOrConst(char *id) {
	_fn();
	int v;
	DruhId druh = idPromKonst(id, &v);

	switch (druh) {
	case IdProm:
		_return(new Var(v, true));
		break;

	case IdKonst:
		_return(new Numb(v));
		break;

	case Nedef:
	default:
		_return(0);
		break;
	}
}
